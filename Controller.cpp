#include "Controller.h"
#include <Arduino.h>
#include "Pins.h"


Controller::Controller() {
}

void Controller::HandleResult(Dispense result) {

    Serial.print(F("Controller received result: ")); // DEBUG
    Serial.println((int)result);                      // DEBUG

    // Will activate digital pins corresponding to which motor is attached to which item.
    // Will also activate digital pins corresponding to electromagnets leading to individual coin collection chambers.

    // * For each motor activation, the following three must be activated. 
    // 1. Recieving Stndy pin on driver must be High via arduino digital pin.
    // 2. Pwm A and Pwm B pins on driver must be on High. (Circuit has them directly pulled to 5v)
    // 3. Ain or bin "exlusive" must be on high.

    switch (result) {

        case Dispense::ItemOne:
            Serial.println(F("Controller: Dispense ItemOne"));

            digitalWrite(D1_STBY, HIGH);
            digitalWrite(D1_AIN1, HIGH);

            delay(3000);

            digitalWrite(D1_AIN1, LOW);
            digitalWrite(D1_STBY, LOW);
            break;

        case Dispense::ItemTwo:
            Serial.println(F("Controller: Dispense ItemTwo"));

            digitalWrite(D1_STBY, HIGH);
            digitalWrite(D1_BIN1, HIGH);

            delay(3000);

            digitalWrite(D1_BIN1, LOW);
            digitalWrite(D1_STBY, LOW);
            break;

        case Dispense::ItemThree:
            Serial.println(F("Controller: Dispense ItemThree"));

            digitalWrite(D2_STBY, HIGH);
            digitalWrite(D2_AIN1, HIGH);

            delay(3000);

            digitalWrite(D2_AIN1, LOW);
            digitalWrite(D2_STBY, LOW);
            break;

        case Dispense::ItemFour:
            Serial.println(F("Controller: Dispense ItemFour"));

            digitalWrite(D2_STBY, HIGH);
            digitalWrite(D2_BIN1, HIGH);

            delay(3000);

            digitalWrite(D2_BIN1, LOW);
            digitalWrite(D2_STBY, LOW);
            break;

        case Dispense::Reset:
            Serial.println(F("Controller: Reset"));

            digitalWrite(D1_AIN1, LOW);
            digitalWrite(D1_BIN1, LOW);
            digitalWrite(D1_STBY, LOW);

            digitalWrite(D2_AIN1, LOW);
            digitalWrite(D2_BIN1, LOW);
            digitalWrite(D2_STBY, LOW);
            break;
    }


}


// Serial.available interacts with tokens currently added to the abstracted buffer, !Read Can only pull off one charecter at once!
Alphabet Controller::ReadInput() {

    if (Serial.available() > 0) {

        char incoming = Serial.read();

        Serial.print(F("Raw serial input: ")); // DEBUG
        Serial.println(incoming);              // DEBUG

        Alphabet token = ConvertSerialChar(incoming);

        Serial.print(F("Converted token: "));  // DEBUG
        Serial.println((int)token);             // DEBUG

        return token;
    }

    return Alphabet::None;
}


Alphabet Controller::ConvertSerialChar(char c) {
    switch (c) {
        case 'a': return Alphabet::A;
        case 'b': return Alphabet::B;
        case '1': return Alphabet::ONE;
        case '2': return Alphabet::TWO;
        case '5': return Alphabet::TWENTY_FIVE;
        case 'd': return Alphabet::Dollar;
        case 'r': return Alphabet::R;
        default:  return Alphabet::None;
    }
}


