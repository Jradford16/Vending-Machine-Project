#include "VendingDfa2.h"

VendingDfa2::VendingDfa2(int (&modelPrices)[4])
    : prices(modelPrices)   // bind to Model's array
{
    currentState = DfaStates::Start;  
}

TokenProgress VendingDfa2::ProgressState(Alphabet token) {
  
  // If every single state has a transition with R, leading to an accept state, then at any input token of R:
  // reset the current state and variables maintaining that current state.
  if (token == Alphabet::R) {
    return TokenProgress::Reset;
  }

  if (!halted) {
    if (!monetaryTokenLock) {

      if (token == Alphabet::TWENTY_FIVE) {

        // Accumulate value of inserted monetary tokens
        monetaryInput += 25;
        return TokenProgress::Steady;

      }

      monetaryTokenLock = true;
    }

    if (monetaryTokenLock) {
      switch (currentState) {

        // Start state case, branch can either accept A or B. Only reachable when current state is Start. 
        case DfaStates::Start:
          // A condition
          if (token == Alphabet::A) {
            currentState = DfaStates::A;
            return TokenProgress::Steady;
          }
          // B condition
          if (token == Alphabet::B) {
            currentState = DfaStates::B;
            return TokenProgress::Steady;
          }
          halted = true;
          return TokenProgress::Halt;

        // Current state is A, valid input is either 1 or 2.

        // ! Current implementation only looking for exact value. Can adjust in future.
        case DfaStates::A:
          if (token == Alphabet::ONE && prices[0] == monetaryInput) {
            currentState = DfaStates::A_1;
            return TokenProgress::Accept;
          }
          if (token == Alphabet::TWO && prices[1] == monetaryInput) {
            currentState = DfaStates::A_2;
            return TokenProgress::Accept;
          }
          // If neither are hit then invalid input.
          halted = true; 
          return TokenProgress::Halt;

        // Current state is B, valid input is either 1 or 2. 

        // ! Current implementation only looking for exact value. Can adjust in future.
        case DfaStates::B:
          if (token == Alphabet::ONE && prices[2] == monetaryInput) {
            currentState = DfaStates::B_1;
            return TokenProgress::Accept;
          }
          if (token == Alphabet::TWO && prices[3] == monetaryInput) {
            currentState = DfaStates::B_2;
            return TokenProgress::Accept;
          }
          // If neither are hit then invalid input.
          halted = true;
          return TokenProgress::Halt;
      }
    }
  }

  return TokenProgress::Halt;
}

void VendingDfa2::ResetSubsystem() {
  currentState = DfaStates::Start;
  halted = false;
  monetaryTokenLock = false;
  monetaryInput = 0;
}

DfaStates VendingDfa2::CurrentState() {
  return currentState;
}

