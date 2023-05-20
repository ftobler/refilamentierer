#include <Arduino.h>
#include "TimerOne.h"


#define EN 2
#define STEP 6
#define DIR 7
#define LED1 8
#define LED2 9
#define LED3 10
#define LED4 11
#define M1 3
#define M2 4
#define M3 5

#define BUTTON1 A2
#define BUTTON2 A3
#define BUTTON3 A4
#define BUTTON4 A5

#define TICKS_PER_SECOND 200

void isr();

void setup() {
    // put your setup code here, to run once:

    Timer1.initialize(1000000 / TICKS_PER_SECOND);
    Timer1.attachInterrupt(isr); // blinkLED to run every 0.15 seconds

    pinMode(EN, OUTPUT);
    pinMode(STEP, OUTPUT);
    pinMode(DIR, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    pinMode(M3, OUTPUT);

    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);
    pinMode(BUTTON4, INPUT_PULLUP);

    digitalWrite(EN, 0);
    digitalWrite(DIR, 1);
    digitalWrite(M1, 1);
    digitalWrite(M2, 1);
    digitalWrite(M3, 0);

    Serial.begin(9600);

}

void loop() {

    static int speed = 250;
    int old_speed = speed;

    if (digitalRead(BUTTON1)==0) speed = 0;
    if (digitalRead(BUTTON2)==0) speed = 100;
    if (digitalRead(BUTTON3)==0) speed = 200;
    if (digitalRead(BUTTON4)==0) speed = 250;

    if (old_speed != speed) {
        Timer1.setPeriod(1000000 / speed / 2);
        Serial.println(speed);
    }

    digitalWrite(LED4, speed==0);
    digitalWrite(LED3, speed==100);
    digitalWrite(LED2, speed==200);
    digitalWrite(LED1, speed==250);

    delay(50);
}

void isr() {
    static bool toggle = false;
    digitalWrite(STEP, toggle);
    toggle = !toggle;
}