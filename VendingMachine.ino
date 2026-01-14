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

  pinMode(D1_AIN1, OUTPUT);
  pinMode(D1_BIN1, OUTPUT);
  pinMode(D1_STBY, OUTPUT);

  pinMode(D2_AIN1, OUTPUT);
  pinMode(D2_BIN1, OUTPUT);
  pinMode(D2_STBY, OUTPUT);

  // Start with everything off
  digitalWrite(D1_AIN1, LOW);
  digitalWrite(D1_BIN1, LOW);
  digitalWrite(D1_STBY, LOW);

  digitalWrite(D2_AIN1, LOW);
  digitalWrite(D2_BIN1, LOW);
  digitalWrite(D2_STBY, LOW);
}

void loop() {

  // Listen for hardware inputs.
  Alphabet token = VendingController.ReadInput();

  if (token != Alphabet::None) {

        Serial.print(F("Input token received: ")); // DEBUG
        Serial.println((int)token);                 // DEBUG

        // update last interaction time
        lastInputTime = millis();

        // Call helper function
        distribute(token);
    }

  // If amount of time elapsed is greater than a minute since last token recieve - reset model - return input coins - notify view
  // if (millis() - lastInputTime > INPUT_TIMEOUT) {

  //   Serial.println(F("Input timeout reached, resetting system")); // DEBUG

  //   // Reset models subsystems
  //   VendingModel.Reset();

  //   // Notify controller
  //   VendingController.HandleResult(Dispense::Reset);
    
  //   // Print to view
  // }
}


// Helper method to distribute token result token to the appropirate subsystem
void distribute(Alphabet token) {
  
  // Send token into the model’s DFA system
  Dispense result = VendingModel.AcceptToken(token);

  Serial.print(F("Model returned Dispense result: ")); // DEBUG
  Serial.println((int)result);                          // DEBUG

  // Direct which output goes to which MVC subsystem

  // Controller - expecting input ItemOne, ItemTwo, ItemThree, ItemFour, and Reset, pass over on Processing.
  if (result == Dispense::ItemOne ||
      result == Dispense::ItemTwo ||
      result == Dispense::ItemThree ||
      result == Dispense::ItemFour ||
      result == Dispense::Reset) {

    Serial.println(F("Controller handling result")); // DEBUG
    VendingController.HandleResult(result);
  }

  // View - expecting each Dispense result at input
}


