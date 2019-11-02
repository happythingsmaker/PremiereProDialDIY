// Resolution : some encoder is too sensitive. this reduces sensitivity.
#define RESOLUTION 2


// include a Keyboard library
#include "Keyboard.h"


// include a EEPROM library for memorizing last function
#include <EEPROM.h>


// "OUTPUT_A" is going to be replaced with "A0".
#define  OUTPUT_A 2


// "PIN_GND_ENCODER" is going to be replaced with "3".
#define PIN_GND_ENCODER  3


// "OUTPUT_B" is going to be replaced with "15".
#define  OUTPUT_B 4


// "BUTTON" is going to be replaced with "A1".
#define  BUTTON A1


// "PIN_GND_BUTTON" is going to be replaced with "A3".
#define PIN_GND_BUTTON  A3


// Declare variables aState, aLastState for checking the state of OUTPUT_A of the encoder
bool aState;


// We need to save the previous state of OUTPUT_A
bool aLastState;


// this variable for check the state of button.
bool lastButtonState = 0;


// mode selection
#define HORIZONTAL_MODE 0
#define VERTICAL_MODE 1


int mode = HORIZONTAL_MODE;
const int numMode = 2;


// void setup(){} function is for one time setting
void setup() {


  //read the last mode
  mode = EEPROM.read(0);


  // in order to use the Keyboard library, begin() is necessary
  Keyboard.begin();


  // OUTPUT_A is for INPUT. must be pull-up
  pinMode(OUTPUT_A, INPUT_PULLUP);


  // OUTPUT_B is for INPUT, must be pull-up
  pinMode(OUTPUT_B, INPUT_PULLUP);                                                                                                                                                        


  // BUTTON is for INPUT
  // Most pin has thier own pull-up resistor.
  // INPUT_PULLUP makes the pin high.
  // a leg of button is connected with this pin and another pin which is grounded.
  // when the button is not pressed, the pin reads HIGH signal because of this PULL-UP
  // when the button is pressed, pin is going to be LOW which means "pressed"
  pinMode(BUTTON, INPUT_PULLUP);


  // PIN_GND_ENCODER is for OUTPUT
  // This pin is used for giving GROUND to the encoder.
  // this is a kind of trick, but it works well.
  pinMode(PIN_GND_ENCODER, OUTPUT);
  digitalWrite(PIN_GND_ENCODER, LOW);


  // PIN_GND_BUTTON is for OUTPUT
  // This pin is used for giving GND to the button.
  // this is a kind of trick, but it works well.
  pinMode(PIN_GND_BUTTON, OUTPUT);
  digitalWrite(PIN_GND_BUTTON, LOW);




  // read a signal from OUTPUT_A
  // this is for initialization
  aLastState = digitalRead(OUTPUT_A);
}


long tempCount = 0;


// this loop() function repeats its code eternally
void loop() {


  //read signal from OUTPUT_A and save its state to aState
  aState = digitalRead(OUTPUT_A);


  // if aLastState is not currentState, it meant there's something changed.
  if (aState != aLastState) {


    // read another pin's state.
    // if you want to know about the theory, watch this video
    // https://www.youtube.com/watch?v=v4BbSzJ-hz4
    if (digitalRead(OUTPUT_B) != aState) {
      rotateLeft();
    } else {
      rotateRight();
    }
    // save current State as last State
    aLastState = aState;
  }


  // read button (short or long)
  if (digitalRead(BUTTON) == LOW) {
    if (lastButtonState == LOW) {
      // LOW -> LOW : nothing to do
    } else {
      // HIGH-> LOW
      delay(300); // ignoring chattering
    }
    lastButtonState = LOW;
  } else {
    if (lastButtonState == LOW) {   // LOW -> HIGH : check whether long press or not
      //changeMode();
      Keyboard.println();
    }
    else {                          // HIGH -> HIGH : noting to do
    }
    lastButtonState = HIGH;
  }
}


void changeMode() {
  mode = ++mode % numMode;
  EEPROM.write(0, mode);
}


void rotateLeft() {
  if (tempCount++ % RESOLUTION == 0) {
    Keyboard.press(KEY_TAB);
    Keyboard.releaseAll();
  }
}

void rotateRight() {
  if (tempCount++ % RESOLUTION == 0) {
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_TAB);
    Keyboard.releaseAll();
  }
}
