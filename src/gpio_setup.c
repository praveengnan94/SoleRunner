/*
 * gpio_setup.c
 *
 *  Created on: Nov 16, 2017
 *      Author: Praveen
 */



#include "gpio_setup.h"

  /* Set CC0 location 3 pin (PD1) as output */
void gpio_setup()
{

	  /* Configure the drive strength of the ports for the light sensor. */

	GPIO_PinModeSet(HR_PWR_PORT, HR_PWR_PIN, gpioModePushPull, 0);
	GPIO_DriveModeSet(HR_PWR_PORT, gpioDriveModeStandard);

	//ADC heart rate
	GPIO_DriveModeSet(ADC_PORT_HR, gpioDriveModeStandard);
	GPIO_PinModeSet(ADC_PORT_HR, ADC_PIN_HR, gpioModeDisabled, 0);

  //  GPIO_PinOutSet(LEDPORT, LEDPIN0);// Added for uart BLE - because its GPIO is ON on initilization
  	  /* Enable GPIO_ODD interrupt vector in NVIC */
//  	    NVIC_EnableIRQ(GPIO_ODD_IRQn);			//GPIO IRQ in i2c_tsl2651.c

}
