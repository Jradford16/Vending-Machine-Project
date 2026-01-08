
#include <Arduino.h>
#include "Model.h"
#include "Controller.h"
#include "View.h"
#include "Enums.h"

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

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

}

void loop() {
 

  // Listen for hardware inputs.
  Alphabet token = VendingController.ReadInput();

  if (token != Alphabet::None) {

        // update last interaction time
        lastInputTime = millis();

        // Call helper function
        distribute(token);
    }
  // If amount of time elapsed is greater than a minute since last token recieve - reset model - return input coins - notify view
  if(millis() - lastInputTime > INPUT_TIMEOUT) {

    // Reset models subsystems
    VendingModel.Reset();

    // Notify controller
    VendingController.HandleResult(Dispense::Reset);
    
    // Print to view
    

  }


}


// Helper method to distribute token result token to the appropirate subsystem
void distribute(Alphabet token) {
  
  // Send token into the model’s DFA system
  Dispense result = VendingModel.AcceptToken(token);

  // Direct which output goes to which MVC subsystem

  // Controller - expecting input ItemOne, ItemTwo, ItemThree, ItemFour, and Reset, pass over on Processing.
  if (result == Dispense::ItemOne ||
    result == Dispense::ItemTwo ||
    result == Dispense::ItemThree ||
    result == Dispense::ItemFour ||
    result == Dispense::Reset) {

  VendingController.HandleResult(result);
  }

  // View - expecting each Dispense result at input
  

 
}


