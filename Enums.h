#ifndef ENUMS_H
#define ENUMS_H

enum class Alphabet {
  A,
  B,
  ONE,
  TWO,
  TWENTY_FIVE,
  Dollar,
  R,
  None
};

enum class TokenProgress{

  Halt,
  Steady,
  Accept,
  Reset
  
};

enum class DfaStates {
  Start,
  A,
  B,
  A_1,
  A_2,
  B_1,
  B_2
};

enum class Dispense{

  ItemOne,
  ItemTwo,
  ItemThree,
  ItemFour,
  Reset,
  Processing,
  None

};

#endif