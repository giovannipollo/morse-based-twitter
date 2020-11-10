#include "Arduino.h";
#include "tpl_os.h";

static int enable = 1;
static int index_frase = 0;

static const int len_parola_alfabeto[] = {2, 4, 4, 3, 1, 4, 3, 4, 2, 4, 3, 4, 2, 2, 3, 4, 4, 3, 3, 1, 3, 4, 3, 4, 4, 4};

static const int len_frase[] = {5, 5, 3, 4, 6};

static const char *alfabeto[] = {
    ".-",   // A
    "-...", // B
    "-.-.", // C
    "-..",  // D
    ".",    // E
    "..-.", // F
    "--.",  // G
    "....", // H
    "..",   // I
    ".---", // J
    "-.-",  // K
    ".-..", // L
    "--",   // M
    "-.",   // N
    "---",  // O
    ".--.", // P
    "--.-", // Q
    ".-.",  // R
    "...",  // S
    "-",    // T
    "..-",  // U
    "...-", // V
    ".--",  // W
    "-..-", // X
    "-.--", // Y
    "--..", // Z
};

static const char *frasi[] = {
    "CIAO",
    "COME",
    "VA",
    "QUA",
    "TUTTO",
};

void setup() {
    Serial.begin(115200);
    pinMode(13, OUTPUT);
}

TASK(Waiting) {
    EventMaskType big_time_mask, small_time_mask;

    while (1) {
        WaitEvent(BigTime);
        GetEvent(Waiting, &bit_time_mask);
        if (big_time_mask) {
            ClearEvent(BigTime);
            enable = 0;
            if (index_frase == 4) {
                index_frase = 0;
            } else {
                index_frase++;
            }
            WaitEvent(SmallTime);
            GetEvent(Waiting, &small_time_mask);
            if (small_time_mask) {
                ClearEvent(small_time_mask);
                enable = 1;
            }
        }
    }
}

TASK(Translator) {
    EventMaskType millisec_mask;
    int pos;
    while (1) {
        if (enable) {
            for (int i = 0; i < len_frase[index_frase]; i++) {

                if (frasi[index_frase][i] == 32) {

                    i++; // used to jump over spaces

                    // need to add led OFF for x times based on violante answer
                } else {

                    pos = frasi[index_frase][i] - 'A';
                    for (int j = 0; j < len_parola_alfabeto[pos]; j++) {

                        if (alfabeto[pos][j] == '.') {

                            // need to switch ON the led once
                            WaitEvent(Millisec);
                            GetEvent(Traslator, &millisec_mask);
                            if (millisec_mask) {
                                ClearEvent(Millisec);
                                digitalWrite(13, HIGH);
                                Serial.println("1");
                            }
                        } else {

                            // need to switch ON the led three times
                            for (int k = 0; k < 3 && enable; k++) {
                                WaitEvent(Millisec);
                                GetEvent(Traslator, &millisec_mask);
                                if (millisec_mask) {
                                    ClearEvent(Millisec);
                                    digitalWrite(13, HIGH);
                                    Serial.println("1");
                                }
                            }
                        }

                        // After a symbol I switch the led OFF once
                        WaitEvent(Millisec);
                        GetEvent(Traslator, &millisec_mask);
                        if (millisec_mask) {
                            ClearEvent(Millisec);
                            digitalWrite(13, LOW);
                            Serial.println("0");
                        }
                    }

                    // After a codeword I switch OFF the led two times
                    for (int l = 0; l < 2 && enable; l++) {
                        WaitEvent(Millisec);
                        GetEvent(Traslator, &millisec_mask);
                        if (millisec_mask) {
                            ClearEvent(Millisec);
                            digitalWrite(13, LOW);
                            Serial.println("0");
                        }
                    }
                }

                // before the end of the for loop I need to check if I reach the end of
                // the frase and if enable = 1, I need reset the index of the loop,
                // because I need to restart from the beginning of the frase. At the
                // same time, because the word is ended, I need to switch the led OFF
                // for four times.

                if ((i == len_frase[index_frase] - 1) && (enable)) {
                    i = 0;
                    for (int l = 0; l < 4 && enable; l++) {
                        WaitEvent(Millisec);
                        GetEvent(Traslator, &millisec_mask);
                        if (millisec_mask) {
                            ClearEvent(Millisec);
                            digitalWrite(13, LOW);
                            Serial.println("0");
                        }
                    }
                }
            }

            // at the end of the word I need to switch led OFF for four times
            for (int l = 0; l < 4 && enable; l++) {
                WaitEvent(Millisec);
                GetEvent(Traslator, &millisec_mask);
                if (millisec_mask) {
                    ClearEvent(Millisec);
                    digitalWrite(13, LOW);
                    Serial.println("0");
                }
            }

        } else {
            // enable = 0, and so it means I'm in the 0.5s pause, and so I need to
            // keep the led OFF for five times
            for (int i = 0; i < 5; i++) {
                WaitEvent(Millisec);
                GetEvent(Traslator, &millisec_mask);
                if (millisec_mask) {
                    ClearEvent(Millisec);
                    digitalWrite(13, LOW);
                    Serial.println("0");
                }
            }
        }
    }
}
