#include "measure.h"

uint32_t Ref_Resistors[REF_NUM] = {R1,R2,R3,R4,R5,R6};
uint16_t Resistor_Pins[REF_NUM] = {R1_Pin, R2_Pin, R3_Pin, R4_Pin, R5_Pin, R6_Pin};

/*
PINOUT OF THE TEST BOARD:
1 - R6 - PD6
2 - R5 - PD0
3 - R4 - PD1
4 - R3 - PD2
5 - R2 - PD3
6 - R1 - PD4
7 - VCC
8 - GND
9 - TP1 - PD5
10 - TP2 (N/C to MCU)
11 - Output - PA0
*/



//Function to set up the ADC. In theory this shouldn't be needed anymore. The MX function
//in the main file should be enough.
HAL_StatusTypeDef Setup_ADC(ADC_HandleTypeDef *ADCx, uint32_t ADC_channel, ADC_TypeDef *ADC_Instance)
{
	Pin_Mode_HighZ(ADC_Input_Port, (ADC1_Input_Pin | ADC2_Input_Pin));
	ADC_ChannelConfTypeDef sConfig = {0};
	ADCx->Instance = ADC_Instance;
	ADCx->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	ADCx->Init.Resolution = ADC_RESOLUTION_12B;
	ADCx->Init.ScanConvMode = DISABLE;
	ADCx->Init.ContinuousConvMode = DISABLE;
	ADCx->Init.DiscontinuousConvMode = DISABLE;
	ADCx->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADCx->Init.ExternalTrigConv = ADC_SOFTWARE_START;
	ADCx->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADCx->Init.NbrOfConversion = 1;
	ADCx->Init.DMAContinuousRequests = DISABLE;
	ADCx->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	HAL_StatusTypeDef result = HAL_ADC_Init(ADCx);
	if (result != HAL_OK)
	{
		return result;
	}
	sConfig.Channel = ADC_channel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	result = HAL_ADC_ConfigChannel(ADCx, &sConfig);
	return result;
}

//This function sets up the test pins for a given reference resistor.
//Pulls the resistor pin to VDD or GND, depending on direction
//Then sets TP1 to the opposite state, creating a circuit through the
//component we want to test and the reference resistor.
void Setup_Test(uint16_t ref_res, uint16_t direction)
{
	GPIO_PinState TP1_State;
	GPIO_PinState Ref_State;

	if(direction==0){
		TP1_State = GPIO_PIN_RESET;
		Ref_State = GPIO_PIN_SET;
	}else{
		TP1_State = GPIO_PIN_SET;
		Ref_State = GPIO_PIN_RESET;
	}

	//Setup GPIO

	Pin_Mode_Output(TP1_GPIO_Port, TP1_Pin);
	
	for(int i=0;i<REF_NUM;i++)
	{
		if(i==ref_res)
		{
			//Make reference resistor an output
			Pin_Mode_Output(RESISTOR_PORT, Resistor_Pins[i]);
		}else{
			//Make all other resistor pins high impedance so that they
			//don't effect the circuit.
			Pin_Mode_HighZ(RESISTOR_PORT, Resistor_Pins[i]);
		}
	}

	//Set pins to desired states.
	HAL_GPIO_WritePin(TP1_GPIO_Port, TP1_Pin, TP1_State);
	HAL_GPIO_WritePin(RESISTOR_PORT, Resistor_Pins[ref_res], Ref_State);

}

//Simple function that returns the voltage on the ADC pin.
//Returns a value from 0 to 4095, 0 being GND, 4095 being +VREF(3.3V)
uint16_t Measure_Voltage(ADC_HandleTypeDef *ADC_L)
{
	uint32_t total = 0;
	Setup_ADC(ADC_L, ADC_CHANNEL_0, ADC1);
	HAL_ADC_Start(ADC_L);
	HAL_ADC_PollForConversion(ADC_L, HAL_MAX_DELAY);
	for(int i=0;i<OVERSAMPLE_AMOUNT;i++){
		total+=HAL_ADC_GetValue(ADC_L);
	}
	HAL_ADC_Stop(ADC_L);
	return total/OVERSAMPLE_AMOUNT;
}

//Does the voltage divider equation, returns resistance as a float in ohms.
float Calc_Resistance(uint16_t ref_res, uint16_t direction, uint16_t voltage)
{
	//R_PARA_GPIO is used to remove error caused by the parasitic resistance of the GPIO
	//Measured as ~22 Ohms.
	float res;
	if(direction==0){
		res = (float)(voltage * (Ref_Resistors[ref_res] + R_PARA_GPIO)) / (float)(4095 - voltage); 
	}else{
		res = ((Ref_Resistors[ref_res] + R_PARA_GPIO)*(float)(4095-voltage)) / (float)voltage;
	}
	if(res>R_PARA_GPIO){
		res-=R_PARA_GPIO; //Remove parasitics on TP1.
	}else{
		res=0; //If our measurement is less than R_PARA_GPIO, we must have a short.
	}
	return(res);
}

// Sets pin to analog mode to make sure its floating and doesn't have a meaningful effect on the measurement.
void Pin_Mode_HighZ(GPIO_TypeDef *GPIOx, uint32_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0}; //Create a GPIO_InitStruct
	//Set our desired values for the pin setup
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG; //This is the highest impedance mode
	GPIO_InitStruct.Pull = GPIO_NOPULL; //Make sure we're not pulling the pin up or down.
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct); //Pass the struct to the HAL init function
}

// Set pin to output. Works the same way as the previous function, with different parameters.
void Pin_Mode_Output(GPIO_TypeDef *GPIOx, uint32_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

//Hacky way of printing resistances with SI units. Could probably be done much better.
void Resistance_String(char *buffer, float resistance)
{
	//@ symbol is used as placeholder for omega
	if(resistance>=900000){
		sprintf(buffer, "%.2fM@", resistance/1000000.0);
	}else{
		if(resistance>=1000){
			sprintf(buffer, "%.2fk@", (resistance/1000.0));
		}else{
		sprintf(buffer, "%.2fO@", resistance);
		}
	}
}

//Measures a resistor, returns the resistance as a float.
float Measure_Resistor(ADC_HandleTypeDef *ADCx)
{
	uint32_t v1;
	uint32_t v2;
	float currentres=0;
	uint16_t refn=0;

	//Repeat the measurement for each reference resistor
	for(int i=0;i<REF_NUM;i++)
    {
      v1=0;
      v2=0;
      Setup_Test(i, 0); //in direction 0 (TP1 to GND)
      for(int i=0;i<300;i++){
        v1+=Measure_Voltage(ADCx);
      }
      v1/=300;//Average 300 measurements. If we don't do this, result bounces around a lot.
      Setup_Test(i, 1); //in direction 1 (TP1 to VDD)
      for(int i=0;i<300;i++){
        v2+=Measure_Voltage(ADCx);
      }
      v2/=300;
      if((v1<3700) && (v1>300)){ //Exclude all large and small measurements, as these introduce the most error.
        currentres+=Calc_Resistance(i, 0, v1);
        refn++;
      }
      if((v2<3700) && (v2>300)){
        currentres+= Calc_Resistance(i, 1, v2);
        refn++;
      }
	}
	if(currentres){
		//return an average of all valid measurements.
		return currentres/refn;
	}else{
		//We were unable to get any reliable measurements. R=0 or R=inf.
		return 0.0F;
	}

}

//connects both test points to GND. (TP2 through the lowest reference, 220R)
void Discharge_Capacitor(uint16_t discharge_time){
	Pin_Mode_Output(GPIOD, TP1_Pin);
	Pin_Mode_Output(GPIOD, Resistor_Pins[0]);
	HAL_GPIO_WritePin(GPIOD, Resistor_Pins[0], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, TP1_Pin, GPIO_PIN_RESET);
	HAL_Delay(discharge_time);
	//could maybe put the pins back in HighZ when done with them? 
}

//Charges capacitor through a desired reference resisor.
void Charge_Capacitor(uint16_t ref_res, uint16_t charge_time){
	Setup_Test(ref_res, 0);
	Delay_us(charge_time*1000);
	Pin_Mode_HighZ(GPIOD, TP1_Pin);
	Pin_Mode_HighZ(GPIOD, Resistor_Pins[ref_res]);
}

//Returns capacitor as float.
float Measure_Capacitor(ADC_HandleTypeDef *ADCx)
{
	uint16_t voltage = 0;
	uint16_t charge_time = 10;
	float cap;
	Discharge_Capacitor(120);
	HAL_Delay(1);
	Charge_Capacitor(0, charge_time);
	HAL_Delay(1);//Add a delay for the op amp.
	voltage = Measure_Voltage(ADCx);
	Discharge_Capacitor(120);

	//Maths was being funky when i put it all on one line so i split it up a bit
	//should be all the same to the compiler?
	//This is a rearranged form of V(t) = V - V * exp(-t/RC)
	cap = (double)R1 * log( (1.0 - (voltage / 4095.0) )) ; 
    cap = -1/cap;
    cap = cap*charge_time*1500; //Because we multiply by charge time in us, our result is in uF.
	return cap;
}