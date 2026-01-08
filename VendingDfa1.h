#ifndef VendingDfa1_H
#define VendingDfa1_H

#include "Enums.h"

class VendingDfa1{

public:

  VendingDfa1(int (&modelPrices)[4]);

  // Primary handler that drives the motion of our underlying dfa. Return values specify which state we are on.
  TokenProgress ProgressState(Alphabet token);

  // Called by parent autonmata to reset internal halted state back to start as well as current item cost.
  void ResetSubsystem();

  // Getter method to get notified what is the current state abstracted by this dfa. 
  DfaStates CurrentState();

private:

  DfaStates currentState;

  // Variable to be set once appropriate dfa state has been reached.
  int itemCost = -1;

  // Array of prices for each item
  int (&prices)[4];

  bool halted = false;


};

#endif