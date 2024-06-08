/*
  Title:  Button, Led and LCD demo
  Author: Daniel Hoang
  Date:   5/6/2024
  Description: Using button to turn on or turn off the led and show on LCD
              - Virtual Pin V0 (Switch) on Blynk for button
*/
#define BLYNK_TEMPLATE_ID "TMPL63kCqkp4D"
#define BLYNK_TEMPLATE_NAME "IoT"
#define BLYNK_AUTH_TOKEN "xAW2hJhRm-zuK5qdZebxgGk_sscZ-1fY"

#define BUTTON_PIN 9
#define LED_PIN 11

// Step 2: include library
#include "BlynkGate.h"
#include "LiquidCrystal_I2C.h"
#include "OneButton.h"

// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MakerLab.vn";  // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "";             // Key in your wifi password.

unsigned long lastTimeSen = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneButton myButton(BUTTON_PIN, true, true);

int ledState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println(F("Start BlynkGate I2C"));

  lcd.init();
  lcd.backlight();

  myButton.attachClick(ledToggle);

  // Step 4: begin BlynkGate
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  // DO NOT using delay
  // delay(100);
  myButton.tick();
  updateLedState();
  showOnLCD();
  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if (millis() - lastTimeSen >= 1000) {
    lastTimeSen = millis();
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

void ledToggle() {
  ledState = !ledState;
  Blynk.virtualWrite(0, ledState);
}

void updateLedState(){
  digitalWrite(LED_PIN, ledState);
}

void showOnLCD() {
  lcd.setCursor(1, 0);
  lcd.print("IoT StarterKit");
  lcd.setCursor(0, 1);
  lcd.print("LED STATUS: ");
  lcd.setCursor(12, 1);
  if (ledState == 1) {
    lcd.print("ON ");

  } else {
    lcd.print("OFF");
  }
  lcd.print("   ");
}