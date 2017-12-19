/*
 * rtc.c
 *
 *  Created on: Dec 15, 2017
 *      Author: Praveen
 */

#include "rtc.h"
/**************************************************************************//**
 * @brief Enables LFACLK and selects LFXO as clock source for RTC
 *        Sets up the RTC to generate an interrupt every minute.
 *****************************************************************************/
void rtcSetup(void)
{
  RTC_Init_TypeDef rtcInit = RTC_INIT_DEFAULT;

  /* Enable LE domain registers */
  CMU_ClockEnable(cmuClock_CORELE, true);

  /* Enable LFXO as LFACLK in CMU. This will also start LFXO */
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

  /* Set a clock divisor of 32 to reduce power conumption. */
  CMU_ClockDivSet(cmuClock_RTC, cmuClkDiv_32);

  /* Enable RTC clock */
  CMU_ClockEnable(cmuClock_RTC, true);

  /* Initialize RTC */
  rtcInit.enable   = false;  /* Do not start RTC after initialization is complete. */
  rtcInit.debugRun = false;  /* Halt RTC when debugging. */
  rtcInit.comp0Top = true;   /* Wrap around on COMP0 match. */
  RTC_Init(&rtcInit);

  if(block_sleep_mode==EM3)			//If EM3, then use Ultra low frequency clocks
	{
		RTC_CompareSet(0, ((ULF_CLK / 32) * RTC_TIME ) - 1 );
	}
	else
	{
		RTC_CompareSet(0, ((LF_CLK / 32) * RTC_TIME ) - 1 );
	}
  /* Interrupt every minute */


  /* Enable interrupt */
  NVIC_EnableIRQ(RTC_IRQn);
  RTC_IntEnable(RTC_IEN_COMP0);

  /* Start Counter */
  RTC_Enable(true);
}

void RTC_IRQHandler(void)
{
  /* Clear interrupt source */
  RTC_IntClear(RTC_IFC_COMP0);

  LETIMER0->IEN|=LETIMER_IF_UF;//ENABLE COMP0(UF) AND COMP1 INTERRUPTS
  /* Increase time by one minute */

  minutes++;
  if (minutes > 59)
  {
    minutes = 0;
    hours++;
    if (hours > 23)
    {
      hours = 0;
    }
  }
  int step_cnt_val = 7;
  sprintf(global_buffer+LAST_WRITTEN_VALUE,"A%02d%02d%02d*",step_cnt_val,hours,minutes);
  LAST_WRITTEN_VALUE=(LAST_WRITTEN_VALUE+8)%256;
}
