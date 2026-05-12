#ifndef VIEW_H
#define VIEW_H

#include "Enums.h"
#include <U8g2lib.h>

class View {
public:
  View();
  void begin();
  void drawUI(Dispense result, Alphabet token);

  // Blocking, displays messege over defined duration. Ignores input during interval, routes physically inserted tokens to return shoot.
  void showMessage(const char* msg, unsigned long durationMs);

private:
  U8G2_SSD1309_128X64_NONAME0_1_HW_I2C u8g2;

  char tokenBuffer[32];
  uint8_t tokenIndex;
  Alphabet lastToken;     

  int moneyCents = 0;

  void appendToken(Alphabet token);
};

#endif
