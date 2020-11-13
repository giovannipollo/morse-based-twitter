// see blink.oil head of file for information about the compilation process.

#include "Arduino.h"
#include "tpl_os.h"

static const char SOS[] = {"10111101111011110111"};
static int enable = 1;
void setup() {
    Serial.begin(115200);
    // initialize digital pin 13 as an output.
    pinMode(13, OUTPUT);
}

/*
TASK(periodicTask) {

    static int counter = 0;
    static int pause = 0;

    if (SOS[counter] && enable) {
        digitalWrite(13, HIGH);
        Serial.print("1");
        Serial.print("       counter: ");
        Serial.println(counter);

    } else {
        digitalWrite(13, LOW);
        Serial.print("0");
        Serial.print("       counter: ");
        if (enable) {
            Serial.println(counter);
        } else {
            Serial.print(counter);
            Serial.print("       pause: ");
            Serial.println(pause - 1);
        }
    }
    if (counter == 19) {
        enable = 0;
        if (pause == 5) {
            pause = 0;
            counter = 0;
            enable = 1;
        } else {
            pause++;
        }
    } else {
        counter++;
    }
    TerminateTask();
}
*/

TASK(eventTask) {

    static int counter = 0;
    static int pause = 0;
    EventMaskType one_sec_mask;

    while (1) {
        WaitEvent(one_sec);
        GetEvent(eventTask, &one_sec_mask);
        if (one_sec_mask) {
            ClearEvent(one_sec);
            if (SOS[counter] && enable) {
                digitalWrite(13, HIGH);
                Serial.print("1");
                Serial.print("       counter: ");
                Serial.println(counter);
            } else {
                digitalWrite(13, LOW);
                Serial.print("0");
                Serial.print("       counter: ");
                if (enable) {
                    Serial.println(counter);
                } else {
                    Serial.print(counter);
                    Serial.print("       pause: ");
                    Serial.println(pause - 1);
                }
            }
            if (counter == 19) {
                enable = 0;
                if (pause == 5) {
                    pause = 0;
                    counter = 0;
                    enable = 1;
                } else {
                    pause++;
                }
            } else {
                counter++;
            }
        }
    }
}