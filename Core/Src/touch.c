#include "touch.h"

#define READ_X 0xD0
#define READ_Y 0x90

#define MIN_RAW_X 2000
#define MAX_RAW_X 30000
#define MIN_RAW_Y 3300
#define MAX_RAW_Y 30000


//Create touchscreen object
struct XPT2046* XPT2046_Create(uint16_t width, uint16_t height, SPI_HandleTypeDef *SPIx, UART_HandleTypeDef *UARTx)
{
	struct XPT2046* ts = malloc(sizeof(struct XPT2046));
	if(ts==NULL){
		return NULL;
	}
	ts->SPIHandle = SPIx;
	ts->UARTHandle = UARTx;
	ts->width = width;
	ts->height = height;
	return ts;
}

void XPT2046_Destroy(struct XPT2046 *ts)
{
	free(ts);
}

//Check if the TS_IRQ pin is low, hence checking if the screen has been pressed.
int TS_is_Pressed(){
	if(HAL_GPIO_ReadPin(SPI2_TS_IRQ_GPIO_Port, SPI2_TS_IRQ_Pin) == GPIO_PIN_RESET){
		return 1;
	}
	return -1;
}

//Updates the display's last x,y touch value.
int TS_Get_Touch(struct XPT2046 *self){
	TS_Chip_Enable();
	uint32_t total_x = 0;
	uint32_t total_y = 0;
	uint16_t avg_x;
	uint16_t avg_y;
	uint8_t samples = 0;
	uint8_t read_x = READ_X;
	uint8_t read_y = READ_Y;
	uint8_t zeroes[] = {0x00,0x00};

	//Sample the display 32 times. Sample size can be increased to help with accuracy.
	for(int i=0;i<32;i++){

		if(TS_is_Pressed()!=1)//Essentially debouncing. If the screen is no longer being touched, stop sampling.
			break;

		uint8_t raw_x[2];
		uint8_t raw_y[2];

		samples++;
		HAL_SPI_Transmit(self->SPIHandle, &read_x, sizeof(uint8_t), HAL_MAX_DELAY);
		HAL_SPI_TransmitReceive(self->SPIHandle, zeroes, raw_x, sizeof(uint16_t), HAL_MAX_DELAY);//Get x value

		HAL_SPI_Transmit(self->SPIHandle, &read_y, sizeof(uint8_t), HAL_MAX_DELAY);
		HAL_SPI_TransmitReceive(self->SPIHandle, zeroes, raw_y, sizeof(uint16_t), HAL_MAX_DELAY);//Get y value

		total_x += ( ((uint16_t)raw_x[0] << 8) | ((uint16_t)raw_x[1]) );//Flip the bytes of the recieved data
		total_y += ( ((uint16_t)raw_y[0] << 8) | ((uint16_t)raw_y[1]) );
	}

	TS_Chip_Disable();

	if(samples<32){
		return -1;//The touch was released while sampling. Return error
	}

	avg_x = total_x / 32;
	avg_y = total_y / 32;

	/*
	char buffer[50];
	sprintf(buffer, "Touch at %d , %d \n\r", avg_x, avg_y);
    HAL_UART_Transmit(self->UARTHandle, (unsigned char *)buffer, strlen(buffer), HAL_MAX_DELAY);

	DEBUG/Calibration
	*/
	

	//Stops us from getting a value outside the display area.
	if(avg_x < MIN_RAW_X) avg_x = MIN_RAW_X;
	if(avg_x > MAX_RAW_X) avg_x = MAX_RAW_X;
	if(avg_y < MIN_RAW_Y) avg_y = MIN_RAW_Y;
	if(avg_y > MAX_RAW_Y) avg_y = MAX_RAW_Y;

	//Calculate coordinates from the touch values
	//flips X/Y due to being in landscape.
	self->last_y = 240 - ((avg_x - MIN_RAW_X) * 240 / (MAX_RAW_X - MIN_RAW_X));
	self->last_x = 320 - ((avg_y - MIN_RAW_Y) * 320 / (MAX_RAW_Y - MIN_RAW_Y));

	return 1;
}

//Toggles CS pin.

void TS_Chip_Enable(){
	HAL_GPIO_WritePin(SPI2_TS_CS_GPIO_Port, SPI2_TS_CS_Pin, GPIO_PIN_RESET);
}

void TS_Chip_Disable(){
	HAL_GPIO_WritePin(SPI2_TS_CS_GPIO_Port, SPI2_TS_CS_Pin, GPIO_PIN_SET);
}