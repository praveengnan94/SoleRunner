/*
 * timer_setup.c
 *
 *  Created on: Nov 16, 2017
 *      Author: Praveen
 */

#include "timer_setup.h"
int hrcount;
int glb_ct=0;
uint32_t adc_array[1000];

void LETIMER0_IRQHandler(void)
{
	INT_Disable();
	hrcount=(hrcount+1)%50;
	if((LETIMER0->IF & LETIMER_IF_COMP1)!=0)//IF COMP1 FLAG IS SET IN LETIMER0->IF
	{
	  LETIMER_IntClear(LETIMER0, LETIMER_IF_COMP1);
	}

	else if((LETIMER0->IF & LETIMER_IF_UF)!=0)//IF UF FLAG IS SET IN LETIMER0->IF
	{
		/* Clear LETIMER0 underflow interrupt flag */
		 LETIMER_IntClear(LETIMER0, LETIMER_IF_UF);
		if(hrcount==1)
		{
			ADC0->CMD = ADC_CMD_SINGLESTART;
		  GPIO_PinModeSet(HR_PWR_PORT, HR_PWR_PIN, gpioModePushPull, 1);
		}
		else if(hrcount==6)
		{
		  //ADC read

		 while (ADC0->STATUS & ADC_STATUS_SINGLEACT) ;

		  adc_array[glb_ct]=ADC0->SINGLEDATA;
			adc_array[glb_ct]=ADC_DataSinglePeek(ADC0);
		  glb_ct++;
		  if(glb_ct==50)
			  glb_ct=0;
//		  adc_array[1]=ADC_DataSinglePeek(ADC0);
		  GPIO_PinModeSet(HR_PWR_PORT, HR_PWR_PIN, gpioModePushPull, 0);
//		  ADC_Start(ADC0,ADC_CMD_SINGLESTOP);
		  ADC0->CMD =  ADC_CMD_SINGLESTOP;
		}
	}

	INT_Enable();
}

/**************************************************************************//**
 * @brief  LETIMER_setup
 * Configures and starts the LETIMER0
 *****************************************************************************/
void timer_setup(void)
{
	int comp0;	//comp0 register countdown value
	int comp1;	//comp1 register value
  	if(block_sleep_mode==EM3)			//If EM3, then use Ultra low frequency clocks
  	{
  		comp0=LETIMER_PERIOD*(ULF_CLK);//MULTIPLY 2s*1000
  		comp1=LED_ON_TIME*(ULF_CLK);	//MULTIPLY 0.05*1000
  	}
  	else
  	{
  		comp0=LETIMER_PERIOD*(LF_CLK-1);//MULTIPLY 2s*(32768-1) to accommodate overflow in the 16but letimer counter register
  		comp1=LED_ON_TIME*(LF_CLK);		//MULTIPLY 0.05s*32768- UNDERFLOW INTERRUPT
  	}
  	/* Set initial compare values for COMP0 and COMP1
  	     COMP1 keeps it's value and is used as TOP value
  	     for the LETIMER.
  	     COMP1 gets decremented through the program execution
  	     to generate a different PWM duty cycle *///Reference main_letimer_rtc_pulses.c-> LETIMER_Setup(void)
  	LETIMER_CompareSet(LETIMER0, 0, comp0);
  	LETIMER_CompareSet(LETIMER0, 1, comp1);


  	 /* Set configurations for LETIMER 0 *///Referenced from main_letimer_rtc_pulses.c-> LETIMER_Setup(void)
  	 const LETIMER_Init_TypeDef letimerInit =
  	  {
  	  .enable         = true,                   /* Start counting when init completed. */
  	  .debugRun       = false,                  /* Counter shall not keep running during debug halt. */
  	  .rtcComp0Enable = false,                  /* Don't start counting on RTC COMP0 match. */
  	  .rtcComp1Enable = false,                  /* Don't start counting on RTC COMP1 match. */
  	  .comp0Top       = true,                   /* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */
  	  .bufTop         = false,                  /* Don't load COMP1 into COMP0 when REP0 reaches 0. */
  	  .out0Pol        = 0,                      /* Idle value for output 0. */
  	  .out1Pol        = 1,                      /* Idle value for output 1. */
  	  .repMode        = letimerRepeatFree       /* Count until stopped */
  	  };

  	  /* Initialize LETIMER */
  	  LETIMER_Init(LETIMER0, &letimerInit);
  //	  BlockSleep(block_sleep_mode);
  	  /* Enable LETIMER0 interrupt vector in NVIC*/
  	    NVIC_EnableIRQ(LETIMER0_IRQn);
  	  LETIMER0->IEN|=LETIMER_IF_UF|LETIMER_IF_COMP1;//ENABLE COMP0(UF) AND COMP1 INTERRUPTS
//  	  LETIMER0->IEN|=LETIMER_IF_COMP1;

}

