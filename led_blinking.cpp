#include "Arduino.h"
#include "tpl_os.h"

// define the number of setences and the value of the CNT for the pause
// Use DEFINE in order to make the code general
#define NUMFRASI 5
#define MAXCNT 1800

static const char *alfabeto[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
};

static const char *frasi[] = {
    "A FEATHER IN THE HAND IS BETTER THAN A BIRD IN THE AIR",
    "A SHORT PENCIL IS USUALLY BETTER THAN A LONG MEMORY ANY DAY",
    "ACCEPT SOMETHING THAT YOU CANNOT CHANGE AND YOU WILL FEEL BETTER",
    "ADVENTURE CAN BE REAL HAPPINESS",
    "ALL THE EFFORT YOU ARE MAKING WILL ULTIMATELY PAY OFF",
};

static char LED[20]; // use to save the sequence of 0 and 1 for a single letter
// 20 because the longest letter is Q or Y or J, that can have maximum 20 characters. For example Q = "--.-"
// -    1110
// -    1110
// .    10
// -    1110
//      00
//      0000
// If we count, we can have maximum 20 char

uint8 string_lenght(const char *string) {
    uint8 i = 0;
    while (string[i] != '\0') {
        i++;
    }
    return i;
}

void populateLED(char value, uint8 num_volte, uint8 *index) {
    uint8 i;
    for (i = *index; i < num_volte + *index; i++) {
        LED[i] = value;
    }
    *index = i;
}

void setup() {
    pinMode(13, OUTPUT); // initialize digital pin 13 as an output.
}

TASK(periodicTask) {

    uint8 index = 0;
    uint16 cnt = 0;
    int pos = 0;
    uint8 i, j, k;
    EventMaskType mask;
    while (1) {
        i = 0;
        while (i < NUMFRASI) { //loop on all sentences
            j = 0;
            while (j < string_lenght(frasi[i])) { // loop on the letters of one sentence
                pos = frasi[i][j] - 'A';          //offset compared to A
                k = 0;

                // pos >= 0 is used ot avoid looping in case I have a space. In fact space is 32 in ascii and 32 - 'A' is < 0.
                // So I loop only if pos is >= 0
                while (pos >= 0 && k < string_lenght(alfabeto[pos])) {
                    if (alfabeto[pos][k] == '.')
                        populateLED('1', 1, &index); // 1 for the symbol .
                    else
                        populateLED('1', 3, &index); // 1 for the symbol -

                    populateLED('0', 1, &index); // 0 at the end of the symbol
                    k++;

                    if (k == string_lenght(alfabeto[pos]))
                        populateLED('0', 2, &index); // 0 at the end of the codeword
                }

                if (frasi[i][j] == 32 || j == string_lenght(frasi[i]) - 1)
                    populateLED('0', 4, &index); // 0 because of a space or because I finish the sentence

                j++;

                // write value of leds with digitalWrite
                k = 0;
                while (k < index && cnt < MAXCNT) {
                    WaitEvent(evento);
                    GetEvent(periodicTask, &mask);
                    if (mask) {
                        ClearEvent(evento);
                        if (LED[k] == '1')
                            digitalWrite(13, HIGH);
                        else
                            digitalWrite(13, LOW);

                        cnt++;
                        k++;
                    }
                }
                index = 0; // because I finish a codeword

                // pause if cnt is equal to MAXCNT, because I reached 180s
                if (cnt == MAXCNT) {

                    // Increment i before calculating j because the condition of the while(j < string_lenght(frasi[i])) statement is based on i.
                    // So I need to compute j with the incremented value.

                    i++;
                    j = string_lenght(frasi[i]); // value of j in order to exit from the loop while(j < string_lenght(frasi[i])) and go to the next sentence
                    cnt = 0;                     // reset the counter
                    digitalWrite(13, LOW);       // switch off the LED
                    k = 0;

                    // 500ms pause
                    while (k < 5) {
                        WaitEvent(evento);
                        GetEvent(periodicTask, &mask);
                        if (mask) {
                            ClearEvent(evento);
                            k++;
                        }
                    }
                }
            }
        }
    }
}
