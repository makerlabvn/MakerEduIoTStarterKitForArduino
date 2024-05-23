// Step 1: Get this infomation from Blynk.cloud
#define BLYNK_TEMPLATE_ID "TMPL6gewHRhwN"
#define BLYNK_TEMPLATE_NAME "hbqBlynkGateTest"
#define BLYNK_AUTH_TOKEN "ZXhT-EXFQTDKIhNx314feu6MHDbvSiHL"

#define LDR_PIN A1
#define LED_PIN 9
#define BUZZER_PIN 10
#define POT_MAX 730
#define POT_MIN 0
#define NUMBER_OF_MODE 2
#define TIME_BETWEEN_CLICK 300

// Step 2: include library
#include "BlynkGate.h"
#include "LiquidCrystal_I2C.h"
#include "OneButton.h"

// Step 3: Setup WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "HshopLTK";      // Key in your wifi name (Bandwidth 2.4Ghz). You can check with your smart phone for your wifi name
char pass[] = "HshopLTK@2311"; // Key in your wifi password.
unsigned long lastTimeSen = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
uint16_t value = 0;
unsigned long lastTimeCheck = 0;
bool buttonState = 1;
OneButton myButton(A2, true, true);
bool ledState = 0, button = 0, lastLedState = 0;
uint8_t trueValue = 0;
uint8_t mode = 1;
bool doubleCickSound_flag = 0;

bool pwmControll_flag = 0;
uint8_t pwmValueForAll = 0;

uint8_t pwmValue = 0;

void makerDelay(unsigned long pa_time)
{
  unsigned long makerMillis = millis() + pa_time;
  while (makerMillis >= millis())
  {
    ;
    ;
  }
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(LDR_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println(F("Start BlynkGate I2C"));

  lcd.init();
  lcd.backlight();

  // Step 4: begin BlynkGate
  Blynk.begin(auth, ssid, pass);

  myButton.setClickMs(TIME_BETWEEN_CLICK);
  myButton.attachClick(changeMode);
  lastLedState = ledState;
}

void loop()
{
  // put your main code here, to run repeatedly:
  Blynk.run();
  // DO NOT using delay
  // delay(100);
  myButton.tick();

  if (mode == 1)
  {
    showDataOnLCDMode1();
    controlLed();
  }
  else if (mode == 2)
  {
    showDataOnLCDMode2();
  }

  // Serial.println(trueValue);
  // Try using millis() and use "Blynk.virtualWrite" at least 10s at a time to avoid spamming the server
  if (millis() - lastTimeSen >= 2000)
  {
    lastTimeSen = millis();
    
    // Step 6: Send Virtual pin Value
  }

  if (millis() - lastTimeCheck >= 50)
  {
    lastTimeCheck = millis();

    if (mode == 1)
    {
      myButton.attachClick(changeMode);
      myButton.attachDoubleClick(functionOfMode1);
    }
    else if (mode == 2)
    {
      myButton.attachClick(changeMode);
      myButton.attachDoubleClick(functionOfMode2);
    }
    else if (mode == 3)
    {
      myButton.attachClick(changeMode);
      myButton.attachDoubleClick(functionOfMode3);
    }
  }
}

void changeMode()
{
  lcd.clear();
  digitalWrite(BUZZER_PIN, HIGH);
  mode++;
  if (mode > NUMBER_OF_MODE)
  {
    mode = 1;
  }
  makerDelay(100);
  digitalWrite(BUZZER_PIN, LOW);
}

void functionOfMode1()
{
  doubleClickSound();
  ledState = !ledState;
  Blynk.virtualWrite(2, ledState);
}

void showDataOnLCDMode1()
{
  uint16_t trueAnalogValue = constrain(analogRead(A3), POT_MIN, POT_MAX);

  uint8_t pwmValue = map(trueAnalogValue, POT_MIN, POT_MAX, 0, 245);

  uint8_t percentValue = map(pwmValue, 0, 245, 0, 100);

  uint8_t truePercentValue = constrain(percentValue, 0, 100);

  lcd.setCursor(0, 0);
  lcd.print("Mode 1: Manual");
  lcd.setCursor(0, 1);
  lcd.print("LED: ");

  if (ledState == 1)
  {
    lcd.setCursor(5, 1);
    lcd.print("ON ");
    lcd.setCursor(8, 1);
    lcd.print("(" + String(truePercentValue) + "%" + ")");
    lcd.print("           ");
  }
  else
  {
    lcd.setCursor(5, 1);
    lcd.print("OFF");
    lcd.print("           ");
  }
}

void controlLed()
{
  uint16_t analogValue1 = constrain(analogRead(A3), POT_MIN, POT_MAX);

  uint8_t pwmValue1 = map(analogValue1, POT_MIN, POT_MAX, 0, 255);

  uint8_t truePwmValue = constrain(pwmValue1, 0, 255);

  if (ledState == 1)
  {
    analogWrite(LED_PIN, truePwmValue);
  }
  else
  {

    analogWrite(LED_PIN, 0);
  }
}

void functionOfMode2()
{
  // if(){

  // }
}

void functionOfMode3()
{
}

void showDataOnLCDMode2()
{
  lcd.setCursor(0, 0);
  lcd.print("Mode 2: Auto");
  lcd.setCursor(0, 1);
  lcd.print("Room is: ");
  uint16_t temp_ldr = analogRead(LDR_PIN);
  uint8_t ldr_to_light_percent = map(temp_ldr, 750, 0, 0, 100);
  if (ldr_to_light_percent > 50)
  {
    ledState = 0;
    digitalWrite(LED_PIN, ledState);
    lcd.setCursor(9, 1);
    lcd.print("Bright");
    lcd.print("      ");
  }
  else if (ldr_to_light_percent < 50)
  {
    ledState = 1;
    digitalWrite(LED_PIN, ledState);
    lcd.setCursor(9, 1);
    lcd.print("Dark");
    lcd.print("      ");
  }
}

void doubleClickSound()
{
  doubleCickSound_flag = 1;
  if (doubleCickSound_flag == 1)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    makerDelay(75);
    digitalWrite(BUZZER_PIN, LOW);
    makerDelay(75);

    digitalWrite(BUZZER_PIN, HIGH);
    makerDelay(75);
    digitalWrite(BUZZER_PIN, LOW);
    makerDelay(75);

    doubleCickSound_flag = 0;
  }
  else
  {
    return;
  }
}

void dimmingLEDControll()
{
  if (pwmControll_flag == 0)
  {
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
