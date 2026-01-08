#ifndef VENDINGDFA2_H
#define VENDINGDFA2_H

#include "Enums.h"

class VendingDfa2 {

public:

  VendingDfa2(int (&modelPrices)[4]);

  // Primary handler that drives the motion of our underlying dfa. Return values specify which state we are on.
  TokenProgress ProgressState(Alphabet token);

  // Called by parent automata to reset internal halted state back to start as well as current item cost.
  void ResetSubsystem();

  // Getter method to get notified what is the current state abstracted by this dfa.
  DfaStates CurrentState();

private:

  DfaStates currentState;

  // Variable to keep track of monetary token input
  int monetaryInput = 0;

  // Tracks monetary token input, after token that is not monetary is recieved barrier is placed.
  bool monetaryTokenLock = false;

  // Array of prices for each item
  int (&prices)[4];

  bool halted = false;
};

#endif

