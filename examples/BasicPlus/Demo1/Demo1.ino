// Step 1: Get this infomation from Blynk.cloud
#define BLYNK_TEMPLATE_ID "TMPL6gewHRhwN"
#define BLYNK_TEMPLATE_NAME "hbqBlynkGateTest"
#define BLYNK_AUTH_TOKEN "ZXhT-EXFQTDKIhNx314feu6MHDbvSiHL"

#define BTN_PIN A1
#define LED_PIN 10
#define POT_MAX 704
#define POT_MIN 0

// Step 2: include library
#include "BlynkGate.h"
#include "LiquidCrystal_I2C.h"
#include "OneButton.h"

// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "HshopLTK";      // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "HshopLTK@2311"; // Key in your wifi password.
unsigned long lastTimeSen = 0;

// Step 4: Declare lcd and some variables

LiquidCrystal_I2C lcd(0x27, 16, 2);
uint16_t value = 0;
unsigned long lastTimeClear = 0;
OneButton myButton(A2, true, true);
bool ledState = 0, button = 0, lastLedState = 0;
uint8_t trueValue = 0;

void setup()
{
  // put your setup code here, to run once:
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println(F("Start BlynkGate I2C"));

  lcd.init();
  lcd.backlight();

  // Step 4: begin BlynkGate
  Blynk.begin(auth, ssid, pass);
  myButton.attachClick(turnLedOnOff);

  lastLedState = ledState;
}

void loop()
{
  // put your main code here, to run repeatedly:
  Blynk.run();
  // DO NOT using delay
  // delay(100);
  myButton.tick();

  showDataOnLCD();
  controlLed();
  // Serial.println(trueValue);
  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if (millis() - lastTimeSen >= 2000)
  {
    lastTimeSen = millis();
    // Step 6: Send Virtual pin Value
  }

  if (millis() - lastTimeClear >= 600)
  {
    lastTimeClear = millis();
    // lcd.clear();
  }
}

void turnLedOnOff()
{
  ledState = !ledState;
  Blynk.virtualWrite(2, ledState);
}

void showDataOnLCD()
{
  uint16_t analogValue = analogRead(A3);
  uint8_t pwmValue = map(analogValue, POT_MIN, POT_MAX, 0, 255);
  uint8_t percentValue = map(pwmValue, 0, 255, 0, 100);

  lcd.setCursor(0, 0);
  lcd.print("LED STATUS: ");
  lcd.setCursor(0, 1);
  lcd.print("LED ON: ");

  if (ledState == 1)
  {
    lcd.setCursor(12, 0);
    lcd.print("ON ");
    lcd.print("   ");
    
  }
  else
  {
    lcd.setCursor(12, 0);
    lcd.print("OFF");
    lcd.print("   ");
    
  }

  lcd.setCursor(12, 1);
  lcd.print(String(percentValue) + "%");
  lcd.print("    ");
}

void controlLed()
{
  uint16_t analogValue = analogRead(A3);
  uint8_t pwmValue = map(analogValue, POT_MIN, POT_MAX, 0, 255);

  if (ledState == 1)
  {
    analogWrite(LED_PIN, pwmValue);
  }
  else
  {

    analogWrite(LED_PIN, 0);
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

  if (request.pin == 2)
  {
    if (myInt == 1)
    {
      ledState = 1;
    }
    else
    {
      ledState = 0;
    }
  }
}
