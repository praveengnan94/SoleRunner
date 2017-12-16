/*
 * timer_setup.c
 *
 *  Created on: Nov 16, 2017
 *      Author: Praveen
 */

#include "timer_setup.h"

#define DIFFERENCE 220
#define HR_COUNT 5005

int hrcount;
long int glb_ct=0;
uint32_t adc_array[HR_COUNT-5];

long int pk_val=0,val_val=0,nw_val=0;
long int prev_val_val=0,prev_pk_val=0;
long int prev_measured_diff=0,measured_diff=0;
bool val_ready_to_measure=false;
bool pk_ready_to_measure=false;
bool proper=false;
bool first_flag=false;
int valid_count=0;
int BPM=0;

void LETIMER0_IRQHandler(void)
{
	INT_Disable();
	hrcount=(hrcount+1)%HR_COUNT;
	if((LETIMER0->IF & LETIMER_IF_COMP1)!=0)//IF COMP1 FLAG IS SET IN LETIMER0->IF
	{

//		ADC0->CMD = ADC_CMD_SINGLESTART;
	  LETIMER_IntClear(LETIMER0, LETIMER_IF_COMP1);

	}

	else if((LETIMER0->IF & LETIMER_IF_UF)!=0)//IF UF FLAG IS SET IN LETIMER0->IF
	{
		/* Clear LETIMER0 underflow interrupt flag */
		 LETIMER_IntClear(LETIMER0, LETIMER_IF_UF);
		if(hrcount==1)
		{
		  GPIO_PinModeSet(HR_PWR_PORT, HR_PWR_PIN, gpioModePushPull, 1);

		}
		else if(hrcount>=5)
		{
		  //ADC read
		  ADC0->CMD =  ADC_CMD_SINGLESTART;
		  while (ADC0->STATUS & ADC_STATUS_SINGLEACT);
		  adc_array[glb_ct]=ADC_DataSingleGet(ADC0);

//		  LEUART_Tx(LEUART0,adc_array[glb_ct]);

		  glb_ct++;
		  if(glb_ct==HR_COUNT)
		  {
			  first_flag=true;
			  GPIO_PinModeSet(HR_PWR_PORT, HR_PWR_PIN, gpioModePushPull, 0);
			  LETIMER0->IEN&=0xFFF9;//DISABLE COMP0(UF) AND COMP1 INTERRUPTS
			  ADC0->CMD =  ADC_CMD_SINGLESTOP;
			  glb_ct=0;
			  hrcount=0;
			  for(int i=0;i<HR_COUNT;i++)
			  {
				  if(first_flag==true)
				  {
					  first_flag=false;
					  nw_val=adc_array[i];
					  pk_val=nw_val;
					  val_val=nw_val;
				  }
				  else
				  {
					  prev_measured_diff=measured_diff;
					  nw_val=adc_array[i];
					  if(nw_val>pk_val)
					  {
						  prev_pk_val=pk_val;
						  pk_val=nw_val;
					  }
					  if(nw_val<val_val)
					  {
						  prev_val_val=val_val;
						  val_val=nw_val;
					  }

					  measured_diff=pk_val-val_val;

					  if(prev_measured_diff<measured_diff)
					  {
						  if(measured_diff>=DIFFERENCE)
						  {
							  //VALID PEAK-VALLEY
							  valid_count++;
							  first_flag=true;
						  }
					  }
				  }
			  }
			  BPM=valid_count*(60/((HR_COUNT-5)/1000));

			  sprintf(global_buffer+LAST_WRITTEN_VALUE,"H%03d%02d%02d",BPM,hours,minutes);
			  LAST_WRITTEN_VALUE=(LAST_WRITTEN_VALUE+8)%255;

			  LEUART_Tx(LEUART0,BPM);
			  //write to NANDFLASH with timestamp
			  valid_count=0;
		  }
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
  		comp0=COMP0TIME*(ULF_CLK);//MULTIPLY 2s*1000
  		comp1=COMP1TIME*(ULF_CLK);	//MULTIPLY 0.05*1000
  	}
  	else
  	{
  		comp0=COMP0TIME*(LF_CLK-1);//MULTIPLY 2s*(32768-1) to accommodate overflow in the 16but letimer counter register
  		comp1=COMP1TIME*(LF_CLK);		//MULTIPLY 0.05s*32768- UNDERFLOW INTERRUPT
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


}
