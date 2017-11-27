/*
 * ADCSetup.c
 *
 *  Created on: Nov 21, 2017
 *      Author: Praveen
 */

#include "ADCSetup.h"
uint16_t i;
int32_t sum=0;

void ADC0_IRQHandler()
{
	INT_Disable();
//	 ADC_IntDisable(ADC0,ADC_IEN_SINGLE);
	ADC_IntClear(ADC0,ADC_IEN_SINGLE);
					  int32_t sum=0;
					  uint32_t adc_array[ADC_SAMPLES];
						  adc_array[i]=ADC0->SINGLEDATA;
						  i++;

//				  if(i==(ADC_SAMPLES-1))
//				  {
//				  i=0;
//				  UnBlockSleep(ADC_EM);
//				  ADC_IntClear(ADC0,ADC_IEN_SINGLE);
//				  ADC_Start(ADC0,ADC_CMD_SINGLESTOP);
//				  while(i<ADC_SAMPLES)
//					  {
//					  sum=sum+adc_array[i];
//					  i++;
//					  }
//				  i=0;
//					  sum = sum/ADC_SAMPLES;
//					  sum=adc_array[0];
//					  float temp;
//					  temp=convertToCelsius(sum);
//
//					  if((upper_temp>=temp)&&(temp>=lower_temp))
//						  GPIO_PinOutClear(LEDPORT, 3U);
//					  else
//						  GPIO_PinOutSet(LEDPORT, 3U);
//				  }

	  INT_Enable();
}

void ADCSetup(void)
{
	ADC_Init_TypeDef init =
   {
	 .lpfMode 						= adcLPFilterBypass,			//ADC_LPFilter_TypeDef
	 .ovsRateSel 					= adcOvsRateSel2,		//ADC_OvsRateSel_TypeDef
	 .warmUpMode 					= adcWarmupKeepADCWarm,		//ADC_Warmup_TypeDef
	 .timebase 						= ADC_TimebaseCalc(0),		//uint8_t
	 .prescale 						= ADC_PrescaleCalc(13000000, 0),					//calculated for
	 .tailgate 						= 0						//No tailgating
  };

	/*Initializing the setup of ADC in Single sample mode*/
	ADC_InitSingle_TypeDef singleInit =
	{
	  .prsSel 							= adcPRSSELCh0,          /* PRS ch0 (if enabled). */
	  .acqTime 							= adcAcqTime1,            /* 2 ADC_CLK cycle acquisition time. */
	  .reference 						= adcRef2V5,          /* 1.25V internal reference for temp measurement*/
	  .resolution 						= adcRes8Bit,         /* 12 bit resolution. */
	  .input 							= adcSingleInpCh6,	     /* Temp input selected. */
	  .diff 							= false,                 /* Single ended input. */
	  .prsEnable 						= false,                  /* PRS disabled. */
	  .leftAdjust 						= false,                 /* Right adjust. */
	  .rep 								= false                   /* Conversion to continue after one read. */
	};

	ADC_Init(adc_channel, &init);
  ADC_InitSingle(adc_channel, &singleInit);
  ADC0->SINGLECTRL |= ADC_SINGLECTRL_REP;

//  ADC_Start(ADC0, adcStartSingle);
//  ADC0->IEN= 0x01;
//  NVIC_EnableIRQ(ADC0_IRQn);
}

