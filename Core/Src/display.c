#include "display.h"


//Creates display object
struct ILI9341 *ILI9341_Create(SPI_HandleTypeDef *SPIx, UART_HandleTypeDef *UARTx, uint16_t width, uint16_t height)
{
	struct ILI9341 *display = malloc(sizeof(struct ILI9341));
	if (display == NULL)
	{
		return NULL;
	}
	display->SPIHandle = SPIx;
	display->x_size = width;
	display->y_size = height;
	display->UARTHandle = UARTx;
	return (display);
}

void ILI9341_Destroy(struct ILI9341 *self)
{
	free(self);
}

void Print_Debug_Message(struct ILI9341 *self, char *message)
{
	HAL_UART_Transmit(self->UARTHandle, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
	HAL_UART_Transmit(self->UARTHandle, (uint8_t *)"\n\r", strlen("\n\r"), HAL_MAX_DELAY);
}

//Send 8 bits to the display
void ILI9341_Write_8(struct ILI9341 *self, uint8_t dat)
{
	HAL_SPI_Transmit(self->SPIHandle, (uint8_t *)&dat, 1, HAL_MAX_DELAY);
}

//Sent 
void ILI9341_Write_16(struct ILI9341 *self, uint16_t dat)
{
	// Flip the bytes to fix colour problems
	dat = (((dat & 0x00FF) << 8) | ((dat & 0xFF00) >> 8));
	HAL_SPI_Transmit(self->SPIHandle, (uint8_t *)&dat, 2, HAL_MAX_DELAY);
}

//Pull DC Pin LOW when writing a command to the display
void ILI9341_Write_Command(struct ILI9341 *self, uint8_t cmd)
{
	HAL_GPIO_WritePin(SPI_TFT_DC_GPIO_Port, SPI_TFT_DC_Pin, GPIO_PIN_RESET);
	ILI9341_Write_8(self, cmd);
	HAL_GPIO_WritePin(SPI_TFT_DC_GPIO_Port, SPI_TFT_DC_Pin, GPIO_PIN_SET);
}


//This initialises the display on power up.
//TO DO- Send sequences of multiple bytes with HAL_SPI_Transmit() to clean up the code a little
void ILI9341_Init(struct ILI9341 *self)
{
	// Pull CS pin high.
	HAL_GPIO_WritePin(SPI_TFT_CS_GPIO_Port, SPI_TFT_CS_Pin, GPIO_PIN_SET);

	// Pull DC pin high for now.
	HAL_GPIO_WritePin(SPI_TFT_DC_GPIO_Port, SPI_TFT_DC_Pin, GPIO_PIN_SET);

	// Set SCK to be high.
	HAL_GPIO_WritePin(SPI1_TFT_SCK_GPIO_Port, SPI1_TFT_SCK_Pin, GPIO_PIN_SET);

	// Reset the display
	HAL_GPIO_WritePin(SPI_TFT_RESET_GPIO_Port, SPI_TFT_RESET_Pin, GPIO_PIN_RESET);

	HAL_Delay(200);

	HAL_GPIO_WritePin(SPI_TFT_RESET_GPIO_Port, SPI_TFT_RESET_Pin, GPIO_PIN_SET);

	HAL_Delay(200);

	HAL_GPIO_WritePin(SPI_TFT_CS_GPIO_Port, SPI_TFT_CS_Pin, GPIO_PIN_RESET);

	HAL_Delay(200);
	// Alternative init sequence
	// SOFTWARE RESET
	ILI9341_Write_Command(self, 0x01);
	HAL_Delay(1000);

	// POWER CONTROL A
	ILI9341_Write_Command(self, 0xCB);
	ILI9341_Write_8(self, 0x39);
	ILI9341_Write_8(self, 0x2C);
	ILI9341_Write_8(self, 0x00);
	ILI9341_Write_8(self, 0x34);
	ILI9341_Write_8(self, 0x02);

	// POWER CONTROL B
	ILI9341_Write_Command(self, 0xCF);
	ILI9341_Write_8(self, 0x00);
	ILI9341_Write_8(self, 0xC1);
	ILI9341_Write_8(self, 0x30);

	// DRIVER TIMING CONTROL A
	ILI9341_Write_Command(self, 0xE8);
	ILI9341_Write_8(self, 0x85);
	ILI9341_Write_8(self, 0x00);
	ILI9341_Write_8(self, 0x78);

	// DRIVER TIMING CONTROL B
	ILI9341_Write_Command(self, 0xEA);
	ILI9341_Write_8(self, 0x00);
	ILI9341_Write_8(self, 0x00);

	// POWER ON SEQUENCE CONTROL
	ILI9341_Write_Command(self, 0xED);
	ILI9341_Write_8(self, 0x64);
	ILI9341_Write_8(self, 0x03);
	ILI9341_Write_8(self, 0x12);
	ILI9341_Write_8(self, 0x81);

	// PUMP RATIO CONTROL
	ILI9341_Write_Command(self, 0xF7);
	ILI9341_Write_8(self, 0x20);

	// POWER CONTROL,VRH[5:0]
	ILI9341_Write_Command(self, 0xC0);
	ILI9341_Write_8(self, 0x23);

	// POWER CONTROL,SAP[2:0];BT[3:0]
	ILI9341_Write_Command(self, 0xC1);
	ILI9341_Write_8(self, 0x10);

	// VCM CONTROL
	ILI9341_Write_Command(self, 0xC5);
	ILI9341_Write_8(self, 0x3E);
	ILI9341_Write_8(self, 0x28);

	// VCM CONTROL 2
	ILI9341_Write_Command(self, 0xC7);
	ILI9341_Write_8(self, 0x86);

	// MEMORY ACCESS CONTROL
	ILI9341_Write_Command(self, 0x36);
	ILI9341_Write_8(self, 0x48);

	// PIXEL FORMAT
	ILI9341_Write_Command(self, 0x3A);
	ILI9341_Write_8(self, 0x55);

	// FRAME RATIO CONTROL, STANDARD RGB COLOR
	ILI9341_Write_Command(self, 0xB1);
	ILI9341_Write_8(self, 0x00);
	ILI9341_Write_8(self, 0x18);

	// DISPLAY FUNCTION CONTROL
	ILI9341_Write_Command(self, 0xB6);
	ILI9341_Write_8(self, 0x08);
	ILI9341_Write_8(self, 0x82);
	ILI9341_Write_8(self, 0x27);

	// 3GAMMA FUNCTION DISABLE
	ILI9341_Write_Command(self, 0xF2);
	ILI9341_Write_8(self, 0x00);

	// GAMMA CURVE SELECTED
	ILI9341_Write_Command(self, 0x26);
	ILI9341_Write_8(self, 0x01);

	// POSITIVE GAMMA CORRECTION
	ILI9341_Write_Command(self, 0xE0);
	ILI9341_Write_8(self, 0x0F);
	ILI9341_Write_8(self, 0x31);
	ILI9341_Write_8(self, 0x2B);
	ILI9341_Write_8(self, 0x0C);
	ILI9341_Write_8(self, 0x0E);
	ILI9341_Write_8(self, 0x08);
	ILI9341_Write_8(self, 0x4E);
	ILI9341_Write_8(self, 0xF1);
	ILI9341_Write_8(self, 0x37);
	ILI9341_Write_8(self, 0x07);
	ILI9341_Write_8(self, 0x10);
	ILI9341_Write_8(self, 0x03);
	ILI9341_Write_8(self, 0x0E);
	ILI9341_Write_8(self, 0x09);
	ILI9341_Write_8(self, 0x00);

	// NEGATIVE GAMMA CORRECTION
	ILI9341_Write_Command(self, 0xE1);
	ILI9341_Write_8(self, 0x00);
	ILI9341_Write_8(self, 0x0E);
	ILI9341_Write_8(self, 0x14);
	ILI9341_Write_8(self, 0x03);
	ILI9341_Write_8(self, 0x11);
	ILI9341_Write_8(self, 0x07);
	ILI9341_Write_8(self, 0x31);
	ILI9341_Write_8(self, 0xC1);
	ILI9341_Write_8(self, 0x48);
	ILI9341_Write_8(self, 0x08);
	ILI9341_Write_8(self, 0x0F);
	ILI9341_Write_8(self, 0x0C);
	ILI9341_Write_8(self, 0x31);
	ILI9341_Write_8(self, 0x36);
	ILI9341_Write_8(self, 0x0F);

	// EXIT SLEEP
	ILI9341_Write_Command(self, 0x11);
	HAL_Delay(120);

	// TURN ON DISPLAY
	ILI9341_Write_Command(self, 0x29);

	// STARTING ROTATION

	Set_Display_Rotation(self, 0);
}


void Set_Display_Rotation(struct ILI9341 *self, uint8_t rotation)
{
	ILI9341_Write_Command(self, 0x36);
	HAL_Delay(1);
	switch (rotation)
	{
	case 1: // vertical 1
		ILI9341_Write_8(self, 0x40 | 0x08);
		self->x_size = 240;
		self->y_size = 320;
		break;
	case 2: // vertical 2
		ILI9341_Write_8(self, 0x80 | 0x08);
		self->x_size = 240;
		self->y_size = 320;
		break;
	case 3: // horizontal 2
		ILI9341_Write_8(self, 0x40 | 0x80 | 0x20 | 0x08);
		self->x_size = 320;
		self->y_size = 240;
		break;
	case 0: // horizontal 1
		ILI9341_Write_8(self, 0x20 | 0x08);
		self->x_size = 320;
		self->y_size = 240;
		break;
	}
}

//Defines a rectangular area to read/write from.
void Set_Address(struct ILI9341 *self, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
	ILI9341_Write_Command(self, 0x2A);
	ILI9341_Write_8(self, X1 >> 8);
	ILI9341_Write_8(self, X1);
	ILI9341_Write_8(self, X2 >> 8);
	ILI9341_Write_8(self, X2);

	ILI9341_Write_Command(self, 0x2B);
	ILI9341_Write_8(self, Y1 >> 8);
	ILI9341_Write_8(self, Y1);
	ILI9341_Write_8(self, Y2 >> 8);
	ILI9341_Write_8(self, Y2);
}

//Sets the address, then tells the display we are writing to it
void Set_Address_Write(struct ILI9341 *self, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
	Set_Address(self, X1, Y1, X2, Y2);
	ILI9341_Write_Command(self, 0x2C);
}

//Read a set area out of the display's memory
void Set_Address_Read(struct ILI9341 *self, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
	Set_Address(self, X1, Y1, X2, Y2);
	ILI9341_Write_Command(self, 0x2E);
}

//Fills the display with set colour
void Fill_Display(struct ILI9341 *self, uint16_t colour)
{
	colour = (((colour & 0x00FF) << 8) | ((colour & 0xFF00) >> 8)); //Flip the bytes in the colour
	uint16_t *line = malloc(sizeof(uint16_t) * (self->x_size)); //Create a buffer for one line
	for (int i = 0; i < self->x_size; i++)
	{
		line[i] = colour;//fill the buffer with given colour
	}
	for (int i = 0; i < self->y_size; i++)//Fill display line by line
	{
		Set_Address_Write(self, 0, i, self->x_size, i + 1);
		HAL_SPI_Transmit(self->SPIHandle, (uint8_t *)line, sizeof(uint16_t) * self->x_size, HAL_MAX_DELAY);
	}
	free(line);
}

//Converts an RGB value (max 31,63,31) to an integer that can be sent to the display
uint16_t RGB_to_HEX(int r, int g, int b)
{
	return ((r & 0x1F) << 11) + ((g & 0x3F) << 5) + (b & 0x1F);
}


//Fills an area with a solid colour. Currently does this pixel by pixel, which is quite slow.
//TO DO- draw line by line or buffer entire area, depending on size.
void Fill_Area(struct ILI9341 *self, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
	Set_Address_Write(self, x1, y1, x2 - 1, y2 - 1);
	for (int i = 0; i < ((x2 - x1) * (y2 - y1)); i++)
	{
		ILI9341_Write_16(self, colour);
	}
}

//Grabs an area of the display and buffers it.
//Doesn't really work yet.
uint16_t *Buffer_Area(struct ILI9341 *self, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t buffer_size = sizeof(uint16_t) * (x2 - x1) * (y2 - y1);
	Set_Address_Read(self, x1, y1, x2, y2);
	self->buffer = malloc(buffer_size);
	if (self->buffer == NULL)
	{
		Print_Debug_Message(self, "Failled to allocate buffer!");
		return NULL;
	}
	Set_Address_Read(self, x1, y1, x2, y2);
	HAL_SPI_Receive(self->SPIHandle, (uint8_t *)self->buffer, buffer_size, HAL_MAX_DELAY);
	return (self->buffer);
}