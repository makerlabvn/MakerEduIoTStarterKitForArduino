// Step 1: Get this infomation from Blynk.cloud
#define BLYNK_TEMPLATE_ID "TMPL63kCqkp4D"
#define BLYNK_TEMPLATE_NAME "IoT"
#define BLYNK_AUTH_TOKEN "xAW2hJhRm-zuK5qdZebxgGk_sscZ-1fY"

// Step 2: include library
#include "BlynkGate.h"
#include "LiquidCrystal_I2C.h"
#include "OneButton.h"
// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MakerLab.vn";  // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "";             // Key in your wifi password.

#define BUTTON_PIN 9
#define BUZZER_PIN 10
#define LED_PIN 11
#define LDR_PIN A1
#define POTEN_PIN A2


LiquidCrystal_I2C lcd(0x27, 16, 2);
OneButton myButton(BUTTON_PIN, true, true);

unsigned long lastTimeSen = 0;

bool ledState = false;
int valuePotentiometer = 0;
int brightness = 0;
int brightnessPercent = 0;
int valueLDR = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("Start BlynkGate I2C"));
  pinMode(BUZZER_PIN, OUTPUT);
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
  // delay(100);
  myButton.tick();
  ledControl();
  readPotentiometer();
  readLDR();
  showOnLCD();
  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if (millis() - lastTimeSen >= 5000) {
    lastTimeSen = millis();
    Blynk.virtualWrite(1, brightnessPercent);
    Blynk.virtualWrite(2, valueLDR);
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
      ledState = true;
    } else {
      ledState = false;
    }
  }
}

void ledStateToggle() {
  ledState = !ledState;
  buzzer();
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
  brightness = map(valuePotentiometer, 0, 692, 0, 255);
  brightness = constrain(brightness, 0, 255);
}

void readLDR() {
  valueLDR = analogRead(LDR_PIN);
}

void showOnLCD() {
  lcd.setCursor(0, 0);
  lcd.print("LED: ");
  lcd.setCursor(5, 0);
  if (ledState) {
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
  }
  lcd.setCursor(12, 0);
  brightnessPercent = map(brightness, 0, 255, 0, 100);
  if (brightnessPercent < 10) {
    lcd.print("  " + (String)brightnessPercent + "%");
  } else if (brightnessPercent < 100) {
    lcd.print(" " + (String)brightnessPercent + "%");
  } else {
    lcd.print((String)brightnessPercent + "%");
  }
  lcd.setCursor(0, 1);
  lcd.print("LIGHT: ");
  lcd.setCursor(8, 1);
  int lightPercent = map(valueLDR, 760, 0, 0, 100);
  if (lightPercent > 50) {
    lcd.print("Bright");
  } else if (lightPercent < 50) {
    lcd.print("Dark");
  }
  lcd.print("      ");
}

void buzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(50);
  digitalWrite(BUZZER_PIN, LOW);
}
