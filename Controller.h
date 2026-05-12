#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Enums.h"
#include <Servo.h>


class Controller {

public:

  Controller();

  // Takes the DFA state from the model and performs hardware actions. Hardware actions are: activate motors, return or recieve coins.
  void HandleResult(Dispense result);

  // Reads input and will be delegated into model through the VendingMachine interface .ino
  Alphabet ReadInput();

  // Controller.h
  void RouteTokenAccept();
  void RouteTokenReject();
  void RouteTokenNeutral();


  void begin(); 

private:
  // Helper to convert input into enum class.
  Alphabet ConvertSerialChar(char c);

  // State trackers to keep track of previous button input - tracking only falling edge presses
  bool lastBtn1;
  bool lastBtn2;
  bool lastBtn3;
  bool lastBtn4;
  bool lastBtn5;

  Servo tokenServo;

};

#endif
