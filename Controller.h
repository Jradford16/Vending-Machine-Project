#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Enums.h"

class Controller {

public:

  Controller();

  // Takes the DFA state from the model and performs hardware actions. Hardware actions are: activate motors, return or recieve coins.
  void HandleResult(Dispense result);

  // Reads input and will be delegated into model through the VendingMachine interface .ino
  Alphabet ReadInput();

private:
  // Helper to convert input into enum class.
  Alphabet ConvertSerialChar(char c);

};

#endif
