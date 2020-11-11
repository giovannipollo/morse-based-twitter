// see blink.oil head of file for information about the compilation process.

#include "Arduino.h"
#include "tpl_os.h"

static int SOS[] = {
    1,
    0,
    1,
    1,
    1,
    1,
    0,
    1,
    1,
    1,
    1,
    0,
    1,
    1,
    1,
    1,
    0,
    1,
    1,
    1,
};
static int enable = 1;
void setup() {
    Serial.begin(115200);
    // initialize digital pin 13 as an output.
    pinMode(13, OUTPUT);
}

//The TASK is activated by the alarm "periodicAl":
//* The alarm "periodicAl" is configured in the .oil file to activate task
//"periodicTask" each 1000 pulses of counter SystemCounter.
//* The SystemCounter is the same as the systick used in Arduino, a tick each 1024us on a 16MHz arduino.
//* This task is periodic, with a period of 1024ms.
TASK(periodicTask) {
    static int counter = 0;
    static int counter_small = 0;
    if (SOS[counter] && enable) {
        digitalWrite(13, HIGH);
        Serial.println("1");
    } else {
        digitalWrite(13, LOW);
        Serial.println("0");
    }
    if (counter == 19) {

        // 0.5s pause
        if (counter_small == 5) {
            counter_small = 0;
            counter = 0;
            enable = 1;
        } else {
            enable = 0;
            counter_small++;
        }
    } else {
        counter++;
    }
}

/* 
TASK(periodicTask2) {
    EventMaskType prova;
    while (1) {
        WaitEvent(Eventprova);
        GetEvent(periodicTask2, &prova);
        if (prova) {
            ClearEvent(Eventprova);
            enable = !enable;
            Serial.print("enable: ");
            Serial.println(enable);
        }
    }
}
*/