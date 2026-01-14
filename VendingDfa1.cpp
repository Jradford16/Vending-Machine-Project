#include "VendingDfa1.h"
#include <Arduino.h>

VendingDfa1::VendingDfa1(int (&modelPrices)[4])
    : prices(modelPrices)
{
    currentState = DfaStates::Start;
    Serial.println(F("DFA1 initialized")); // DEBUG
}

TokenProgress VendingDfa1::ProgressState(Alphabet token) {

  Serial.print(F("DFA1 received token: ")); // DEBUG
  Serial.println((int)token);               // DEBUG

  if (token == Alphabet::R) {
    Serial.println(F("DFA1: Reset token received")); // DEBUG
    return TokenProgress::Reset;
  }

  if (!halted) {
    switch (currentState) {

      case DfaStates::Start:
        Serial.println(F("DFA1 state: Start")); // DEBUG

        if (token == Alphabet::A) {
          currentState = DfaStates::A;
          Serial.println(F("DFA1 transition: Start -> A")); // DEBUG
          return TokenProgress::Steady;
        }
        if (token == Alphabet::B) {
          currentState = DfaStates::B;
          Serial.println(F("DFA1 transition: Start -> B")); // DEBUG
          return TokenProgress::Steady;
        }
        halted = true;
        Serial.println(F("DFA1 halted at Start")); // DEBUG
        return TokenProgress::Halt;

      case DfaStates::A:
        Serial.println(F("DFA1 state: A")); // DEBUG

        if (token == Alphabet::ONE) {
          currentState = DfaStates::A_1;
          Serial.println(F("DFA1 transition: A -> A_1")); // DEBUG
          return TokenProgress::Steady;
        }
        if (token == Alphabet::TWO) {
          currentState = DfaStates::A_2;
          Serial.println(F("DFA1 transition: A -> A_2")); // DEBUG
          return TokenProgress::Steady;
        }
        halted = true;
        Serial.println(F("DFA1 halted at A")); // DEBUG
        return TokenProgress::Halt;

      case DfaStates::B:
        Serial.println(F("DFA1 state: B")); // DEBUG

        if (token == Alphabet::ONE) {
          currentState = DfaStates::B_1;
          Serial.println(F("DFA1 transition: B -> B_1")); // DEBUG
          return TokenProgress::Steady;
        }
        if (token == Alphabet::TWO) {
          currentState = DfaStates::B_2;
          Serial.println(F("DFA1 transition: B -> B_2")); // DEBUG
          return TokenProgress::Steady;
        }
        halted = true;
        Serial.println(F("DFA1 halted at B")); // DEBUG
        return TokenProgress::Halt;

      case DfaStates::A_1:
        Serial.println(F("DFA1 state: A_1")); // DEBUG

        if (token == Alphabet::TWENTY_FIVE) {
          if (itemCost == -1) {
            itemCost = prices[0];
            Serial.print(F("DFA1 A_1 price initialized: "));
            Serial.println(itemCost);
          }

          itemCost -= 25;
          Serial.print(F("DFA1 A_1 remaining cost: "));
          Serial.println(itemCost);

          if (itemCost == 0) {
            Serial.println(F("DFA1 A_1 ACCEPT")); // DEBUG
            return TokenProgress::Accept;
          }
          return TokenProgress::Steady;
        }

        halted = true;
        Serial.println(F("DFA1 halted at A_1")); // DEBUG
        return TokenProgress::Halt;

      case DfaStates::A_2:
        Serial.println(F("DFA1 state: A_2")); // DEBUG

        if (token == Alphabet::TWENTY_FIVE) {
          if (itemCost == -1) {
            itemCost = prices[1];
            Serial.print(F("DFA1 A_2 price initialized: "));
            Serial.println(itemCost);
          }

          itemCost -= 25;
          Serial.print(F("DFA1 A_2 remaining cost: "));
          Serial.println(itemCost);

          if (itemCost == 0) {
            Serial.println(F("DFA1 A_2 ACCEPT")); // DEBUG
            return TokenProgress::Accept;
          }
          return TokenProgress::Steady;
        }

        halted = true;
        Serial.println(F("DFA1 halted at A_2")); // DEBUG
        return TokenProgress::Halt;

      case DfaStates::B_1:
        Serial.println(F("DFA1 state: B_1")); // DEBUG

        if (token == Alphabet::TWENTY_FIVE) {
          if (itemCost == -1) {
            itemCost = prices[2];
            Serial.print(F("DFA1 B_1 price initialized: "));
            Serial.println(itemCost);
          }

          itemCost -= 25;
          Serial.print(F("DFA1 B_1 remaining cost: "));
          Serial.println(itemCost);

          if (itemCost == 0) {
            Serial.println(F("DFA1 B_1 ACCEPT")); // DEBUG
            return TokenProgress::Accept;
          }
          return TokenProgress::Steady;
        }

        halted = true;
        Serial.println(F("DFA1 halted at B_1")); // DEBUG
        return TokenProgress::Halt;

      case DfaStates::B_2:
        Serial.println(F("DFA1 state: B_2")); // DEBUG

        if (token == Alphabet::TWENTY_FIVE) {
          if (itemCost == -1) {
            itemCost = prices[3];
            Serial.print(F("DFA1 B_2 price initialized: "));
            Serial.println(itemCost);
          }

          itemCost -= 25;
          Serial.print(F("DFA1 B_2 remaining cost: "));
          Serial.println(itemCost);

          if (itemCost == 0) {
            Serial.println(F("DFA1 B_2 ACCEPT")); // DEBUG
            return TokenProgress::Accept;
          }
          return TokenProgress::Steady;
        }

        halted = true;
        Serial.println(F("DFA1 halted at B_2")); // DEBUG
        return TokenProgress::Halt;
    }
  }

  Serial.println(F("DFA1 fallback HALT")); // DEBUG
  return TokenProgress::Halt;
}

void VendingDfa1::ResetSubsystem() {

  Serial.println(F("DFA1 reset subsystem")); // DEBUG

  currentState = DfaStates::Start;
  itemCost = -1;
  halted = false;
}

DfaStates VendingDfa1::CurrentState() {
  return currentState;
}


