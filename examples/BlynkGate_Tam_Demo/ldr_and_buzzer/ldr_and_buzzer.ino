/**
 * Title: Control LED using Blynk Switch and Push Button
 *
 * Author: Daniel
 *
 *
 *
 *
 *
 */

// Step 1: Get this infomation from Blynk.cloud
#define BLYNK_TEMPLATE_ID "TMPL63kCqkp4D"
#define BLYNK_TEMPLATE_NAME "IoT"
#define BLYNK_AUTH_TOKEN "xAW2hJhRm-zuK5qdZebxgGk_sscZ-1fY"

#define LDR_PIN A1
#define LED_PIN 9
#define BUZZER_PIN 10
#define LDR_MIN_VALUE 0
#define LDR_MAX_VALUE 730

// Step 2: include library
#include "BlynkGate.h"
#include "LiquidCrystal_I2C.h"

// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MakerLab.vn";  // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "";             // Key in your wifi password.

LiquidCrystal_I2C lcd(0x27, 16, 2);

int percentValue = 0;
bool doubleCickSound_flag = 0;
bool dayState = 0;
bool lastDayState = 0;
unsigned long lastTimeSen = 0;

void makerDelay(unsigned long pa_time) {
  unsigned long makerMillis = millis() + pa_time;
  while (makerMillis >= millis()) {
    ;
    ;
  }
}

void doubleClickSound() {
  doubleCickSound_flag = 1;
  if (doubleCickSound_flag == 1) {
    digitalWrite(BUZZER_PIN, HIGH);
    makerDelay(75);
    digitalWrite(BUZZER_PIN, LOW);
    makerDelay(75);

    digitalWrite(BUZZER_PIN, HIGH);
    makerDelay(75);
    digitalWrite(BUZZER_PIN, LOW);
    makerDelay(75);

    doubleCickSound_flag = 0;
  } else {
    return;
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

  lcd.init();
  lcd.backlight();

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
  showDataOnLCD();
  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if (millis() - lastTimeSen >= 1000) {
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
}

void controlState() {
  int temp_ldr = analogRead(LDR_PIN);
  int lightPercent = map(temp_ldr, LDR_MIN_VALUE, LDR_MIN_VALUE, 0, 100);
  if (lightPercent >= 50) {
    dayState = 1;
    digitalWrite(LED_PIN, HIGH);
  } else {
    dayState = 0;
    digitalWrite(LED_PIN, LOW);
  }
}
void showDataOnLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Basic 3");
  lcd.setCursor(0, 1);
  lcd.print("Room is: ");
  lcd.setCursor(9, 1);
  if (dayState) {
    lcd.print("Bright");
  } else {
    lcd.print("Dark");
  }
  lcd.print("      ");
}