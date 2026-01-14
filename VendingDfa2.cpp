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

  if (token == Alphabet::R) {
    Serial.println(F("DFA2: Reset token received")); // DEBUG
    return TokenProgress::Reset;
  }

  if (!halted) {
    if (!monetaryTokenLock) {

      if (token == Alphabet::TWENTY_FIVE) {

        monetaryInput += 25;

        Serial.print(F("DFA2 monetary input += 25, total: ")); // DEBUG
        Serial.println(monetaryInput);                         // DEBUG

        return TokenProgress::Steady;
      }

      monetaryTokenLock = true;
      Serial.println(F("DFA2 monetary token lock engaged")); // DEBUG
    }

    if (monetaryTokenLock) {
      switch (currentState) {

        case DfaStates::Start:
          Serial.println(F("DFA2 state: Start")); // DEBUG

          if (token == Alphabet::A) {
            currentState = DfaStates::A;
            Serial.println(F("DFA2 transition: Start -> A")); // DEBUG
            return TokenProgress::Steady;
          }
          if (token == Alphabet::B) {
            currentState = DfaStates::B;
            Serial.println(F("DFA2 transition: Start -> B")); // DEBUG
            return TokenProgress::Steady;
          }
          halted = true;
          Serial.println(F("DFA2 halted at Start")); // DEBUG
          return TokenProgress::Halt;

        case DfaStates::A:
          Serial.println(F("DFA2 state: A")); // DEBUG

          if (token == Alphabet::ONE && prices[0] == monetaryInput) {
            currentState = DfaStates::A_1;
            Serial.println(F("DFA2 ACCEPT A_1")); // DEBUG
            return TokenProgress::Accept;
          }
          if (token == Alphabet::TWO && prices[1] == monetaryInput) {
            currentState = DfaStates::A_2;
            Serial.println(F("DFA2 ACCEPT A_2")); // DEBUG
            return TokenProgress::Accept;
          }
          halted = true;
          Serial.println(F("DFA2 halted at A")); // DEBUG
          return TokenProgress::Halt;

        case DfaStates::B:
          Serial.println(F("DFA2 state: B")); // DEBUG

          if (token == Alphabet::ONE && prices[2] == monetaryInput) {
            currentState = DfaStates::B_1;
            Serial.println(F("DFA2 ACCEPT B_1")); // DEBUG
            return TokenProgress::Accept;
          }
          if (token == Alphabet::TWO && prices[3] == monetaryInput) {
            currentState = DfaStates::B_2;
            Serial.println(F("DFA2 ACCEPT B_2")); // DEBUG
            return TokenProgress::Accept;
          }
          halted = true;
          Serial.println(F("DFA2 halted at B")); // DEBUG
          return TokenProgress::Halt;
      }
    }
  }

  Serial.println(F("DFA2 fallback HALT")); // DEBUG
  return TokenProgress::Halt;
}

void VendingDfa2::ResetSubsystem() {

  Serial.println(F("DFA2 reset subsystem")); // DEBUG

  currentState = DfaStates::Start;
  halted = false;
  monetaryTokenLock = false;
  monetaryInput = 0;
}

DfaStates VendingDfa2::CurrentState() {
  return currentState;
}

