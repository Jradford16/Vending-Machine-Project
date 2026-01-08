#include "Model.h"
#include <EEPROM.h>

Model::Model(): Dfa1(prices), Dfa2(prices)
{
     // Load from EEPROM or initialize defaults
    for (int i = 0; i < 4; i++) {

        // Read stored price
        uint8_t val = EEPROM.read(i);

        // If EEPROM is "blank" (255), initialize to default 100
        if (val == 255) {
            prices[i] = 100;       // default price
            EEPROM.write(i, 100);  // store into EEPROM
        }
        else {
            prices[i] = val;       // load saved price
        }
    }
}

// With DFA subsystems, each DFA is responsible for upholding its own DFA properties, like resetting, accepting, and transitioning. They can parse and increment on their own, 
// but if they are tied to a larger system, those subsystems still uphold their own local responsibilities. 
// However, there is a greater system that now upholds its own accept, reject, and transition responsibilities, which can dictate the smaller machines. So, in general, 
// DFAs are like recursive structures, where they could essentially be represented as a state itself within a larger DFA.
Dispense Model::AcceptToken(Alphabet token) {

  // Insert token into both dfa's.
    // Purpose of this model is to filter output from each individual dfa's, waiting for what output is relevent to the interface. Parent automata holds
    // subsystems accountable by enforcing the acceptance and reset rules. Although subsystems are in charge of keeping track of their own states, transitions, and accepts.
    
    // Filter output of dfa1 and dfa2 dependent on combinatorical possabilitys.

    TokenProgress dfa1Result = Dfa1.ProgressState(token);
    TokenProgress dfa2Result = Dfa2.ProgressState(token);


    // Outline conditions beginning with dfa2Result on halt

    if(dfa2Result == TokenProgress::Halt && dfa1Result == TokenProgress::Accept){

      // Reset Dfa's to start state.
      Dfa1.ResetSubsystem();
      Dfa2.ResetSubsystem();

      // Check current state of Dfa1 to see which accept state got hit.
      if(Dfa1.CurrentState() == DfaStates::A_1){
        return Dispense::ItemOne;
      }
      else{
        return Dispense::ItemTwo;
      }

    }

    if(dfa2Result == TokenProgress::Halt && dfa1Result == TokenProgress::Steady){

      return Dispense::Processing;

    }

    if(dfa2Result == TokenProgress::Reset && dfa1Result == TokenProgress::Reset){

      // Reset Dfa's to start state.

      Dfa1.ResetSubsystem();
      Dfa2.ResetSubsystem();

      return Dispense::Reset;

    }

    if(dfa2Result == TokenProgress::Halt && dfa1Result == TokenProgress::Halt){

      Dfa1.ResetSubsystem();
      Dfa2.ResetSubsystem();

      return Dispense::Reset;

    }

    // Outline conditions beginning with Dfa1 on halt

    if(dfa2Result == TokenProgress::Accept && dfa1Result == TokenProgress::Halt){

       // Reset Dfa's to start state.
      Dfa1.ResetSubsystem();
      Dfa2.ResetSubsystem();

      // Check current state of Dfa1 to see which accept state got hit.
      if(Dfa2.CurrentState() == DfaStates::B_1){
        return Dispense::ItemThree;
      }
      else{
        return Dispense::ItemFour;
      }

    }

    if(dfa2Result == TokenProgress::Steady && dfa1Result == TokenProgress::Halt){
      return Dispense::Processing;
    }

    // If no branches are hit (Which should not happen depending on combinatorical possabilities all accounted for) - reset the machine.
    return Dispense::Reset;
}

void Model::SetPrice(int price, int item) {
  if (item < 1 || item > 4) {
    return; // invalid item, ignore
  }

  int index = item - 1;
  prices[index] = price;
  EEPROM.write(index, price);
}

void Model::Reset() {
    // Reset both internal DFA subsystems
    Dfa1.ResetSubsystem();
    Dfa2.ResetSubsystem();
}