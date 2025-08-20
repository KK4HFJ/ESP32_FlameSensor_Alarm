#define DO_PIN 23  // ESP32's pin GPIO23 connected to DO pin of the flame sensor
#define ALARM 22

#include "pitches.h"

// notes in the melody:
int melody[] = {NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {2, 2, 2, 2, 2, 2};

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int AO_PIN = 34;

// variable for storing the potentiometer value
int AO_Value = 0;

void setup() {
  // initialize serial communication
  Serial.begin(115200);
  // initialize the ESP32's pin as an input
  pinMode(DO_PIN, INPUT);
  pinMode(ALARM, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  int flame_state = digitalRead(DO_PIN);

  if (flame_state == LOW){
    Serial.println("No flame dected => The fire is NOT detected");
    digitalWrite(LED_BUILTIN, LOW);
    
  }else{
    Serial.println("Flame dected => The fire is detected");
    digitalWrite(LED_BUILTIN, HIGH);
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(ALARM, melody[thisNote], noteDuration);
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(ALARM);
    }
  }
  AO_Value = analogRead(AO_PIN);
  Serial.println(AO_Value);
  
}
