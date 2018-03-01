#include "Keyboard.h"

#define  outputB 15
#define  outputA A0
#define  BUTTON A1
#define PIN_5V  A2
#define PIN_GND  A3


int counter = 0 ;
int aState;
int aLastState;
int functionFlag = 0;
int lastButtonState = 0;

void setup() {

  Keyboard.begin();


  // put your setup code here, to run once:
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(PIN_5V, OUTPUT);
  pinMode(PIN_GND, OUTPUT);
  digitalWrite(PIN_5V, HIGH);
  digitalWrite(PIN_GND, LOW);

  Serial.begin(9600);

  aLastState = digitalRead(outputA);
}
bool count = 0;
bool keyFlag = false;
long lastClickTime = 0;
long lastRotateTime = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - lastClickTime > 1000) {
    aState = digitalRead(outputA);
    if (aState != aLastState) {
      if (digitalRead(outputB) != aState) {
        counter ++;
        Keyboard.press(KEY_LEFT_ARROW);
        Keyboard.releaseAll();
      } else {
        counter --;
        Keyboard.press(KEY_RIGHT_ARROW);
        Keyboard.releaseAll();
      }
      aLastState = aState;
    }
    if (digitalRead(BUTTON) == LOW) {
      if (lastButtonState  == HIGH) {
        if (functionFlag ++ >= 1 ) {
          functionFlag = 0;
        }
        Keyboard.print(" ");
        lastClickTime = millis();
      }
      lastButtonState = LOW;
    } else {
      lastButtonState = HIGH;
    }
  }

}
