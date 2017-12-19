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

#if 0
  GpioSetup();
  timer_init();
  spi_init();
  bma_init();
#endif

  ADCSetup();
  i2c_setup();
  rtcSetup();
  timer_setup();
  uart0_init();



/* Infinite loop */
  while (1)
  {
//	  sleep();
  }
}
