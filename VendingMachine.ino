#include <Arduino.h>
#include "Model.h"
#include "Controller.h"
#include "View.h"
#include "Enums.h"
#include "Pins.h"


// Declarations
void distribute(Alphabet token);


// File represents the interface between model view and controller components.
  // As of now:
    // Model represents the parent automata of unioned dfa1 and dfa2.
    // Controller recieves hardware input from buttons, and also activates motors
    // View activates led to represent what is actively being processed


// Global instances
Model VendingModel;
Controller VendingController;
View VendingView;

// Timeout tracking variables
unsigned long lastInputTime = 0;
const unsigned long INPUT_TIMEOUT = 60000; // 60 seconds

void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println("SETUP START");
  VendingView.begin();
  Serial.println("VIEW READY");

  // PinModes for headers to Motor Driver 1 and Motor Driver 2
  pinMode(D1_AIN1, OUTPUT);
  pinMode(D1_BIN1, OUTPUT);
  pinMode(D1_STBY, OUTPUT);

  pinMode(D2_AIN1, OUTPUT);
  pinMode(D2_BIN1, OUTPUT);
  pinMode(D2_STBY, OUTPUT);

  // Start with everything off on both motor drivers
  digitalWrite(D1_AIN1, LOW);
  digitalWrite(D1_BIN1, LOW);
  digitalWrite(D1_STBY, LOW);

  digitalWrite(D2_AIN1, LOW);
  digitalWrite(D2_BIN1, LOW);
  digitalWrite(D2_STBY, LOW);

  // Initialize buttons with internal pull up resistors - Buttons correlate to each alphabet value. 
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
  pinMode(BTN_3, INPUT_PULLUP);
  pinMode(BTN_4, INPUT_PULLUP);
  pinMode(BTN_5, INPUT_PULLUP);
  
}

void loop() {
  Alphabet token = VendingController.ReadInput();

  // Debug only when something meaningful happens
  if (token != Alphabet::None) {
    Serial.print(F("Input token received: "));
    Serial.println((int)token);
  }

  // ALWAYS distribute — View needs None for edge reset
  distribute(token);
}


// Helper method to distribute token result token to the appropirate subsystem

// TEMP PATCH:
// Alphabet::None is filtered from the Model for now.
// The View still receives None to detect falling edges.
// TODO (cleanup): move edge detection into the Controller
// or teach the Model to explicitly handle None.
void distribute(Alphabet token) {

  static Dispense lastResult = Dispense::None;
  Dispense result = lastResult;

  // Only feed the DFA on real tokens
  if (token != Alphabet::None) {
    result = VendingModel.AcceptToken(token);
    lastResult = result;

    Serial.print(F("Model returned Dispense result: "));
    Serial.println((int)result);
  }

  // View ALWAYS runs (needs falling edge info)
  VendingView.drawUI(result, token);

  // Controller only reacts to terminal actions
  if (result == Dispense::ItemOne ||
      result == Dispense::ItemTwo ||
      result == Dispense::ItemThree ||
      result == Dispense::ItemFour ||
      result == Dispense::Reset) {


    // Route BEFORE dispensing / message
  if (result == Dispense::Reset) {
    VendingController.RouteTokenReject();
  } else {
    VendingController.RouteTokenAccept();
  }

  // Activate motors / physical dispense
  VendingController.HandleResult(result);

  // Blocking UX message
  if (result == Dispense::Reset) {
    VendingView.showMessage("Resetting...", 1500);
  } else {
    VendingView.showMessage("Enjoy!", 1500);
  }

  // Return servo to default position
  VendingController.RouteTokenNeutral();

  // Prevent repeated firing
  lastResult = Dispense::None;
  }
}



