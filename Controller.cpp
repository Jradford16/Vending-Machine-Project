#include "Controller.h"
#include <Arduino.h>
#include "Pins.h"

// Servo tuning
constexpr int SERVO_ACCEPT_POS  = 30;
constexpr int SERVO_REJECT_POS  = 150;
constexpr int SERVO_NEUTRAL_POS = 90;


Controller::Controller() {
    lastBtn1 = HIGH;
    lastBtn2 = HIGH;
    lastBtn3 = HIGH;
    lastBtn4 = HIGH;
    lastBtn5 = HIGH;
}

void Controller::begin() {
  tokenServo.attach(SERVO_TOKEN_ROUTE);
  tokenServo.write(SERVO_NEUTRAL_POS);
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

void Controller::RouteTokenAccept() {
  tokenServo.write(SERVO_ACCEPT_POS);
}

void Controller::RouteTokenReject() {
  tokenServo.write(SERVO_REJECT_POS);
}

void Controller::RouteTokenNeutral() {
  tokenServo.write(SERVO_NEUTRAL_POS);
}




// Serial.available interacts with tokens currently added to the abstracted buffer, !Read Can only pull off one charecter at once!
Alphabet Controller::ReadInput() {

    // --- SERIAL INPUT (debug / testing) ---
    if (Serial.available() > 0) {

        char incoming = Serial.read();

        Serial.print(F("[SERIAL] Raw input: "));
        Serial.println(incoming);

        Alphabet token = ConvertSerialChar(incoming);

        Serial.print(F("[SERIAL] Converted token: "));
        Serial.println((int)token);

        return token;
    }

    // --- BUTTON INPUTS (edge-detected, active LOW) ---
    bool currBtn1 = digitalRead(BTN_1);
    bool currBtn2 = digitalRead(BTN_2);
    bool currBtn3 = digitalRead(BTN_3);
    bool currBtn4 = digitalRead(BTN_4);
    bool currBtn5 = digitalRead(BTN_5);

    if (lastBtn1 == HIGH && currBtn1 == LOW) {
        Serial.println(F("[BTN_1] Press -> Alphabet::A"));
        lastBtn1 = currBtn1;
        return Alphabet::A;
    }
    if (lastBtn2 == HIGH && currBtn2 == LOW) {
        Serial.println(F("[BTN_2] Press -> Alphabet::B"));
        lastBtn2 = currBtn2;
        return Alphabet::B;
    }
    if (lastBtn3 == HIGH && currBtn3 == LOW) {
        Serial.println(F("[BTN_3] Press -> Alphabet::ONE"));
        lastBtn3 = currBtn3;
        return Alphabet::ONE;
    }
    if (lastBtn4 == HIGH && currBtn4 == LOW) {
        Serial.println(F("[BTN_4] Press -> Alphabet::TWO"));
        lastBtn4 = currBtn4;
        return Alphabet::TWO;
    }
    if (lastBtn5 == HIGH && currBtn5 == LOW) {
        Serial.println(F("[BTN_5] Press -> Alphabet::R"));
        lastBtn5 = currBtn5;
        return Alphabet::R;
    }

    // update last states (no event)
    lastBtn1 = currBtn1;
    lastBtn2 = currBtn2;
    lastBtn3 = currBtn3;
    lastBtn4 = currBtn4;
    lastBtn5 = currBtn5;

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


