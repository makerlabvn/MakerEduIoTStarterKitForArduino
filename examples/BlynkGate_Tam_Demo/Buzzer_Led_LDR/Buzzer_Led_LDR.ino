/*
  Title:  LDR and Buzzer demo
  Author: Daniel Hoang
  Date:   5/6/2024
  Description: Read value of the LDR sensor and write value into Blynk every 5s.
              - Virtual Pin V2 (Chart) on Blynk for LDR sensor
*/
// Step 1: Get this infomation from Blynk.cloud
#define BLYNK_TEMPLATE_ID "TMPL63kCqkp4D"
#define BLYNK_TEMPLATE_NAME "IoT"
#define BLYNK_AUTH_TOKEN "xAW2hJhRm-zuK5qdZebxgGk_sscZ-1fY"

#define LDR_PIN A1
#define LED_PIN 11
#define BUZZER_PIN 10

#define LDR_MIN_VALUE 0
#define LDR_MAX_VALUE 690

// Step 2: include library
#include "BlynkGate.h"

// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MakerLab.vn";  // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "";             // Key in your wifi password.

int percentValue = 0;
bool doubleCickSound_flag = 0;
bool dayState = 0;
bool lastDayState = 0;
unsigned long lastTimeSen = 0;
int temp_ldr = 0;

void doubleClickSound() {
  doubleCickSound_flag = 1;
  if (doubleCickSound_flag == 1) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);

    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);

    doubleCickSound_flag = 0;
  } 
}

void checkDayState() {
  if (lastDayState != dayState) {
    doubleClickSound();
    lastDayState = dayState;
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println(F("Start BlynkGate I2C"));
  // Step 4: begin BlynkGate
  Blynk.begin(auth, ssid, pass);

  lastDayState = dayState;
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  // DO NOT using delay
  // delay(100);
  checkDayState();
  controlState();
  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if (millis() - lastTimeSen >= 5000) {
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

void controlState() {
  temp_ldr = constrain(analogRead(LDR_PIN), LDR_MIN_VALUE, LDR_MAX_VALUE);
  percentValue = map(temp_ldr, LDR_MAX_VALUE, LDR_MIN_VALUE, 0, 100);
  if (percentValue >= 60) {
    dayState = 1;
    digitalWrite(LED_PIN, HIGH);
  } else {
    dayState = 0;
    digitalWrite(LED_PIN, LOW);
  }
}