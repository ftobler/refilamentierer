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

#define ON_TIME 10

void loop() {
    
    delay(60000);
    
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
