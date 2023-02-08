#include "GFX.h"

int LCD_Print_Text(struct ILI9341 *self, char *text, uint8_t font[], uint8_t *font_widths, uint8_t font_height, uint16_t x, uint16_t y, uint16_t size, uint16_t colour, uint16_t block_colour)
{
	int current_X = x;
	int current_Y = y;
	for (int i = 0; i < strlen(text); i++)//Iterate character by character
	{
		if (text[i] == '\n')//detect newline
		{
			current_Y += font_height * size; //Go a line down
			current_X = x; //reset the x position
		}
		else
		{
			Draw_Character(self, text[i], font, font_widths, font_height, current_X, current_Y, size, colour, block_colour);
			//Variable width fonts require us to check width of each letter to increment the X.
			current_X = current_X + font_widths[text[i] - 32] * size + size;
		}
	}
	return current_X;
}

void Draw_Character(struct ILI9341 *self, char character, uint8_t font[], uint8_t *font_widths, uint8_t font_height, uint16_t x, uint16_t y, uint16_t scale, uint16_t colour, uint16_t block_colour)
{
	colour = (((colour & 0x00FF) << 8) | ((colour & 0xFF00) >> 8)); //flip the bytes in the colour

	//Since the font file is just a long array of uint8s, we need to find the index of the desired letter.
	int fontIndex=0;
	for (int i = 0; i < (character - 32); i++) //the start from ascii 32, so we do char-32.
	{
		fontIndex += (((font_widths[i] + 7) / 8) * font_height); //(font_widths[i] + 7) / 8) == width in bytes
	}

	int characterWidth = font_widths[character - 32];	// character width in bits (pixels)
	int lineWidth = (characterWidth+7) / 8;				// character width in bytes

	int xPos = 0;
	int yPos = 0;
	
	uint16_t *videoBuffer = calloc(characterWidth*font_height, sizeof(uint16_t)); //Allocate a buffer

	/*
	ADD CHECK FOR CALLOC SUCCESS HERE
	*/

	for (int n = 0; n < font_height; n++)
	{ // iterate line by line
			for (int j = 0; j < characterWidth; j++)//BIT BY BIT
			{
				if (font[fontIndex + n*lineWidth + j/8] & (1 << (7-(j%8)))) //checks if bit is set
				{
					videoBuffer[(n*characterWidth)+j]=colour; //write to the buffer
				}else{
					videoBuffer[(n*characterWidth)+j]=block_colour;
				}
			}
	}
	Set_Address_Write(self, x, y, x+characterWidth-1, y+font_height);
	HAL_SPI_Transmit(self->SPIHandle, ( uint8_t *)videoBuffer, characterWidth*font_height*sizeof(uint16_t), HAL_MAX_DELAY);
	free(videoBuffer);
}

void Draw_Hollow_Rectangle(struct ILI9341* self, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t line_width, uint16_t colour, uint16_t block_colour)
{
	Fill_Area(self, x1, y1, x2, y2, colour);
	Fill_Area(self, x1+line_width, y1+line_width, x2-line_width, y2-line_width, block_colour);
}


/*

FOR TJPEGD,  DOESNT REALLY WORK PROPERLY AT THE MOMENT

size_t tjpegd_Input(JDEC* jdec, uint8_t* buff, size_t ndata)
{
	if((data_pos + ndata) > image_size) ndata = image_size - data_pos;
  	if(buff) {
		memcpy(buff, (const uint8_t *)(earthbmp + data_pos), ndata);
	}
  	data_pos += ndata;
  	return ndata;
}

int tjpegd_Output(JDEC* jdec, void* bitmap, JRECT* rect){
	if((SPI_global == NULL) || (UART_global ==NULL)){
		return 0;
	}
	char buf[50];
	uint32_t rectangle_size = (rect->right - rect->left) * (rect->bottom - rect->top);
	sprintf(buf, "Drawing at %d,%d to %d,%d\n\r", rect->left, rect->top, rect->right, rect->bottom);
	HAL_UART_Transmit(UART_global, buf, strlen(buf), HAL_MAX_DELAY);

	Set_Address_Write(LCD, rect->left, rect->top, rect->right, rect->bottom);
	HAL_SPI_Transmit(LCD->SPIHandle, bitmap, rectangle_size, HAL_MAX_DELAY);
	return 1;
}

void drawImage(){
	JDEC jdec;
	void* workArea = malloc(3096);
	data_pos = 0;

	if(workArea == NULL){
		HAL_UART_Transmit(UART_global, "Houston, we have problem\n\r", strlen("Houston, we have problem\n\r"), HAL_MAX_DELAY);
	}
	JRESULT result = jd_prepare(&jdec, tjpegd_Input, workArea, 3096, NULL);
	if(result == JDR_OK){
		HAL_UART_Transmit(UART_global, "I AM TOO GOOD\n\r", strlen("I AM TOO GOOD\n\r"), HAL_MAX_DELAY);
	}else if(result == JDR_INP){
		HAL_UART_Transmit(UART_global, "error INP\n\r", strlen("error INP\n\r"), HAL_MAX_DELAY);
	}else if(result == JDR_MEM1){
		HAL_UART_Transmit(UART_global, "error MEM1\n\r", strlen("error INP1\n\r"), HAL_MAX_DELAY);
	}else if(result == JDR_MEM2){
		HAL_UART_Transmit(UART_global, "error MEM2\n\r", strlen("error INP1\n\r"), HAL_MAX_DELAY);
	}else if(result == JDR_FMT1){
		HAL_UART_Transmit(UART_global, "error FMT1\n\r", strlen("error INP1\n\r"), HAL_MAX_DELAY);
	}else if(result == JDR_FMT3){
		HAL_UART_Transmit(UART_global, "error FMT2\n\r", strlen("error INP1\n\r"), HAL_MAX_DELAY);
	}
	char printbuf[50];
	sprintf(printbuf, "image size %d, %d found\n\r", jdec.width, jdec.height);
	HAL_UART_Transmit(UART_global, printbuf, strlen(printbuf), HAL_MAX_DELAY);

	result = jd_decomp(&jdec, tjpegd_Output, 0);

	if(result == JDR_OK){
		HAL_UART_Transmit(UART_global, "I AM TOO GOOD\n\r", strlen("I AM TOO GOOD\n\r"), HAL_MAX_DELAY);
	}else if(result == JDR_INP){
		HAL_UART_Transmit(UART_global, "error INP\n\r", strlen("error INP\n\r"), HAL_MAX_DELAY);
	}else if(result == JDR_MEM1){
		HAL_UART_Transmit(UART_global, "error MEM1\n\r", strlen("error INP1\n\r"), HAL_MAX_DELAY);
	}else if(result == JDR_MEM2){
		HAL_UART_Transmit(UART_global, "error MEM2\n\r", strlen("error INP1\n\r"), HAL_MAX_DELAY);
	}else if(result == JDR_FMT1){
		HAL_UART_Transmit(UART_global, "error FMT1\n\r", strlen("error INP1\n\r"), HAL_MAX_DELAY);
	}else if(result == JDR_FMT3){
		HAL_UART_Transmit(UART_global, "error FMT2\n\r", strlen("error INP1\n\r"), HAL_MAX_DELAY);
	}
}
*/