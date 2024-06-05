/*
  Title:  Basic Demo 2
  Author: Daniel Hoang
  Date:   5/6/2024
  Description: Using button(Virtual Pin V0 Switch) to turn on or turn off the led and using potentiometer(Virtual Pin V1 Gauge) to control brightness of the led, 
               show on LCD and write value on Blynk every 5s.
*/
#define BLYNK_TEMPLATE_ID "TMPL63kCqkp4D"
#define BLYNK_TEMPLATE_NAME "IoT"
#define BLYNK_AUTH_TOKEN "xAW2hJhRm-zuK5qdZebxgGk_sscZ-1fY"

#define LED_PIN 11
#define POTEN_PIN A2
#define BUTTON_PIN 9

#define POT_MIN_VALUE 0
#define POT_MAX_VALUE 690

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
int percentValue = 0;
int pwmValue = 0;
bool ledState = 0;
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
  readPotentiometer();
  showOnLCD();

  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if (millis() - lastTimeSen >= 5000) {
    lastTimeSen = millis();
    Blynk.virtualWrite(1, percentValue);
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

void updateLedState() {
  if (ledState) {
    analogWrite(LED_PIN, pwmValue);
  } else {
    analogWrite(LED_PIN, 0);
  }
}

void readPotentiometer() {
  int temp_pot = constrain(analogRead(POTEN_PIN), POT_MIN_VALUE, POT_MAX_VALUE);
  pwmValue = map(temp_pot, POT_MIN_VALUE, POT_MAX_VALUE, 0, 255);
  pwmValue = constrain(pwmValue, 0, 255);
}


void showOnLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Basic 2");
  lcd.setCursor(0, 1);
  lcd.print("LED: ");
  if (ledState == 1) {
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
  }
  lcd.setCursor(11, 1);
  percentValue = map(pwmValue, 0, 255, 0, 100);
  if (percentValue < 10) {
    lcd.print("  " + (String)percentValue + "%");
  } else if (percentValue < 100) {
    lcd.print(" " + (String)percentValue + "%");
  } else {
    lcd.print((String)percentValue + "%");
  }
  lcd.print("    ");
}