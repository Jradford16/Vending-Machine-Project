#include "Controller.h"
#include <Arduino.h>


Controller::Controller() {

}

void Controller::HandleResult(Dispense result) {

    // Will activate digital pins corresponding to which motor is attached to which item.
    // Will also activate digital pins corresponding to electromagnets leading to individual coin collection chambers.

    // * For each motor activation, the following three must be activated. 
    // 1. Recieving Stndy pin on driver must be High via arduino digital pin.
    // 2. Pwm A and Pwm B pins on driver must be on High. (Circuit has them directly pulled to 5v)
    // 3. Ain or bin "exlusive" must be on high.

    switch (result) {

        case Dispense::ItemOne:

            // Motor logic for ItemOne

            // STBY HIGH
            digitalWrite(3, HIGH); // STBY
            // Direction pins set
            digitalWrite(2, HIGH); // AIN1 (direction)
            digitalWrite(4, LOW);  // BIN1 or paired pin LOW

            // delay(time_for_vend);

            // STBY Low
            digitalWrite(3, LOW);

            // AIN1 direction pin low
            digitalWrite(2, LOW);

            // Electro magnet component -
            

            break;

        case Dispense:: ItemTwo:
            // Motor logic for ItemTwo
            // STBY HIGH
            digitalWrite(3, HIGH); // STBY
            // Direction pins set
            digitalWrite(4,HIGH); // BIN1 set to rotate
            digitalWrite(2, LOW); // AIN1 set to stop

            // delay(time_for_vend);

            // STBY Low
            digitalWrite(3,LOW);

            // BIN1 direction pin low
            digitalWrite(4,LOW);

            // Electro magnet component -


            break;

        case Dispense:: ItemThree:

            // Motor logic for ItemThree

            // STBY HIGH
            digitalWrite(6, HIGH); // STBY
            // Direction pins set
            digitalWrite(5, HIGH); // AIN1 (direction)
            digitalWrite(7, LOW);  // BIN1 or paired pin LOW

            // delay(time_for_vend);

            // STBY Low
            digitalWrite(6, LOW);

            // AIN1 direction pin low
            digitalWrite(5, LOW);

            // Electro magnet component -

            break;

        case Dispense:: ItemFour:

            // Motor logic for ItemFour

            // STBY HIGH
            digitalWrite(6, HIGH); // STBY
            // Direction pins set
            digitalWrite(7, HIGH); // BIN1 set to rotate
            digitalWrite(5, LOW);  // AIN1 set to stop

            // delay(time_for_vend);

            // STBY Low
            digitalWrite(6, LOW);

            // BIN1 direction pin low
            digitalWrite(7, LOW);

            // Electro magnet component -

            break;
        
        case Dispense:: Reset:


            // Disable motors / reset state
            // STBY LOW, PWM LOW, direction pins LOW
            break;

}


  
}



// Serial.available interacts with tokens currently added to the abstracted buffer, !Read Can only pull off one charecter at once!
Alphabet Controller::ReadInput() {
    // 
    if (Serial.available() > 0) {

        char incoming = Serial.read();

        // Convert a single typed char into an Alphabet token
        return ConvertSerialChar(incoming);
    }

    return Alphabet::None; // nothing received
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




