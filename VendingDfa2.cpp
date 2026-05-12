#include "VendingDfa2.h"
#include <Arduino.h>

VendingDfa2::VendingDfa2(int (&modelPrices)[4])
    : prices(modelPrices)
{
    currentState = DfaStates::Start;
    Serial.println(F("DFA2 initialized")); // DEBUG
}

TokenProgress VendingDfa2::ProgressState(Alphabet token) {

  Serial.print(F("DFA2 received token: ")); // DEBUG
  Serial.println((int)token);               // DEBUG

  // Reset dominates
  if (token == Alphabet::R) {
    Serial.println(F("DFA2: Reset token received")); // DEBUG
    return TokenProgress::Reset;
  }

  if (!halted) {
    switch (currentState) {

      // ---- Step 1: expect number first (1 or 2) ----
      case DfaStates::Start:
        Serial.println(F("DFA2 state: Start")); // DEBUG

        if (token == Alphabet::ONE) {
          currentState = DfaStates::A; // DFA2: "got 1"
          Serial.println(F("DFA2 transition: Start -> (got ONE)")); // DEBUG
          return TokenProgress::Steady;
        }

        if (token == Alphabet::TWO) {
          currentState = DfaStates::B; // DFA2: "got 2"
          Serial.println(F("DFA2 transition: Start -> (got TWO)")); // DEBUG
          return TokenProgress::Steady;
        }

        halted = true;
        Serial.println(F("DFA2 halted at Start")); // DEBUG
        return TokenProgress::Halt;

      // ---- Step 2: expect letter second (A or B) ----
      // If first token was ONE (state A), then:
      //   A -> A_1, B -> B_1
      case DfaStates::A:
        Serial.println(F("DFA2 state: (got ONE)")); // DEBUG

        if (token == Alphabet::A) {
          currentState = DfaStates::A_1; // item A1
          Serial.println(F("DFA2 transition: ONE -> A_1")); // DEBUG
          return TokenProgress::Steady;
        }
        if (token == Alphabet::B) {
          currentState = DfaStates::B_1; // item B1
          Serial.println(F("DFA2 transition: ONE -> B_1")); // DEBUG
          return TokenProgress::Steady;
        }

        halted = true;
        Serial.println(F("DFA2 halted at (got ONE)")); // DEBUG
        return TokenProgress::Halt;

      // If first token was TWO (state B), then:
      //   A -> A_2, B -> B_2
      case DfaStates::B:
        Serial.println(F("DFA2 state: (got TWO)")); // DEBUG

        if (token == Alphabet::A) {
          currentState = DfaStates::A_2; // item A2
          Serial.println(F("DFA2 transition: TWO -> A_2")); // DEBUG
          return TokenProgress::Steady;
        }
        if (token == Alphabet::B) {
          currentState = DfaStates::B_2; // item B2
          Serial.println(F("DFA2 transition: TWO -> B_2")); // DEBUG
          return TokenProgress::Steady;
        }

        halted = true;
        Serial.println(F("DFA2 halted at (got TWO)")); // DEBUG
        return TokenProgress::Halt;

      // ---- Step 3: quarters until price hits 0 (same style as DFA1) ----

      case DfaStates::A_1:
        Serial.println(F("DFA2 state: A_1")); // DEBUG

        if (token == Alphabet::TWENTY_FIVE) {
          if (itemCost == -1) {
            itemCost = prices[0];
            Serial.print(F("DFA2 A_1 price initialized: "));
            Serial.println(itemCost);
          }

          itemCost -= 25;
          Serial.print(F("DFA2 A_1 remaining cost: "));
          Serial.println(itemCost);

          if (itemCost == 0) {
            Serial.println(F("DFA2 A_1 ACCEPT")); // DEBUG
            return TokenProgress::Accept;
          }
          return TokenProgress::Steady;
        }

        halted = true;
        Serial.println(F("DFA2 halted at A_1")); // DEBUG
        return TokenProgress::Halt;

      case DfaStates::A_2:
        Serial.println(F("DFA2 state: A_2")); // DEBUG

        if (token == Alphabet::TWENTY_FIVE) {
          if (itemCost == -1) {
            itemCost = prices[1];
            Serial.print(F("DFA2 A_2 price initialized: "));
            Serial.println(itemCost);
          }

          itemCost -= 25;
          Serial.print(F("DFA2 A_2 remaining cost: "));
          Serial.println(itemCost);

          if (itemCost == 0) {
            Serial.println(F("DFA2 A_2 ACCEPT")); // DEBUG
            return TokenProgress::Accept;
          }
          return TokenProgress::Steady;
        }

        halted = true;
        Serial.println(F("DFA2 halted at A_2")); // DEBUG
        return TokenProgress::Halt;

      case DfaStates::B_1:
        Serial.println(F("DFA2 state: B_1")); // DEBUG

        if (token == Alphabet::TWENTY_FIVE) {
          if (itemCost == -1) {
            itemCost = prices[2];
            Serial.print(F("DFA2 B_1 price initialized: "));
            Serial.println(itemCost);
          }

          itemCost -= 25;
          Serial.print(F("DFA2 B_1 remaining cost: "));
          Serial.println(itemCost);

          if (itemCost == 0) {
            Serial.println(F("DFA2 B_1 ACCEPT")); // DEBUG
            return TokenProgress::Accept;
          }
          return TokenProgress::Steady;
        }

        halted = true;
        Serial.println(F("DFA2 halted at B_1")); // DEBUG
        return TokenProgress::Halt;

      case DfaStates::B_2:
        Serial.println(F("DFA2 state: B_2")); // DEBUG

        if (token == Alphabet::TWENTY_FIVE) {
          if (itemCost == -1) {
            itemCost = prices[3];
            Serial.print(F("DFA2 B_2 price initialized: "));
            Serial.println(itemCost);
          }

          itemCost -= 25;
          Serial.print(F("DFA2 B_2 remaining cost: "));
          Serial.println(itemCost);

          if (itemCost == 0) {
            Serial.println(F("DFA2 B_2 ACCEPT")); // DEBUG
            return TokenProgress::Accept;
          }
          return TokenProgress::Steady;
        }

        halted = true;
        Serial.println(F("DFA2 halted at B_2")); // DEBUG
        return TokenProgress::Halt;
    }
  }

  Serial.println(F("DFA2 fallback HALT")); // DEBUG
  return TokenProgress::Halt;
}

void VendingDfa2::ResetSubsystem() {

  Serial.println(F("DFA2 reset subsystem")); // DEBUG

  currentState = DfaStates::Start;
  itemCost = -1;
  halted = false;
}

DfaStates VendingDfa2::CurrentState() {
  return currentState;
}


