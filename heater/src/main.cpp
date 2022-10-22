#include <Arduino.h>

// Pins
#define PWMOUT 3
#define ADCIN A1
#define LED 13

// Control sample period
#define DT_MS 100


//PLA 180°C
//PET 185°C
//#define TARGET_TEMP 180
int TARGET_TEMP = 185;

//uart commands
#define MAX_BUFFER 50
char buffer[MAX_BUFFER];
int bufferpos = 0;


// PI Control parameters
#define P 0.0125f
#define I 0.0004f //0.0025f


//thermistor configuration
#define T0 (25.0f + 273.15f)
#define RT0 10000.0f   // Ω
#define B 3977.0f      // K
#define VCC 5.0f    //Supply voltage
#define R 10000.0f  //R=10KΩ




void setup() {
    // put your setup code here, to run once:

    Serial.begin(9600);

    pinMode(LED, OUTPUT);

    // analogReference(EXTERNAL);

    pinMode(PWMOUT, OUTPUT);
    TCCR2B = (TCCR2B & B11111000) | B00000111; // for PWM frequency of 31372.55 Hz

    analogWrite(PWMOUT, 0);
}


float analogReadMulti(int adc, unsigned long ms) {
    uint32_t start = micros();
    float sum = 0.0f;
    int n = 0;
    while (ms > 0) {
        yield();
        while ( ms > 0 && (micros() - start) >= 1000) {
            ms--;
            start += 1000;
        }
        sum += analogRead(adc);
        n++;
    }
    return sum / n;
}

float getTemp(void) {

    static float filtered = 0;

    float VRT = analogReadMulti(ADCIN, DT_MS); //Acquisition analog value of VRT

    // Serial.println(VRT);

    VRT = (5.00f / 1023.00f) * VRT; //Conversion to voltage
    float VR = VCC - VRT;
    float RT = VRT / (VR / R); //Resistance of RT

    float ln = log(RT / RT0);
    float TX = (1.0f / ((ln / B) + (1.0f / T0))); //Temperature from thermistor

    TX = TX - 273.15f; //Conversion to Celsius

    if (filtered == 0) filtered = TX;

    filtered = 0.9f * filtered + 0.1f * TX;

    return filtered;
}


void loop() {

    static float i = 0;

    float temp = getTemp();

    float x = TARGET_TEMP - temp;

    i += ((((float)DT_MS)/1000) * I) * x;
    if (i > 2.0f) i = 2.0f;
    if (i < -2.0f) i = -2.0f;

    float p = P * x;

    float y = p + i;

    if (y > 1) y = 1;
    if (y < 0) y = 0;

    int pwm = y * 255;
    analogWrite(PWMOUT, pwm);

    Serial.println("temp: " + ((String)temp) + " pwm: " + ((String)pwm) + " x: " + ((String)x) + " p: " + ((String)p) + " i: " + ((String)i));

    digitalWrite(LED, abs(x) < 3); //light up led if target reached

    //delay(DT_MS);

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
                if (buffer[0] == 't') {
                    Serial.print("new speed:"); 
                    Serial.println(val);
                    TARGET_TEMP = val;
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