#include "Arduino.h"
#include "tpl_os.h"

#define NUMFRASI 5
#define MAXCNT 900

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

static const int len_alfabeto[] = {2, 4, 4, 3, 1, 4, 3, 4, 2, 4, 3, 4, 2, 2, 3, 4, 4, 3, 3, 1, 3, 4, 3, 4, 4, 4};
static const int len_frase[] = {54, 59, 64, 31, 53};

static const char *frasi[] = {
    "A FEATHER IN THE HAND IS BETTER THAN A BIRD IN THE AIR",
    "A SHORT PENCIL IS USUALLY BETTER THAN A LONG MEMORY ANY DAY",
    "ACCEPT SOMETHING THAT YOU CANNOT CHANGE AND YOU WILL FEEL BETTER",
    "ADVENTURE CAN BE REAL HAPPINESS",
    "ALL THE EFFORT YOU ARE MAKING WILL ULTIMATELY PAY OFF",
};

static char LED[50];

void setup() {
    // Serial.begin(115200);
    // initialize digital pin 13 as an output.
    pinMode(13, OUTPUT);
}

void populateLED(char value, int num_volte, int *index) {
    int i;
    for (i = *index; i < num_volte + *index; i++) {
        LED[i] = value;
    }
    *index = i;
}

TASK(periodicTask) {

    int index = 0;
    int cnt = 0;
    int pos = 0;
    // Serial.println("");
    EventMaskType mask;
    while (1) {
        for (int k = 0; k < NUMFRASI; k++) {
            // Serial.print("Numero Frase: ");
            // Serial.println(k);
            for (int i = 0; i < len_frase[k]; i++) {
                if (frasi[k][i] != 32) {
                    pos = frasi[k][i] - 'A';
                    // Serial.print("Lettera: ");
                    // Serial.print(frasi[k][i]);
                    // Serial.print("      Posizione: ");
                    // Serial.println(pos);
                    for (int l = 0; l < len_alfabeto[pos]; l++) {
                        if (alfabeto[pos][l] == '.') {
                            populateLED('1', 1, &index);
                        } else {
                            populateLED('1', 3, &index);
                        }
                        populateLED('0', 1, &index); // 0 at the end of the symbol
                    }
                    populateLED('0', 2, &index); // 0 at the end of the codeword

                    // missing end of the frase & sapce (end of the word)
                    if (i == len_frase[k] - 1) {
                        populateLED('0', 4, &index); // 0 at the end of the codeword
                    }
                } else {
                    populateLED('0', 4, &index); // 0 for the space
                }

                // write value of leds

                for (int m = 0; m < index && cnt < MAXCNT; m++) {
                    WaitEvent(evento);
                    GetEvent(periodicTask, &mask);
                    if (mask) {
                        ClearEvent(evento);
                        if (LED[m] == '1') {
                            digitalWrite(13, HIGH);
                            // Serial.print("1");
                            // Serial.print("         cnt = ");
                            //Serial.println(cnt);
                        } else {
                            digitalWrite(13, LOW);
                            // Serial.print("0");
                            // Serial.print("         cnt = ");
                            // Serial.println(cnt);
                        }
                        cnt++;
                    }
                }
                index = 0; // because I finish a codeword

                // check if the end of the loop then restart

                if (i == len_frase[k] - 1) {
                    i = -1; // -1 Because at the next cycle it will increment by 1 and so it will return to 0
                }

                // pause

                if (cnt == MAXCNT) {
                    i = len_frase[k]; // così da uscire dal loop
                    cnt = 0;
                    for (int j = 0; j < 5; j++) {
                        WaitEvent(evento);
                        GetEvent(periodicTask, &mask);
                        if (mask) {
                            ClearEvent(evento);
                            digitalWrite(13, LOW);
                            // Serial.println("0         Pause");
                        }
                    }
                }
            }
            if (k == NUMFRASI - 1) {
                k = -1; // -1 Because at the next cycle it will increment by 1 and so it will return to 0
            }
        }
    }
}
