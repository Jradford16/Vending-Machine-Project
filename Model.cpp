#include "Model.h"
#include <EEPROM.h>
#include <Arduino.h>

Model::Model(): Dfa1(prices), Dfa2(prices)
{
    for (int i = 0; i < 4; i++) {
        uint8_t val = EEPROM.read(i);

        if (val == 255) {
            prices[i] = 100;
            EEPROM.write(i, 100);
        } else {
            prices[i] = val;
        }
    }

    Serial.println(F("Model initialized")); // DEBUG
}

Dispense Model::AcceptToken(Alphabet token) {

    TokenProgress dfa1Result = Dfa1.ProgressState(token);
    TokenProgress dfa2Result = Dfa2.ProgressState(token);

    // Reset dominates
    if (dfa1Result == TokenProgress::Reset ||
        dfa2Result == TokenProgress::Reset) {

        Dfa1.ResetSubsystem();
        Dfa2.ResetSubsystem();
        return Dispense::Reset;
    }

    // DFA1 wins
    if (dfa1Result == TokenProgress::Accept &&
        dfa2Result == TokenProgress::Halt) {

        DfaStates s = Dfa1.CurrentState();

        Dfa1.ResetSubsystem();
        Dfa2.ResetSubsystem();

        return (s == DfaStates::A_1) ? Dispense::ItemOne  :
               (s == DfaStates::A_2) ? Dispense::ItemTwo  :
               (s == DfaStates::B_1) ? Dispense::ItemThree:
                                       Dispense::ItemFour;
    }

    // DFA2 wins
    if (dfa2Result == TokenProgress::Accept &&
        dfa1Result == TokenProgress::Halt) {

        DfaStates s = Dfa2.CurrentState();

        Dfa1.ResetSubsystem();
        Dfa2.ResetSubsystem();

        return (s == DfaStates::A_1) ? Dispense::ItemOne  :
               (s == DfaStates::A_2) ? Dispense::ItemTwo  :
               (s == DfaStates::B_1) ? Dispense::ItemThree:
                                       Dispense::ItemFour;
    }

    // Still processing
    if (dfa1Result == TokenProgress::Steady ||
        dfa2Result == TokenProgress::Steady) {
        return Dispense::Processing;
    }

    // Both dead
    if (dfa1Result == TokenProgress::Halt &&
        dfa2Result == TokenProgress::Halt) {

        Dfa1.ResetSubsystem();
        Dfa2.ResetSubsystem();
        return Dispense::Reset;
    }

    // Defensive fallback
    Dfa1.ResetSubsystem();
    Dfa2.ResetSubsystem();
    return Dispense::Reset;
}




void Model::Reset() {

    Serial.println(F("Model reset invoked")); // DEBUG

    Dfa1.ResetSubsystem();
    Dfa2.ResetSubsystem();
}

