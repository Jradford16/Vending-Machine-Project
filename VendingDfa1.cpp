#include "VendingDfa1.h"

VendingDfa1::VendingDfa1(int (&modelPrices)[4])
    : prices(modelPrices)   // bind reference here
{
    currentState = DfaStates::Start;   // constructor code
}

TokenProgress VendingDfa1::ProgressState(Alphabet token) {

  // Make distinction of what branch is next by what the current state is.

  // If every single state has a transition with R, leading to an accept state, then at any input token of R:
  // reset the current state and variables maintaining that current state.
  if(token == Alphabet::R){
    return TokenProgress::Reset;

  }

  if(!halted){
  // Decision tree dependent on what the currentState is.
  switch(currentState){

    // Start state case, branch can either accept A or B. Only reachable when current state is Start. 
    case DfaStates::Start:
      // A condition
      if(token == Alphabet::A){
        currentState = DfaStates::A;
        return TokenProgress::Steady;
      }
      // B condition
      if(token == Alphabet::B){
        currentState = DfaStates::B;
        return TokenProgress::Steady;
      }
      halted = true;
      return TokenProgress::Halt;

    // Current state is A, valid input is either 1 or 2. 
    case DfaStates::A:
      if(token == Alphabet::ONE){
        currentState = DfaStates:: A_1;
        return TokenProgress::Steady;
      }
      if(token == Alphabet::TWO){
        currentState = DfaStates::A_2;
        return TokenProgress::Steady;
      }
      halted = true;
      return TokenProgress::Halt;


    // Current state is B, valid input is either 1 or 2. 
    case DfaStates::B:
      if (token == Alphabet::ONE) {
        currentState = DfaStates::B_1;
        return TokenProgress::Steady;
      }
      if (token == Alphabet::TWO) {
        currentState = DfaStates::B_2;
        return TokenProgress::Steady;
      }
      // If neither are hit then invalid input.
      halted = true;
      return TokenProgress::Halt;

    // State to begin acceping money, abstract further states and explicit transitions by just
    // maintaining a variable that keeps track of price which decrements per token insertion.
    
    case DfaStates::A_1:

      if(token == Alphabet::TWENTY_FIVE){
        // First pass check, initialize itemCost to index in array representing price of A1
        if(itemCost == -1){
          itemCost = prices[0];
        }

        // Tracking current state.
        itemCost = itemCost - 25;


        if(itemCost == 0){

          return TokenProgress::Accept;
        }

        return TokenProgress::Steady;
      }

      halted = true;
      return TokenProgress::Halt;

    // Output for item A2
    case DfaStates::A_2:

      if(token == Alphabet::TWENTY_FIVE){
        if (itemCost == -1) {
          itemCost = prices[1];
        }

        itemCost -= 25;  // same concept as above

        if (itemCost == 0) {

          return TokenProgress::Accept;
        }

        return TokenProgress::Steady;
      }

      halted = true;
      return TokenProgress::Halt;

    // State to begin acceping money, abstract further states and explicit transitions by just
    // maintaining a variable that keeps track of price which decrements per token insertion.
    case DfaStates::B_1:

      if(token == Alphabet :: TWENTY_FIVE){
      // First pass check, initialize itemCost to index in array representing price of B1
        if (itemCost == -1) {
          itemCost = prices[2];
        }

        // Tracking current state.
        itemCost = itemCost - 25;

        // *FUTURE, for now just accept exact input operating on cents.
        // If statement represents a transition leading into an accept state.
        if (itemCost == 0) {

        
          return TokenProgress::Accept;
          
        }
        return TokenProgress::Steady;
      }
      halted = true;
      return TokenProgress::Halt;
    
    // State to begin acceping money, abstract further states and explicit transitions by just
    // maintaining a variable that keeps track of price which decrements per token insertion.
    case DfaStates::B_2:

      if(token == Alphabet :: TWENTY_FIVE){

        // First pass check, initialize itemCost to index in array representing price of B2
        if (itemCost == -1) {
          itemCost = prices[3];
        }

        // Tracking current state.
        itemCost = itemCost - 25;

        // *FUTURE, for now just accept exact input operating on cents.
        // If statement represents a transition leading into an accept state.
        if (itemCost == 0) {

          return TokenProgress::Accept;
        }

        return TokenProgress::Steady;

      }
      halted = true;
      return TokenProgress::Halt;

  }
    
  }
    return TokenProgress::Halt;
}


void VendingDfa1::ResetSubsystem(){

  currentState = DfaStates::Start;
  itemCost = -1;
  halted = false;

}

DfaStates VendingDfa1 :: CurrentState(){

  return currentState;
}



