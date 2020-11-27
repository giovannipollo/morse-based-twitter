#include "Arduino.h"
#include "tpl_os.h"

// define the number of setences and the value of the CNT for the pause
#define NUMFRASI 5
#define MAXCNT 600

// #define DEBUG

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
    //"CIAQ",
    "A SHORT PENCIL IS USUALLY BETTER THAN A LONG MEMORY ANY DAY",
    "ACCEPT SOMETHING THAT YOU CANNOT CHANGE AND YOU WILL FEEL BETTER",
    "ADVENTURE CAN BE REAL HAPPINESS",
    "ALL THE EFFORT YOU ARE MAKING WILL ULTIMATELY PAY OFF",
};

static char LED[20];

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
#ifdef DEBUG
    Serial.begin(115200);
#endif
    // initialize digital pin 13 as an output.
    pinMode(13, OUTPUT);
}

TASK(periodicTask) {

    uint8 index = 0;
    uint16 cnt = 0;
    int pos = 0;
    uint8 i, j, k;
    EventMaskType mask;
#ifdef DEBUG
    Serial.println("");
#endif
    while (1) {
        i = 0;
        //loop on all sentences
        while (i < NUMFRASI) {
#ifdef DEBUG
            Serial.print("Numero Frase: ");
            Serial.println(i);
            Serial.print("Lunghezza Frase: ");
            Serial.println(string_lenght(frasi[i]));

#endif
            j = 0;
            // loop on one sentence
            while (j < string_lenght(frasi[i])) {
                pos = frasi[i][j] - 'A';
#ifdef DEBUG
                Serial.print("Lettera: ");
                Serial.print(frasi[i][j]);
                Serial.print("      Posizione: ");
                Serial.println(pos);
#endif
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

#ifdef DEBUG
                Serial.print("Lunghezza: ");
                Serial.println(index);
#endif

                // write value of leds
                k = 0;
                while (k < index && cnt < MAXCNT) {
                    WaitEvent(evento);
                    GetEvent(periodicTask, &mask);
                    if (mask) {
                        ClearEvent(evento);
                        if (LED[k] == '1') {
                            digitalWrite(13, HIGH);
#ifdef DEBUG
                            Serial.print("1");
                            Serial.print("         cnt = ");
                            Serial.println(cnt);
#endif
                        } else {
                            digitalWrite(13, LOW);
#ifdef DEBUG
                            Serial.print("0");
                            Serial.print("         cnt = ");
                            Serial.println(cnt);
#endif
                        }
                        cnt++;
                        k++;
                    }
                }
                index = 0; // because I finish a codeword

                // pause if cnt is equal to MAXCNT
                if (cnt == MAXCNT) {
                    j = string_lenght(frasi[i + 1]); // set j to the lenght of the sentence so that we exit from the while loop
                    cnt = 0;                         // reset the counter
                    i++;                             // when I exit from the while loop (after the pause) I need to go to the next sentence. So I increase the value of i
                    digitalWrite(13, LOW);           // switch off the LED
                    k = 0;
                    while (k < 5) {
                        WaitEvent(evento);
                        GetEvent(periodicTask, &mask);
                        ClearEvent(evento);
                        k++;
#ifdef DEBUG
                        Serial.println("0         Pause");
#endif
                    }
                }
            }
        }
    }
}
