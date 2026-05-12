#include "View.h"
#include <Wire.h>

View::View()
  : u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE),
    tokenIndex(0),
    lastToken(Alphabet::None)
{
  tokenBuffer[0] = '\0';
}

// hardware init (safe)
void View::begin() {
  Wire.begin();
  u8g2.begin();
}

void View::appendToken(Alphabet token) {
  if (tokenIndex >= sizeof(tokenBuffer) - 6) return;

  const char* str = "";

  switch (token) {
    case Alphabet::A:            str = "A "; break;
    case Alphabet::B:            str = "B "; break;
    case Alphabet::ONE:          str = "1 "; break;
    case Alphabet::TWO:          str = "2 "; break;
    case Alphabet::TWENTY_FIVE:  str = "$0.25 "; break;
    case Alphabet::Dollar:       str = "$1.00 "; break;
    case Alphabet::R:            str = "R "; break;
    case Alphabet::None:
    default:
      return;
  }

  while (*str && tokenIndex < sizeof(tokenBuffer) - 1) {
    tokenBuffer[tokenIndex++] = *str++;
  }
  tokenBuffer[tokenIndex] = '\0';
}

// NOTE:
// Edge detection here is TEMPORARY.
// This exists only to prevent duplicate input caused by
// polling-level button reads / serial testing.
// Once real scanner / coin hardware is integrated and
// input becomes event-based, this logic should be removed
// and handled in the Controller instead.

void View::drawUI(Dispense result, Alphabet token) {

  if (token == Alphabet::None) {
    lastToken = Alphabet::None;}

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x12_tr);

    switch (result) {

      case Dispense::ItemOne:
        u8g2.drawStr(0, 12, "Item 1 Selected");
        u8g2.drawStr(0, 28, "Dispensing...");
        break;

      case Dispense::ItemTwo:
        u8g2.drawStr(0, 12, "Item 2 Selected");
        u8g2.drawStr(0, 28, "Dispensing...");
        break;

      case Dispense::ItemThree:
        u8g2.drawStr(0, 12, "Item 3 Selected");
        u8g2.drawStr(0, 28, "Dispensing...");
        break;

      case Dispense::ItemFour:
        u8g2.drawStr(0, 12, "Item 4 Selected");
        u8g2.drawStr(0, 28, "Dispensing...");
        break;

      case Dispense::Processing:

        // Quarter input (edge-triggered, cumulative)
        if (token == Alphabet::TWENTY_FIVE && token != lastToken) {
          moneyCents += 25;

          snprintf(tokenBuffer, sizeof(tokenBuffer),
                  "$%d.%02d", moneyCents / 100, moneyCents % 100);

          lastToken = token;
        }

        // Other tokens (edge-triggered, symbolic)
        else if (token != Alphabet::None && token != lastToken) {
          appendToken(token);
          lastToken = token;
        }

        u8g2.drawStr(0, 12, "Input:");
        u8g2.drawStr(0, 28, tokenBuffer);
        break;

      case Dispense::Reset:
        // Reset View-local state
        moneyCents = 0;
        tokenIndex = 0;
        tokenBuffer[0] = '\0';
        lastToken = Alphabet::None;
        break;

    }

  } while (u8g2.nextPage());
}

void View::showMessage(const char* msg, unsigned long durationMs) {
  unsigned long start = millis();

  while (millis() - start < durationMs) {

    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_6x12_tr);

      // Center-ish placement, tweak if you want
      u8g2.drawStr(0, 24, msg);

    } while (u8g2.nextPage());
  }
}

