#ifndef GFX_H
#define GFX_H

#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include <string.h>
#include "display.h"

void Draw_Hollow_Rectangle(struct ILI9341*, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t line_width, uint16_t, uint16_t);
void Draw_Circle(struct ILI9341*);
//void drawImage();

/*
size_t tjpegd_Input(
  JDEC* jdec,       // Pointer to the decompression object 
  uint8_t* buff,    // Pointer to buffer to store the read data 
  size_t ndata      // Number of bytes to read/remove 
);

int tjpegd_Output(
  JDEC* jdec,    // Pointer to the decompression object 
  void* bitmap,  // Bitmap to be output 
  JRECT* rect    // Rectangle to output 
);
*/

void Draw_Character(struct ILI9341*, char, uint8_t[], uint8_t*, uint8_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
int LCD_Print_Text(struct ILI9341*, char*, uint8_t[], uint8_t*, uint8_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
int LCD_Print_Text_Centred(struct ILI9341 *self, char *text, uint8_t font[], uint8_t *font_widths, uint8_t font_height, uint16_t y, uint16_t size, uint16_t colour, uint16_t block_colour);
#endif