/*
  Title:  Demo 1
  Author: Thanh Tam
  Date: 5/6/2024
  Description: Using button and potentiometer to control led and show on LCD
              - Virtual Pin V0 (Switch) on Blynk for button
              - Virtual Pin V1 (Gauge)  on Blynk for Potentiometer
*/
// Step 1: Get this infomation from Blynk.cloud
#define BLYNK_TEMPLATE_ID "Copy_BLYNK_TEMPLATE_ID_From_BlynkCloud"
#define BLYNK_TEMPLATE_NAME "Copy_BLYNK_TEMPLATE_NAME_From_BlynkCloud"
#define BLYNK_AUTH_TOKEN "Copy_BLYNK_AUTH_TOKEN_From_BlynkCloud"

// Step 2: include library
#include "BlynkGate.h"
#include "LiquidCrystal_I2C.h"
#include "OneButton.h"
// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MakerLab.vn";  // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "";             // Key in your wifi password.

#define POTEN_MAX_VALUE 692
#define POTEN_MIN_VALUE 0

#define POTEN_PIN A2
#define BUTTON_PIN A3
#define LED_PIN 11

unsigned long intervalLCD = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);

OneButton myButton(BUTTON_PIN, true, true);

unsigned long lastTimeSen = 0;

int ledState = 0;

int valuePotentiometer = 0;
int brightness = 0;
int brightnessPercent = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Start BlynkGate I2C"));
  myButton.attachClick(ledStateToggle);
  lcd.init();
  lcd.backlight();
  // Step 4: begin BlynkGate
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  // DO NOT using delay
  myButton.tick();
  ledControl();
  readPotentiometer();
  if (millis() - intervalLCD > 500) {
    showOnLCD();
    intervalLCD = millis();
  }
  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if (millis() - lastTimeSen >= 10000) {
    lastTimeSen = millis();
    Blynk.virtualWrite(1, brightnessPercent);
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
  if (request.pin == 0) {
    if (myInt == 1) {
      ledState = 1;
    } else {
      ledState = 0;
    }
  }
}

void ledStateToggle() {
  ledState = !ledState;
  Blynk.virtualWrite(0, ledState);
}

void ledControl() {
  if (ledState) {
    analogWrite(LED_PIN, brightness);
  } else {
    analogWrite(LED_PIN, 0);
  }
}

void readPotentiometer() {
  valuePotentiometer = analogRead(POTEN_PIN);
  brightness = map(valuePotentiometer, POTEN_MIN_VALUE, POTEN_MAX_VALUE, 0, 255);
  brightness = constrain(brightness, 0, 255);
  brightnessPercent = map(brightness, 0, 255, 0, 100);
}

void showOnLCD() {
  lcd.setCursor(1, 0);
  lcd.print("IoT StarterKit");
  lcd.setCursor(0, 1);
  lcd.print("LED: ");
  lcd.setCursor(5, 1);
  if (ledState) {
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
  }
  lcd.setCursor(12, 1);
  if (brightnessPercent < 10) {
    lcd.print("  ");
  } else if (brightnessPercent < 100) {
    lcd.print(" ");
  }
  lcd.print(brightnessPercent);
  lcd.print("%");
}