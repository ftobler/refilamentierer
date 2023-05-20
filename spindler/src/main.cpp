#include <Arduino.h>
#include "TimerOne.h"


#define SWLEFT 3
#define SWRIGHT 2
#define MLEFT A5
#define MRIGHT A4



void setup() {
    pinMode(SWLEFT, INPUT_PULLUP);
    pinMode(SWRIGHT, INPUT_PULLUP);
    pinMode(MLEFT, OUTPUT);
    pinMode(MRIGHT, OUTPUT);

    Serial.begin(9600);

}

boolean direction = false;

#define ON_TIME 5

void loop() {
    for (int i = 0; i < 106; i++) {
        delay(100);
        if (digitalRead(SWLEFT) == 0) {
            break;
        }
        if (digitalRead(SWRIGHT) == 0) {
            break;
        }
    }
    
    if (digitalRead(SWLEFT) == 0) {
        direction = true;
    }
    if (digitalRead(SWRIGHT) == 0) {
        direction = false;
    }

    if (direction) {
        digitalWrite(MRIGHT, 1);
        digitalWrite(MLEFT, 0);
        delay(ON_TIME);
        digitalWrite(MRIGHT, 0);
        digitalWrite(MLEFT, 0);
    } else {
        digitalWrite(MRIGHT, 0);
        digitalWrite(MLEFT, 1);
        delay(ON_TIME);
        digitalWrite(MRIGHT, 0);
        digitalWrite(MLEFT, 0);
    }
}
