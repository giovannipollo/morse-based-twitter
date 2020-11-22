#include "Arduino.h"
#include "tpl_os.h"

#define NUMFRASI 5
#define MAXCNT 50

#define DEBUG

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

static char LED[16];

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
        while (i < NUMFRASI) {

#ifdef DEBUG
            Serial.print("Numero Frase: ");
            Serial.println(i);
            Serial.print("Lunghezza Frase: ");
            Serial.println(string_lenght(frasi[i]));

#endif
            j = 0;
            while (j < string_lenght(frasi[i])) {
                pos = frasi[i][j] - 'A';
#ifdef DEBUG
                Serial.print("Lettera: ");
                Serial.print(frasi[i][j]);
                Serial.print("      Posizione: ");
                Serial.println(pos);
#endif
                k = 0;
                while (pos >= 0 && k < string_lenght(alfabeto[pos])) {
                    if (alfabeto[pos][k] == '.')
                        populateLED('1', 1, &index);
                    else
                        populateLED('1', 3, &index);

                    populateLED('0', 1, &index); // 0 at the end of the symbol
                    k++;

                    if (k == string_lenght(alfabeto[pos]))
                        populateLED('0', 2, &index); // 0 at the end of the codeword
                }

                if (frasi[i][j] == 32 || frasi[i][j] == '\0')
                    populateLED('0', 4, &index); // 0 because of a space

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

                // pause

                if (cnt == MAXCNT) {
                    j = string_lenght(frasi[i]); // così da uscire dal loop
                    cnt = 0;
                    digitalWrite(13, LOW);
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
            i++;
        }
    }
}
