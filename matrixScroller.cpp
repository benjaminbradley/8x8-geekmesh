#include "matrixScroller.h"

#define CHAR_WIDTH 3
#define CHAR_HEIGHT 5

MatrixScroller::MatrixScroller(String message_) {
  message = message_;
  position = 0;
  color = 0xffffff;
}

uint16_t MatrixScroller::getPosition() {
  return position;
}

void MatrixScroller::setColor(uint32_t color_) {
  color = color_;
}

void MatrixScroller::draw(RGBMatrix &matrix)
{
  uint16_t messageIndex;
  uint8_t charIndex;

  for(uint8_t x = 0; x < LED_COLS; x++) {
    messageIndex = (position+x) / (CHAR_WIDTH+1); // character count within message
    charIndex = (position+x) % (CHAR_WIDTH+1);    // column within character bitmap
    // get the next character in the message
    uint8_t charValue = message[messageIndex];
    // get the bitmap for this character
    uint8_t* char_bitmap;
    if(charValue >= ' ' && charValue <= 127) {
      uint8_t charIntValue = charValue - ' ';   // integer value for the number
      char_bitmap = *bitmap_ascii[charIntValue];
    } else {
      // draw a question mark
      char_bitmap = *bitmap_ascii[31];
    }
    
    for(uint8_t y = 0; y < LED_ROWS; y++) {
      if(charIndex == CHAR_WIDTH  // this is the spacer in between characters
        || y >= CHAR_HEIGHT)      // rows below the character
      {
        matrix.set(y, x, 0,0,0);
      } else {
        uint8_t onoff = char_bitmap[y*3 + charIndex];
        if(onoff == 1) {
          matrix.set(y, x, color);
        } else {
          matrix.set(y, x, 0,0,0);
        }
      }
    }
  }

  matrix.show();
  
  position++;
  // check for wrap-around
  if(position >= (message.length()+1)*(CHAR_WIDTH+1)) {
    position = 0;
  }
};
