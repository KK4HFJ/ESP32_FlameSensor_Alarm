/*
  ESP32 Flame Sensor Alarm
  ------------------------
  IR flame sensor module -> ESP32 -> passive piezo buzzer + on-board LED.

  Wiring (both modules powered from 3.3 V):
    Flame sensor DO -> GPIO23
    Flame sensor AO -> GPIO34 (ADC1_CH6, input-only pin)
    Buzzer signal   -> GPIO22 (passive buzzer, driven with tone())
*/

#include "pitches.h"

// ---- Configuration ---------------------------------------------------------

constexpr uint8_t  FLAME_DO_PIN     = 23;
constexpr uint8_t  FLAME_AO_PIN     = 34;
constexpr uint8_t  BUZZER_PIN       = 22;

// Level DO reads when the sensor sees flame. Flip to LOW if your module's
// DO output is active-low.
constexpr int      FLAME_DETECTED   = HIGH;

constexpr uint32_t SERIAL_BAUD      = 115200;
constexpr uint32_t ANALOG_REPORT_MS = 500;  // how often to log the AO reading

// Alarm pattern: six half notes on E4.
const int ALARM_MELODY[]     = {NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4};
const int ALARM_NOTE_TYPES[] = {2, 2, 2, 2, 2, 2};  // 2 = half, 4 = quarter, 8 = eighth

constexpr size_t ALARM_NOTES = sizeof(ALARM_MELODY) / sizeof(ALARM_MELODY[0]);
static_assert(sizeof(ALARM_NOTE_TYPES) / sizeof(ALARM_NOTE_TYPES[0]) == ALARM_NOTES,
              "ALARM_MELODY and ALARM_NOTE_TYPES must be the same length");

// ---- Alarm -----------------------------------------------------------------

// Plays the melody once (blocking, about 3.9 s).
void soundAlarm() {
  for (size_t i = 0; i < ALARM_NOTES; i++) {
    const int noteMs = 1000 / ALARM_NOTE_TYPES[i];
    tone(BUZZER_PIN, ALARM_MELODY[i], noteMs);
    delay(noteMs * 13 / 10);  // note length + 30 % gap so repeated notes stay distinct
    noTone(BUZZER_PIN);
  }
}

// ---- Arduino entry points --------------------------------------------------

void setup() {
  Serial.begin(SERIAL_BAUD);
  pinMode(FLAME_DO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  static int8_t   lastState    = -1;  // -1 = nothing reported yet
  static uint32_t lastReportMs = 0;

  const bool flame = digitalRead(FLAME_DO_PIN) == FLAME_DETECTED;
  digitalWrite(LED_BUILTIN, flame ? HIGH : LOW);

  // Log state changes only, not every pass.
  if (static_cast<int8_t>(flame) != lastState) {
    Serial.println(flame ? "Flame detected" : "No flame");
    lastState = flame;
  }

  if (flame) {
    soundAlarm();
  }

  if (millis() - lastReportMs >= ANALOG_REPORT_MS) {
    lastReportMs = millis();
    Serial.print("AO:");
    Serial.println(analogRead(FLAME_AO_PIN));
  }
}
