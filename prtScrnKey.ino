// Boards manager URL: http://digistump.com/package_digistump_index.json
// Board: Digispark (Default 16.5MHz)

#include <DigiKeyboard.h>

int ledPin = 1;
int buttonPin = 0;
int groundPin = 5;
int fade = 6;

int lastButtonState;
int buttonState;
int brighthess;

void notify() {
  for (int brightness = 40; brightness > 0; brightness -= fade) {
    analogWrite(ledPin, brightness);
    DigiKeyboard.delay(30);
  }
  analogWrite(ledPin, 0);
}

void sendKey(byte keyStroke, byte modifiers) {
  while (!usbInterruptIsReady()) {
    usbPoll();
    _delay_ms(5);
  }
  memset(DigiKeyboard.reportBuffer, 0, sizeof(DigiKeyboard.reportBuffer));
  DigiKeyboard.reportBuffer[0] = modifiers;
  DigiKeyboard.reportBuffer[1] = keyStroke;
  usbSetInterrupt(DigiKeyboard.reportBuffer, sizeof(DigiKeyboard.reportBuffer));
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH); // PULLUP
  pinMode(groundPin, OUTPUT);
  digitalWrite(groundPin, LOW);
  notify(); // ready
  buttonState = digitalRead(buttonPin);
}

void loop() {
  // detect alternate switch altered
  for (lastButtonState = buttonState; lastButtonState == buttonState; buttonState = digitalRead(buttonPin)) {
    DigiKeyboard.delay(100);
  }
  digitalWrite(buttonPin, HIGH); // KEEP PULLUP

  // send Key Strokes; see http://www.usb.org/developers/hidpage/Hut1_12v2.pdf#page=53
  sendKey(0, 0); // all key up
  sendKey(70, MOD_ALT_LEFT);  // Alt + Tab (43) // PrtSc 70
  sendKey(0, MOD_ALT_LEFT);   // Alt
  sendKey(0, 0); // all key up
  notify();
}

