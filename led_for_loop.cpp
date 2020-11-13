

#include "Arduino.h"
#include "tpl_os.h"

static const char *frasi[] = {
    "CIAO",
    "AOIC",
};

void setup() {
    Serial.begin(115200);
    // initialize digital pin 13 as an output.
    pinMode(13, OUTPUT);
}

TASK(periodicTask) {

    char led = '0';
    int index = 0;
    int cnt = 0;
    EventMaskType mask;
    while (1) {
        for (int k = 0; k < 2; k++) {
            for (int i = 0; i < 4; i++) {
                if (frasi[k][i] == 'C') {
                    index = 4;
                    led = '0';
                }
                if (frasi[k][i] == 'I') {
                    index = 2;
                    led = '1';
                }
                if (frasi[k][i] == 'A') {
                    index = 1;
                    led = '0';
                }
                if (frasi[k][i] == 'O') {
                    index = 4;
                    led = '1';
                }

                // write value of leds

                for (int i = 0; i < index && cnt < 180; i++) {
                    WaitEvent(evento);
                    GetEvent(periodicTask, &mask);
                    if (mask) {
                        ClearEvent(evento);
                        if (led == '1') {
                            digitalWrite(13, HIGH);
                            Serial.print("1");
                            Serial.print("         cnt = ");
                            Serial.println(cnt);
                        } else {
                            digitalWrite(13, LOW);
                            Serial.print("0");
                            Serial.print("         cnt = ");
                            Serial.println(cnt);
                        }
                        cnt++;
                    }
                }

                // check if the end of the loop restart

                if (i == 3) {
                    i = -1; // -1 Because at the next cycle it will increment by 1 and so it will return to 0
                }

                // pause

                if (cnt == 180) {
                    i = 3; // così da uscire dal loop
                    cnt = 0;
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
            if (k == 1) {
                k = -1; // -1 Because at the next cycle it will increment by 1 and so it will return to 0
            }
        }
    }
}
