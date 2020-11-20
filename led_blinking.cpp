#include "Arduino.h"
#include "tpl_os.h"

#define NUMFRASI 5
#define MAXCNT 50

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

uint8 string_lenght(const char *string) {
    uint8 i = 0;
    while (string[i] != '\0') {
        i++;
    }
    return i;
}

static const char *frasi[] = {
    //"A FEATHER IN THE HAND IS BETTER THAN A BIRD IN THE AIR",
    "Q Q Q Q",
    "A SHORT PENCIL IS USUALLY BETTER THAN A LONG MEMORY ANY DAY",
    "ACCEPT SOMETHING THAT YOU CANNOT CHANGE AND YOU WILL FEEL BETTER",
    "ADVENTURE CAN BE REAL HAPPINESS",
    "ALL THE EFFORT YOU ARE MAKING WILL ULTIMATELY PAY OFF",
};

static char LED[21];
void setup() {

#ifdef DEBUG
    Serial.begin(115200);
#endif
    // initialize digital pin 13 as an output.
    pinMode(13, OUTPUT);
}

void populateLED(char value, int num_volte, uint8 *index) {
    int i;
    for (i = *index; i < num_volte + *index; i++) {
        LED[i] = value;
    }
    *index = i;
}

TASK(periodicTask) {

    uint8 index = 0;
    uint16 cnt = 0;
    uint8 pos = 0;
    uint8 k = 0;
    uint8 i = 0;
    uint8 l, m;
    EventMaskType mask;
#ifdef DEBUG
    Serial.println("");
#endif
    while (1) {
        while (k < NUMFRASI) {

#ifdef DEBUG
            Serial.print("Numero Frase: ");
            Serial.println(k);
            Serial.print("Lunghezza Frase: ");
            Serial.println(string_lenght(frasi[k]));

#endif
            while (i < string_lenght(frasi[k])) {
                pos = frasi[k][i] - 'A';
#ifdef DEBUG
                Serial.print("Lettera: ");
                Serial.print(frasi[k][i]);
                Serial.print("      Posizione: ");
                Serial.println(pos);
#endif
                l = 0;
                while (l < string_lenght(alfabeto[pos])) {
                    if (alfabeto[pos][l] == '.') {
                        populateLED('1', 1, &index);
                    } else {
                        populateLED('1', 3, &index);
                    }
                    populateLED('0', 1, &index); // 0 at the end of the symbol
                    l++;
                }
                populateLED('0', 2, &index); // 0 at the end of the codeword

                if (i == string_lenght(frasi[k]) - 1) {
                    i = 0;
                    populateLED('0', 4, &index); // 0 at the end of the sentence
                } else {
                    i++; // increment i if I'm not at the end of the word
                }

                if (frasi[k][i] == 32) {
                    i++;                         // increment because I have a space
                    populateLED('0', 4, &index); // 0 because of a space
                }

#ifdef DEBUG
                Serial.print("Lunghezza: ");
                Serial.println(string_lenght(LED));
#endif

                // write value of leds
                m = 0;
                while (m < index && cnt < MAXCNT) {
                    WaitEvent(evento);
                    GetEvent(periodicTask, &mask);
                    if (mask) {
                        ClearEvent(evento);
                        if (LED[m] == '1') {
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
                        m++;
                    }
                }
                index = 0; // because I finish a codeword

                // pause

                if (cnt == MAXCNT) {
                    i = string_lenght(frasi[k]); // così da uscire dal loop
                    cnt = 0;
                    for (int j = 0; j < 5; j++) {
                        WaitEvent(evento);
                        GetEvent(periodicTask, &mask);
                        if (mask) {
                            ClearEvent(evento);
                            digitalWrite(13, LOW);
#ifdef DEBUG
                            Serial.println("0         Pause");
#endif
                        }
                    }
                }
            }
            if (k == NUMFRASI - 1) {
                k = 0; // Reset because I reach the end of all phrases
            } else {
                k++;
            }
        }
    }
}
