/*
  Title:  LDR and Buzzer demo
  Author: Daniel Hoang
  Date:   5/6/2024
  Description: Read value of the LDR sensor and write value into Blynk every 5s.
              - Virtual Pin V2 (Chart) on Blynk for LDR sensor
*/
// Step 1: Get this infomation from Blynk.cloud
#define BLYNK_TEMPLATE_ID "Copy_BLYNK_TEMPLATE_ID_From_BlynkCloud"
#define BLYNK_TEMPLATE_NAME "Copy_BLYNK_TEMPLATE_NAME_From_BlynkCloud"
#define BLYNK_AUTH_TOKEN "Copy_BLYNK_AUTH_TOKEN_From_BlynkCloud"

#define LDR_PIN A1
#define LED_PIN 11
#define BUZZER_PIN 10

#define LDR_MIN_VALUE 0
#define LDR_MAX_VALUE 692

#define SETPOINT  60
#define OFFSET    5

// Step 2: include library
#include "BlynkGate.h"

// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MakerLab.vn";  // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "";             // Key in your wifi password.

bool dayState = 0;
bool lastDayState = 0;

int temp_ldr = 0;
int percentValue = 0;

unsigned long lastTimeSen = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println(F("Start BlynkGate I2C"));
  // Step 4: begin BlynkGate
  Blynk.begin(auth, ssid, pass);

  lastDayState = dayState;
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  // DO NOT using delay
  checkDayState();
  controlState();
  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if (millis() - lastTimeSen >= 10000) {
    lastTimeSen = millis();
    Blynk.virtualWrite(2, percentValue);
    // Step 6: Send Virtual pin Value
  }
}

// Step 5: Get Virtual pin Value
BLYNK_WRITE_DEFAULT() {
  // double myDouble = param.asFloat();
  // String myString = param.asString();
  int myInt = param.asInt();

  Serial.print("input V");
  Serial.print(request.pin);
  Serial.print(": ");
  Serial.println(myInt);
}

void doubleClickSound() {
    // careful with function delay(), which makes your program run slowly
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);

    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
}

void checkDayState() {
  if (lastDayState != dayState) {
    doubleClickSound();
    lastDayState = dayState;
  }
}

void controlState() {
  temp_ldr = constrain(analogRead(LDR_PIN), LDR_MIN_VALUE, LDR_MAX_VALUE);
  percentValue = map(temp_ldr, LDR_MAX_VALUE, LDR_MIN_VALUE, 0, 100);
  if (percentValue > (SETPOINT + OFFSET)) {
    dayState = 1;
    digitalWrite(LED_PIN, HIGH);
  } else if(percentValue < (SETPOINT - OFFSET)) {
    dayState = 0;
    digitalWrite(LED_PIN, LOW);
  }
}