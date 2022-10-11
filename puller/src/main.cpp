#include <Arduino.h>
#include "TimerOne.h"
#include "HX711.h"


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
#define hxdout A1
#define hxclk A0

#define BUTTON1 A2
#define BUTTON2 A3
#define BUTTON3 A4
#define BUTTON4 A5

#define TICKS_PER_SECOND 300


HX711 loadcell;

void isr();

float measure_accurate();

float zero = 0;


#define MAX_BUFFER 50
char buffer[MAX_BUFFER];
int bufferpos = 0;

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

    loadcell.begin(hxdout, hxclk);
    loadcell.set_scale(-1);

    Serial.begin(9600);
}

#define preset_line_speed 1300 //1207 //1182

void loop() {

    static float speed = preset_line_speed;//1424;

    // put your main code here, to run repeatedly:
    

    // if (digitalRead(BUTTON1)==0) {speed += 2;
    // if (digitalRead(BUTTON2)==0) speed -= 2;

    float load = loadcell.get_value(1);

    if (digitalRead(BUTTON1)==0) digitalWrite(EN, 1);
    if (digitalRead(BUTTON2)==0) digitalWrite(EN, 0);
    if (digitalRead(BUTTON3)==0) {
        Serial.print("tare...");
        zero = loadcell.get_value(50);
        Serial.print("done");
    }
    // if (load - zero > 250000) {
    //     //runout detected
    //     digitalWrite(EN, 1);
    // }
    
    if (digitalRead(BUTTON4)==0) speed = preset_line_speed;

    if (speed >  2000) speed = 2000;
    if (speed <  1000) speed = 1000;


    Timer1.setPeriod(1000000.0f / speed);
    //speed += (load - zero) * (-0.00000025f);

    // Serial.print(load);
    // Serial.print(' ');
    // Serial.print(zero);
    // Serial.print(' ');
    Serial.print(load - zero);
    Serial.print(' ');
    Serial.print(speed);
    Serial.println();

    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\r' || c == '\n') {
            buffer[bufferpos] = 0;
            if (bufferpos <= 1) {
                break;
            }
            bufferpos = 0;
            int val = 0;
            if (sscanf(&buffer[1], "%d", &val)) {
                if (buffer[0] == 's') {
                    Serial.print("new speed:"); 
                    Serial.println(val);
                    speed = val;
                }
                if (buffer[0] == 'z') {
                    Serial.print("new zero:"); 
                    Serial.println(val);
                    zero = val;
                }
            }
        } else {
            buffer[bufferpos++] = c;
        }
        if (bufferpos > MAX_BUFFER) {
            bufferpos = 0;
        }
    }
}

void isr() {
    static bool toggle = false;
    digitalWrite(STEP, toggle);
    toggle = !toggle;
}

float measure_accurate() {
    float f = 0;
    const int n = 10;
    for (int i = 0; i < n; i++) {
        f += loadcell.get_value();
    }
    return f / (float)n;
}