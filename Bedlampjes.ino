#include <Encoder.h>
#include <TimerOne.h>

const uint32_t DEBOUNCE_INTERVAL  = 50;
const uint32_t OTHER_TIMEOUT      = 5000;

const uint8_t LAMP_LINKS          = 9;
const uint8_t LAMP_RECHTS         = 10;
const uint8_t LAMP_ONDER          = 3;

const uint8_t LINKS_A             = 11;
const uint8_t LINKS_B             = 12;
const uint8_t LINKS_S             = 13;
const uint8_t RECHTS_A            = A1;
const uint8_t RECHTS_B            = A2;
const uint8_t RECHTS_S            = A3;

const uint8_t MOTIONSENSOR1       = 6;
const uint8_t MOTIONSENSOR2       = 5;
const uint32_t MOTION_ACTIVE      = 60 * 1000ul;            // How long the light has to remain on upon having been triggered by the motion sensor.
const uint32_t MOTION_BRIGHTNESS  = 32;                     // Fade up to this level of logStripBrightness (32 = PWM level 50).
const uint32_t FADE_SPEED         = 3 * 1000ul;             // Time in milliseconds to reach MOTION_BRIGHTNESS.
const uint32_t FADE_STEP_SPEED    = FADE_SPEED / (MOTION_BRIGHTNESS - 4);


// Bedlampjes halen maximale helderheid bij een PWM waarde van ~80!
//const uint8_t BRIGHTNESS_VALUES = 72;                       // Encoder makes four steps at a time... This is 3/4 turn for minimum to maximum brightness, 18 steps.
//const uint8_t logStripBrightness[BRIGHTNESS_VALUES] = {0, 0, 0, 1, 1, 2, 2, 3,
//                                                       4, 5, 6, 7, 8, 9, 11, 12,
//                                                       14, 16, 18, 20, 21, 24, 26, 28,
//                                                       30, 33, 35, 38, 41, 44, 47, 50,
//                                                       54, 56, 60, 63, 67, 70, 75, 79,
//                                                       82, 87, 91, 95, 99, 104, 108, 113,
//                                                       117, 123, 128, 133, 139, 143, 149, 154,
//                                                       160, 165, 171, 178, 183, 190, 195, 202,
//                                                       207, 215, 220, 228, 233, 241, 249, 255
//                                                      };
//
//const uint8_t logBulbBrightness[BRIGHTNESS_VALUES] = {0, 0, 0, 0, 1, 1, 1, 1,
//                                                      2, 2, 2, 2, 3, 3, 4, 4,
//                                                      5, 5, 6, 6, 7, 8, 8, 9,
//                                                      10, 11, 11, 12, 13, 14, 15, 16,
//                                                      17, 18, 19, 20, 21, 23, 24, 25,
//                                                      26, 28, 29, 30, 32, 33, 35, 36,
//                                                      38, 39, 41, 42, 44, 46, 47, 49,
//                                                      51, 53, 55, 56, 58, 60, 62, 64,
//                                                      66, 68, 70, 73, 75, 77, 79, 81
//                                                     };

const uint8_t BRIGHTNESS_VALUES = 96;                       // Encoder makes four steps at a time... This is 1 turn for minimum to maximum brightness, 24 steps.
const uint8_t logStripBrightness[BRIGHTNESS_VALUES] = {0, 0, 0, 0, 1, 1, 1, 2,
                                                       2, 3, 3, 4, 5, 5, 6, 7,
                                                       8, 9, 10, 11, 12, 13, 15, 16,
                                                       17, 19, 20, 21, 23, 24, 26, 28,
                                                       30, 32, 34, 35, 38, 40, 42, 44,
                                                       47, 48, 51, 54, 56, 58, 61, 63,
                                                       66, 69, 71, 75, 77, 80, 84, 86,
                                                       89, 93, 95, 99, 103, 105, 109, 113,
                                                       116, 120, 124, 127, 131, 136, 139, 143,
                                                       148, 151, 155, 160, 163, 168, 173, 176,
                                                       181, 186, 190, 195, 200, 204, 209, 215,
                                                       218, 224, 230, 233, 239, 245, 249, 255
                                                      };

//const uint8_t logBulbBrightness[BRIGHTNESS_VALUES] = {0, 0, 0, 0, 1, 1, 1, 1,
//                                                      2, 2, 2, 2, 3, 3, 3, 3,
//                                                      4, 4, 4, 4, 5, 5, 5, 5,
//                                                      6, 6, 6, 7, 7, 8, 8, 9,
//                                                      9, 10, 11, 11, 12, 13, 13, 14,
//                                                      15, 16, 16, 17, 18, 19, 19, 20,
//                                                      21, 22, 23, 24, 24, 26, 26, 28,
//                                                      28, 30, 30, 32, 32, 34, 35, 36,
//                                                      37, 38, 39, 41, 42, 43, 44, 46,
//                                                      47, 48, 49, 51, 52, 54, 55, 56,
//                                                      57, 59, 60, 62, 63, 65, 66, 68,
//                                                      69, 71, 73, 75, 76, 78, 79, 81
//                                                     };

const uint8_t logBulbBrightness[BRIGHTNESS_VALUES] = {0, 0, 0, 0, 1, 1, 1, 1,
                                                      2, 2, 2, 2, 3, 3, 3, 3,
                                                      4, 4, 4, 4, 5, 5, 6, 6,
                                                      7, 7, 8, 8, 9, 9, 10, 11,
                                                      11, 12, 13, 14, 15, 16, 16, 17,
                                                      18, 19, 20, 21, 21, 23, 24, 24,
                                                      26, 27, 28, 29, 30, 31, 32, 33,
                                                      35, 36, 37, 38, 40, 41, 42, 44,
                                                      45, 47, 48, 49, 51, 53, 54, 56,
                                                      57, 58, 60, 62, 63, 65, 67, 68,
                                                      70, 73, 74, 76, 78, 79, 81, 84,
                                                      85, 87, 89, 91, 93, 95, 97, 99
                                                     };
Encoder encoderLeft(LINKS_A, LINKS_B);
Encoder encoderRight(RECHTS_A, RECHTS_B);

enum KnobState {
  SELF,
  OTHER,
  UNDER_SELF,
  UNDER_OTHER
};

struct Knob {
  KnobState state = SELF;                                   // Status van deze knop.
  int32_t pos;                                              // De huidige positie van deze encoder.
  uint32_t activity;                                        // Wanneer deze knop voor het laatst gedraaid is (millis() waarde).
  bool lastState = HIGH;                                    // De oude staat van de drukschakelaar.
  uint32_t lastPressed;                                     // Wanneer deze knop voor het laatst ingedrukt werd (millis() waarde).
  uint32_t posWhenPressed;                                  // Positie van de encoder op het moment dat de knop wordt ingedrukt.
} leftKnob, rightKnob;

struct UnderLight {
  uint8_t brightness;
  bool motionActive;
  bool motionEnabled = true;
};

void setup() {
  pinMode(LINKS_A, INPUT_PULLUP);
  pinMode(LINKS_B, INPUT_PULLUP);
  pinMode(LINKS_S, INPUT_PULLUP);
  pinMode(RECHTS_A, INPUT_PULLUP);
  pinMode(RECHTS_B, INPUT_PULLUP);
  pinMode(RECHTS_S, INPUT_PULLUP);

  pinMode(LAMP_LINKS, OUTPUT);
  pinMode(LAMP_RECHTS, OUTPUT);
  pinMode(LAMP_ONDER, OUTPUT);

  Timer1.initialize(1000);
}

void loop() {
  static uint8_t leftBrightness;
  static uint8_t rightBrightness;
  static UnderLight underLight;

  static Knob leftEncoder;
  static Knob rightEncoder;

  handleKnob(&leftKnob, encoderLeft.read(), &leftBrightness, &rightBrightness, &underLight);
  handleKnob(&rightKnob, encoderRight.read(), &rightBrightness, &leftBrightness, &underLight);
//  handleMotionSensor(&underLight);

  Timer1.pwm(LAMP_LINKS, logBulbBrightness[leftBrightness]);
  Timer1.pwm(LAMP_RECHTS, logBulbBrightness[rightBrightness]);
  analogWrite(LAMP_ONDER, logStripBrightness[underLight.brightness]);

  // Read the encoder switch.
  // When switch is closed: set to UNDER.
  // When switch is released with no encoder activity: switch between SELF and OTHER.
  // If OTHER and no activity for 5000 ms: switch to SELF.
  handleSwitch(&leftKnob, digitalRead(LINKS_S));
  handleSwitch(&rightKnob, digitalRead(RECHTS_S));
}

void handleKnob(Knob* knob, const int32_t newPosition, uint8_t* selfBrightness, uint8_t* otherBrightness, UnderLight* underLight) {
  if (newPosition != knob->pos) {
    knob->activity = millis();
    switch (knob->state) {
      case SELF:
        if (newPosition > knob->pos) {
          if (*selfBrightness < BRIGHTNESS_VALUES - 1) {
            (*selfBrightness)++;
          }
        }
        else {
          if (*selfBrightness > 0) {
            (*selfBrightness)--;
          }
        }
        break;

      case OTHER:
        if (newPosition > knob->pos) {
          if (*otherBrightness < BRIGHTNESS_VALUES - 1) {
            (*otherBrightness)++;
          }
        }
        else {
          if (*otherBrightness > 0) {
            (*otherBrightness)--;
          }
        }
        break;

      case UNDER_SELF:
      case UNDER_OTHER:
        if (newPosition > knob->pos) {
          if (underLight->brightness < BRIGHTNESS_VALUES - 1) {
            (underLight->brightness)++;
          }
        }
        else {
          if (underLight->brightness > 0) {
            (underLight->brightness)--;
          }
        }
        if (underLight->brightness == 0) {
          underLight->motionEnabled = true;                 // Manually lights off: reactivate the motion sensor.
          underLight->motionActive = false;                 // Motion controlled light is of course not active now.
        }
        else {
          underLight->motionEnabled = false;                // Override the motion sensor until the lights are manually switched off.
        }
        break;
    }
    knob->pos = newPosition;
  }
}

void handleSwitch(Knob* knob, bool newState) {
  if (newState == LOW) {                                    // Encoder switch is pressed.
    if (knob->lastState == HIGH) {                          // But it was not yet pressed.
      knob->lastPressed = millis();                         // Record when it got pressed.
      knob->state = (knob->state == SELF) ? UNDER_SELF : UNDER_OTHER;   // Pressed: now we're managing the UNDER lights.
      knob->lastState = LOW;                                // Remember the state we're in.
      knob->posWhenPressed = knob->pos;
    }
  }
  else {                                                    // Switch is now not pressed; newState is HIGH.
    if (knob->lastState == LOW) {                           // Switch was just released.
      if (millis() - knob->lastPressed > DEBOUNCE_INTERVAL) {  // It's been longer than we expect the switch to bounce, so we may act on it.
        knob->lastState = HIGH;                             // Remember the state we're in.
        if ((millis() - knob->lastPressed < millis() - knob->activity) ||
            (abs(knob->pos - knob->posWhenPressed) < 4)) {  // No activity or no actual change in position since pressed: this is a click to change sides.
          knob->state = (knob->state == UNDER_SELF) ? OTHER : SELF;
        }
        else {                                              // UNDER lights have changed; revert to previous state.
          knob->state = (knob->state == UNDER_SELF) ? SELF : OTHER;
        }
      }
    }
  }
  if (newState == HIGH && knob->state != SELF) {            // If set to anything but SELF and the switch is not pressed,
    if (millis() - knob->activity > OTHER_TIMEOUT) {        // after some time of no activity
      knob->state = SELF;                                   // revert to SELF.
    }
  }
}

/*
  Gedrag van de bewegingssensor:
  - als beweging gedetecteerd en helderheid lager dan MOTION_BRIGHTNESS: fade in tot die helderheid.
    - Na geen sensor signaal voor MOTION_ACTIVE ms: fade uit tot helderheid van voor de sensor was geactiveerd.

  - bij handmadig instellen van de helderheid tijdens de sensor sequence:
    - niet automatisch regelen totdat het licht handmatig is uitgedaan.

*/

void handleMotionSensor(UnderLight* underLight) {

  static uint32_t motionLastActive = -MOTION_ACTIVE;          // Ensure no activation upon starting up.
  static bool oldSensorState;
  static uint8_t oldUnderBrightness;
  static uint32_t lastFadeTick;

  if (underLight->motionEnabled) {                            // Only work on the sensor if it's enabled (i.e. no manual override done through the knob).
    bool sensorActive = digitalRead(MOTIONSENSOR1) || digitalRead(MOTIONSENSOR2);
    if (sensorActive) {
      motionLastActive = millis();                            // Retrigger as long as the sensor is active.
      if (oldSensorState == LOW) {                            // Sensor just got triggered.
        oldSensorState = HIGH;                                // Remember this.
        if (underLight->motionActive == false) {              // Motion controlled light was not yet active.
          underLight->motionActive = true;                    // But now it is!
          oldUnderBrightness = underLight->brightness;        // Remember to what brightness we have to return later.
          lastFadeTick = millis();
          if (underLight->brightness < 4) {
            underLight->brightness = 4;                       // Beginnivo.
          }
        }
      }
    }
    else {
      oldSensorState = LOW;
    }
    
    if (millis() - motionLastActive < MOTION_ACTIVE) {      // De sensor sequence is actief.
      if (underLight->brightness < MOTION_BRIGHTNESS) {     // Helderheid is nog te laag.
        if (millis() - lastFadeTick > FADE_STEP_SPEED) {    // Tijd voor de volgende stap.
          underLight->brightness++;
          lastFadeTick = millis();
        }
      }
    }
    else {                                                  // Lang genoeg aan geweest.
      if (underLight->brightness > oldUnderBrightness) {    // Verlichting is te fel, zet 'm lager.
        if (millis() - lastFadeTick > FADE_STEP_SPEED) {    // Tijd voor de volgende stap.
          underLight->brightness--;
          lastFadeTick = millis();
        }
      }
      else {
        underLight->motionActive = false;                   // We hebben de doelhelderheid bereikt.
      }
    }
  }
}
