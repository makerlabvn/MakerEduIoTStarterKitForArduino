#define BLYNK_TEMPLATE_ID "TMPL63kCqkp4D"
#define BLYNK_TEMPLATE_NAME "IoT"
#define BLYNK_AUTH_TOKEN "xAW2hJhRm-zuK5qdZebxgGk_sscZ-1fY"

#define LED_PIN 11
#define POTEN_PIN A2

#define POT_MIN_VALUE 0
#define POT_MAX_VALUE 690

// Step 2: include library
#include "BlynkGate.h"
#include "LiquidCrystal_I2C.h"

// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MakerLab.vn";  // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "";             // Key in your wifi password.

unsigned long lastTimeSen = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
int percentValue = 0;
int pwmValue = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println(F("Start BlynkGate I2C"));

  lcd.init();
  lcd.backlight();

  // Step 4: begin BlynkGate
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  // DO NOT using delay
  // delay(100);
  controlLed();
  showOnLCD();

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

void controlLed() {
  int temp_pot = constrain(analogRead(POTEN_PIN), POT_MIN_VALUE, POT_MAX_VALUE);
  pwmValue = map(temp_pot, POT_MIN_VALUE, POT_MAX_VALUE, 0, 255);
  pwmValue = constrain(pwmValue, 0, 255);
  analogWrite(LED_PIN, pwmValue);
}


void showOnLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Basic 2");
  lcd.setCursor(0, 1);
  lcd.print("LED ON: ");
  lcd.setCursor(8, 1);
  percentValue = map(pwmValue, 0, 255, 0, 100);
  lcd.print(String(percentValue) + "%");
  lcd.print("    ");
}