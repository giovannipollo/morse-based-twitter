

#include "Arduino.h"
#include "tpl_os.h"

char prova[] = "CIAO";

void setup() {
    Serial.begin(115200);
    // initialize digital pin 13 as an output.
    pinMode(13, OUTPUT);
}

TASK(periodicTask) {

    EventMaskType mask;
    while (1) {
        for (int i = 0; i < 4; i++) {
            if (prova[i] == 'C') {
                for (int j = 0; j < 4; j++) {
                    WaitEvent(evento);
                    GetEvent(periodicTask, &mask);
                    if (mask) {
                        ClearEvent(evento);
                        digitalWrite(13, LOW);
                        Serial.println("0");
                    }
                }
            }
            if (prova[i] == 'I') {
                for (int j = 0; j < 2; j++) {
                    WaitEvent(evento);
                    GetEvent(periodicTask, &mask);
                    if (mask) {
                        ClearEvent(evento);
                        digitalWrite(13, HIGH);
                        Serial.println("1");
                    }
                }
            }
            if (prova[i] == 'A') {
                for (int j = 0; j < 1; j++) {
                    WaitEvent(evento);
                    GetEvent(periodicTask, &mask);
                    if (mask) {
                        ClearEvent(evento);
                        digitalWrite(13, LOW);
                        Serial.println("0");
                    }
                }
            }
            if (prova[i] == 'O') {
                for (int j = 0; j < 4; j++) {
                    WaitEvent(evento);
                    GetEvent(periodicTask, &mask);
                    if (mask) {
                        ClearEvent(evento);
                        digitalWrite(13, HIGH);
                        Serial.println("1");
                    }
                }
            }
            if (i == 3) {
                i = -1;
                for (int j = 0; j < 5; j++) {
                    WaitEvent(evento);
                    GetEvent(periodicTask, &mask);
                    if (mask) {
                        ClearEvent(evento);
                        digitalWrite(13, LOW);
                        Serial.println("0         Pause");
                    }
                }
            }
        }
    }
}
