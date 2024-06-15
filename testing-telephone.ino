#include <Bounce2.h>
#include <SD.h>



#define NUMBER_PIN 0  // Yellow
#define DIAL_PIN 1   // Red
#define HOOK_PIN 2   // Red
#define MAX_DIGITS 3


Bounce2::Button numberSwitch = Bounce2::Button();
Bounce2::Button dialSwitch = Bounce2::Button();
Bounce2::Button hookSwitch = Bounce2::Button();


char number[MAX_DIGITS + 1];
int currentDigit;
int pulseCount;

typedef enum { Idle,
               Dialling } stateType;
stateType state = Idle;


void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 5000) {
    // wait for serial port to connect.
  }
  Serial.println("Serial set up correctly");

  // Switch set up
  numberSwitch.attach(NUMBER_PIN, INPUT_PULLUP); 
  numberSwitch.interval(5);

  dialSwitch.attach(DIAL_PIN, INPUT_PULLUP);
  dialSwitch.interval(10);


  hookSwitch.attach(HOOK_PIN, INPUT_PULLUP);
  hookSwitch.interval(10);
}

void loop() {
  dialSwitch.update();
  numberSwitch.update();
  hookSwitch.update();

  if(hookSwitch.fell()) {
    Serial.println("Hook released");
  }

  switch (state) {
    case Idle:
      if (dialSwitch.fell()) {
        state = Dialling;
        Serial.println("Dialing!");
      }

      break;
    case Dialling:
      if (numberSwitch.rose()) {
        pulseCount++;
      }

      if (dialSwitch.rose()) {
        // Last digit is zero on the dialler
        if (pulseCount > 10) {
          pulseCount = 0;
        } else {
          pulseCount--;
        }

        Serial.println(pulseCount);
        pulseCount = 0;
        state = Idle;
      }
      break;
  }
}
