#include <Arduino.h>

// Pins
#define PWMOUT 3
#define ADCIN A1

// Control sample period
#define DT_MS 100

#define TARGET_TEMP 200

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // analogReference(EXTERNAL);

  pinMode(PWMOUT, OUTPUT);
  TCCR2B = (TCCR2B & B11111000) | B00000111; // for PWM frequency of 31372.55 Hz

  analogWrite(PWMOUT, 0);
}

#define T0 (25.0f + 273.15f)
#define RT0 10000.0f   // Ω
#define B 3977.0f      // K
#define VCC 5.0f    //Supply voltage
#define R 10000.0f  //R=10KΩ

float getTemp(void) {

  static float filtered = 0;

  float VRT = analogRead(ADCIN); //Acquisition analog value of VRT

  // Serial.println(VRT);

  VRT = (5.00f / 1023.00f) * VRT; //Conversion to voltage
  float VR = VCC - VRT;
  float RT = VRT / (VR / R); //Resistance of RT

  float ln = log(RT / RT0);
  float TX = (1.0f / ((ln / B) + (1.0f / T0))); //Temperature from thermistor

  TX = TX - 273.15; //Conversion to Celsius

  if (filtered == 0) filtered = TX;

  filtered = 0.9 * filtered + 0.1 * TX;

  return filtered;
}

// PI Control parameters
#define P 0.0125
#define I 0.0025

void loop() {

  static float i = 0;

  float temp = getTemp();

  float x = TARGET_TEMP - temp;

  i += ((((float)DT_MS)/1000) * I) * x;
  if (i > 0.2) i = 0.2;
  if (i < -0.2) i = -0.2;

  float p = P * x;

  float y = p + i;

  if (y > 1) y = 1;
  if (y < 0) y = 0;

  int pwm = y * 255;
  analogWrite(PWMOUT, pwm);

  Serial.println("temp: " + ((String)temp) + " pwm: " + ((String)pwm) + " x: " + ((String)x) + " p: " + ((String)p) + " i: " + ((String)i));

  delay(DT_MS);
}