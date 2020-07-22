
// [ Instruction ]
//Youtube https://youtu.be/FACm5D3bskQ
//Manual https://sites.google.com/view/100happythings/15-premiere-pro-dial-diy
//Source Code https://github.com/happythingsmaker/PremiereProDialDIY
//3D Printing File https://www.thingiverse.com/thing:2810760

//[ Parts and Tools ]
//Rotational encoder with switch http://ebay.to/2t6I0f4  ($1)
//Arduino pro micro http://ebay.to/2oxvIrw ($4.9)
//Non Slip Rubber http://ebay.to/2oyFGc1 ($0.5)

// include a Keyboard library
#include "Keyboard.h"

// From here, text "OUTPUT_B" is going to be replaced with "15".
#define  OUTPUT_B 15

// "OUTPUT_A" is going to be replaced with "A0".
#define  OUTPUT_A A0

// "BUTTON" is going to be replaced with "A1".
#define  BUTTON A1

// "PIN_5V" is going to be replaced with "A2".
#define PIN_5V  A2

// "PIN_GND" is going to be replaced with "A3".
#define PIN_GND  A3

// Declare variables aState, aLastState for checking the state of OUTPUT_A of the encoder
bool aState;

// We need to save the previous state of OUTPUT_A
bool aLastState;

// this variable for check the state of button.
// in order to prevent the button from chattering, we need to check the first moment the button is pressed.
// Becasue when a button pressed, the button usually sends a lot of signals.
// So, we need to ignore following noise signals for a while.
bool lastButtonState = 0;


// void setup(){} function is for one time setting
void setup() {

  // in order to use the Keyboard library, begin() is necessary
  Keyboard.begin();

  // OUTPUT_A (A0) is for INPUT
  pinMode(OUTPUT_A, INPUT);

  // OUTPUT_B (15) is for INPUT
  pinMode(OUTPUT_B, INPUT);

  // BUTTON (A1) is for INPUT
  // Most pin has thier own pull-up resistor.
  // INPUT_PULLUP makes the pin high.
  // a leg of button is connected with this pin and GND
  // when the button is not pressed, the pin reads HIGH signal because of this PULL-UP
  // when the button is pressed, pin is going to be LOW which means "pressed"
  pinMode(BUTTON, INPUT_PULLUP);

  // PIN_5V (A2) is for OUTPUT
  // This pin is used for giving 5V to the encoder.
  // normally, 5v is coming from VCC, but, I didn't want to use any wires
  // So, this is a kind of trick, but it works well.
  pinMode(PIN_5V, OUTPUT);
  digitalWrite(PIN_5V, HIGH);

  // PIN_GND (A3) is for OUTPUT
  // This pin is used for giving GND to the encoder.
  // normally, GND is coming from GND, but, I didn't want to use any wires
  // So, this is a kind of trick, but it works well.
  pinMode(PIN_GND, OUTPUT);
  digitalWrite(PIN_GND, LOW);

  // read a signal from OUTPUT_A
  // this is for initialization
  aLastState = digitalRead(OUTPUT_A);
}


// in order to prevent chattering, we need to check the moment when was the last click moment
// for 1000ms, we will ignore all signals
long lastClickTime = 0;
long tempCount = 0;

// this loop() function repeats its code eternally
void loop() {

  // millis() returns current millisecond from the moment when the arduino powers on
  if (millis() - lastClickTime > 1000) {

    //read signal from OUTPUT_A and save its state to aState
    aState = digitalRead(OUTPUT_A);

    // if aLastState is not currentState, it meant there's something changed.
    if (aState != aLastState) {

      // read another pin's state.
      // if you want to know about the theory, watch this video
      // https://www.youtube.com/watch?v=v4BbSzJ-hz4
      if (digitalRead(OUTPUT_B) != aState) {

        // send Left Arrow
        if (tempCount ++ % 2 == 0) {
          Keyboard.press(KEY_LEFT_ARROW);
          delay(8);
        }
      } else {

        // send Right Arrow
        if (tempCount ++ % 2 == 0) {
          Keyboard.press(KEY_RIGHT_ARROW);
          delay(8);
        }
      }

      Keyboard.releaseAll();

      // save current State as last State
      aLastState = aState;
    }

    // read Button pin and check it is pressed (LOW)
    if (digitalRead(BUTTON) == LOW) {

      // and this code checks whether it is edge (HIGH->LOW)
      if (lastButtonState == HIGH) {

        // sends space character
        Keyboard.print(" ");

        // save currnet milliseconds for ignoring for a while
        lastClickTime = millis();
      }

      // for checking edge
      lastButtonState = LOW;
    } else {

      // for checking edge
      lastButtonState = HIGH;
    }
  }
}
