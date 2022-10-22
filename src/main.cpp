// COMP-10184 - Mohawk College
// Alarm System
// This program uses an IR sensor and button to emulate a alarm system
// @author Nick Vella
// @id 000826457
// I created this work and have not shared it with anyone else.

#include <Arduino.h>

// digital pin for the pir sensor
#define PIN_PIR D5
// digital pin for the button
#define PIN_BUTTON D6

// alarm is disabled, press button to re-enable
#define ALARM_DISABLED 0
// alarm is going off, police are coming
#define ALARM_ENABLE 1
// alarm is counting down, press button to disable
#define ALARM_COUNTDOWN 2
// alarm is waiting to sense bad guys
#define ALARM_ACTIVE 3
// delay used for the flashing light on and off
#define FLASH_DELAY 125
// delay to help ensure the button press isnt a double press
#define BUTTON_DELAY 200

// the current state of the alarm
int iAlarmState = ALARM_ACTIVE;
// timer for the alarm to go off in 0.25 of a second.
int iAlarmTimer = 40;


// Checks inputs and selects the correct state based on where it is right now
void collectInputs()
{
  bool bPIR;
  bPIR = digitalRead(PIN_PIR);
  if (bPIR && iAlarmState != ALARM_DISABLED && iAlarmState != ALARM_ENABLE)
  {
    iAlarmState = ALARM_COUNTDOWN;
  }

  bool bButtonPressed = digitalRead(PIN_BUTTON);
  if (!bButtonPressed && iAlarmState == ALARM_COUNTDOWN)
  {
    delay(BUTTON_DELAY);
    Serial.write("Huh");
    iAlarmTimer = 40;
    iAlarmState = ALARM_DISABLED;
  }
  else if (!bButtonPressed && iAlarmState == ALARM_DISABLED)
  {
    delay(BUTTON_DELAY);
    iAlarmState = ALARM_ACTIVE;
  }
}
//Sets the correct states for the alarm and the matching light
void checkAlarmState()
{
  if (iAlarmState == ALARM_DISABLED || iAlarmState == ALARM_ACTIVE)
  {
    digitalWrite(LED_BUILTIN, true);
  }
  if (iAlarmState == ALARM_ENABLE)
  {
    digitalWrite(LED_BUILTIN, false);
  }
  if (iAlarmState == ALARM_COUNTDOWN)
  {
    if (iAlarmTimer > 0)
    {
      delay(FLASH_DELAY);
      digitalWrite(LED_BUILTIN, false);
      delay(FLASH_DELAY);
      digitalWrite(LED_BUILTIN, true);
      iAlarmTimer--;
    }
    else
    {
      iAlarmState = ALARM_ENABLE;
    }
  }
}

// *************************************************************
void setup()
{
  // configure the USB serial monitor
  Serial.begin(115200);

  // configure the LED output
  pinMode(LED_BUILTIN, OUTPUT);

  // PIR sensor is an INPUT
  pinMode(PIN_PIR, INPUT);

  // Button is an INPUT
  pinMode(PIN_BUTTON, INPUT_PULLUP);
}

// *************************************************************
void loop()
{
  collectInputs();
  checkAlarmState();
}