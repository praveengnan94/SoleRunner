/*
 * main.c
 *
 *  Created on: Nov 16, 2017
 *      Author: Praveen
 */

#include "includes.h"

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();
  CMU_setup();	//SET UP ALL THE CLOCK OF CONTROLLER AND PERIPHERALS
  BlockSleep(block_sleep_mode);
  gpio_setup();

  ADCSetup();
  i2c_setup();
  timer_setup();
  uart0_init();


//  LESENSE_setup();

/* Infinite loop */
  while (1)
  {
//	  sleep();
  }
}
