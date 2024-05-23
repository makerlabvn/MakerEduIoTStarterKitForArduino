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
#define BLYNK_TEMPLATE_ID "TMPL6gewHRhwN"
#define BLYNK_TEMPLATE_NAME "hbqBlynkGateTest"
#define BLYNK_AUTH_TOKEN "ZXhT-EXFQTDKIhNx314feu6MHDbvSiHL"

#define PUSH_BUTTON_PIN A2
#define LED_PIN         9

// Step 2: include library
#include "BlynkGate.h"
#include "LiquidCrystal_I2C.h"
#include "OneButton.h"

// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MakerLab.vn"; // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "";        // Key in your wifi password.

unsigned long lastTimeSen = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneButton myButton(A2, true, true);

bool ledState = 0;

void setup()
{
  // put your setup code here, to run once:
  pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println(F("Start BlynkGate I2C"));

  lcd.init();
  lcd.backlight();

  myButton.attachClick(turnLedOnOff);

  // Step 4: begin BlynkGate
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  // put your main code here, to run repeatedly:
  Blynk.run();
  // DO NOT using delay
  // delay(100);
  myButton.tick();
  showDataOnLCDAndControlLED();

  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if (millis() - lastTimeSen >= 1000)
  {
    lastTimeSen = millis();
    // Step 6: Send Virtual pin Value
  }
}

// Step 5: Get Virtual pin Value
BLYNK_WRITE_DEFAULT()
{
  // double myDouble = param.asFloat();
  // String myString = param.asString();
  int myInt = param.asInt();

  Serial.print("input V");
  Serial.print(request.pin);
  Serial.print(": ");
  Serial.println(myInt);

  if(request.pin == 0){
    if(myInt == 1){
      ledState = 1;
    }
    else{
      ledState = 0;
    }
  }
}

void turnLedOnOff()
{
  ledState = !ledState;
  Blynk.virtualWrite(0, ledState);
}

void showDataOnLCDAndControlLED()
{
  lcd.setCursor(0, 0);
  lcd.print("LED STATUS: ");

  if (ledState == 1)
  {
    digitalWrite(LED_PIN, ledState);
    lcd.setCursor(12, 0);
    lcd.print("ON ");
    lcd.print("   ");
    
  }
  else
  {
    digitalWrite(LED_PIN, ledState);
    lcd.setCursor(12, 0);
    lcd.print("OFF");
    lcd.print("   ");
    
  }
}
