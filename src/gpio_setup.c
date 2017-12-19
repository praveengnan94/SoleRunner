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

	/* Initialize inputs for NFC interrupt */
	GPIO_PinModeSet(Nfc_Fd_Port, Nfc_Fd_Pin, gpioModeWiredAndPullUp, 1);	/* Input is set to be filtered */

	GPIO_IntConfig(Nfc_Fd_Port, Nfc_Fd_Pin, false, true, true);

	GPIO->INSENSE = GPIO_INSENSE_INT;				// Enable sensing interrupts on GPIO pins

	/* Enable interrupts at the CPU level */
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);					/* Nfc_Fd interrupt is on an odd pin */

}

