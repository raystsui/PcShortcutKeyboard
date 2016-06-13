#include <Keyboard.h>
#include <SoftPWM.h>
#include <SoftPWM_timer.h>

#include <Bounce2.h>

#define NUM_BUTTONS 8
#define DEBOUNCE_MS 5

// -----------------------------------------------------------------------------
//  #
//  # DECLARATIONS
//  #
// -----------------------------------------------------------------------------

typedef struct {
  Bounce bounce;
  uint8_t buPin;
  uint8_t ledPin;
} button_t;

uint8_t buPinList[] = {
  0, 1, 2, 3,
  4, 5, 6, 7
};

uint8_t ledPinList[] = {
  8, 9, 10, 11,
  A0, A1, A2, A3
};

char hotKeyList[] = {
  '1', '2', '3', '4',
  '5', '6', '7', '8',
};

button_t bu[NUM_BUTTONS];
unsigned long time;
unsigned long m;

// -----------------------------------------------------------------------------
//  #
//  # CUSTOM FUNCTIONS
//  #
// -----------------------------------------------------------------------------
void sendHotKey(uint8_t idx) {
  if ( idx < 0 || idx >= NUM_BUTTONS ) return;
  Keyboard.releaseAll();
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(hotKeyList[idx]);
  delay(10);
  Keyboard.releaseAll();
}

// -----------------------------------------------------------------------------
//  #
//  # SETUP
//  #
// -----------------------------------------------------------------------------
void startupShow() {
    SoftPWMSetFadeTime(l, 40, 200);
  for (int i = 0; i < 8; i++) {
    SoftPWMSetPercent(bu[i].ledPin, 100);
    delay(40);
  }
  delay(50);

  for (int i = 0; i < 8; i++) {
    SoftPWMSetPercent(bu[i].ledPin, 0);
    delay(50);
  }

  for (int i = 0; i < 8; i++) {
    SoftPWMSetPercent(bu[i].ledPin, 100);
    delay(40);
  }
  delay(50);

  for (int i = 0; i < 8; i++) {
    SoftPWMSetPercent(bu[i].ledPin, 0);
    delay(50);
  }
    SoftPWMSetFadeTime(bu[i].ledPin, 50, 500);
}

void initButton() {
  SoftPWMBegin(SOFTPWM_INVERTED);
  Keyboard.begin();
  // Init for Buttons and LEDs (PWM)
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    uint8_t b =  buPinList[i];
    uint8_t l = ledPinList[i];
    Bounce *BU = &bu[i].bounce;
    *BU = Bounce();
    bu[i].buPin = b;
    bu[i].ledPin = l;
    pinMode(b, INPUT);
    digitalWrite(b, HIGH);
    BU->attach(bu[i].buPin);
    BU->interval(DEBOUNCE_MS);
    SoftPWMSet(l, 0);
    SoftPWMSetFadeTime(l, 40, 200);
  }
}

//void setPWMFadeTime() {
//  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
//    SoftPWMSetFadeTime(bu[i].ledPin, 50, 1000);
//  }
//}

float millisToSecond(unsigned long m) {
  return (m / 1000);
}
float millisToMinute(unsigned long m) {
  return (m / 60000);
}

// -----------------------------------------------------------------------------
//  #
//  # SETUP
//  #
// -----------------------------------------------------------------------------
void setup() {
  // Kickstart sub-system library modules
  initButton();
  startupShow();
  time = millis();
}

// -----------------------------------------------------------------------------
//  #
//  # LOOP
//  #
// -----------------------------------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    bu[i].bounce.update();
  }
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    uint8_t l = bu[i].ledPin;

    if (   bu[i].bounce.fell() ) {
      // Button Pressed
      SoftPWMSetPercent(l, 100);
      sendHotKey(i);
//      Keyboard.write('A');
    } else if (   bu[i].bounce.rose() ) {
      // Button Released
      SoftPWMSetPercent(l, 0);
    }
  }

  m = millis();
  if (millisToMinute(m - time) >= 5) {
    startupShow();
    time = m;
  }
}






