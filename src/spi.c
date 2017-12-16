#include "spi.h"
//
///* Global Variable - Array for Sleep Modes */
//int sleep_counter[5] = {0,0,0,0,0};
//
///* LED PORT AND PIN Properties */
//#define LED_PORT    gpioPortE
//#define LED_pin     2
//
//void GpioSetup(void);
//
//
//void CmuSetup(void)
//{
//	CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);			// Enable HFRC0
//	CMU_ClockSelectSet(cmuClock_HFPER, cmuSelect_HFRCO);
//	CMU_ClockEnable(cmuClock_HFPER, true);
//
//	CMU_ClockEnable(cmuClock_CORELE,true);
//	CMU_ClockEnable(cmuClock_USART0, true);
//	CMU_ClockEnable(cmuClock_TIMER0, true);
//	CMU_ClockEnable(cmuClock_GPIO, true);
//}
//
//void GpioSetup(void)
//{
//	// Set SPI pins for BMA280
//	GPIO_DriveModeSet(SPI_MOSI_port,gpioDriveModeLowest);
//
//	GPIO_PinModeSet(SPI_MOSI_port, SPI_MOSI_pin, gpioModePushPull, INITIAL_LEVEL_HIGH);
//	GPIO_PinModeSet(SPI_MISO_port, SPI_MISO_pin, gpioModeInput, INITIAL_LEVEL_LOW);
//	GPIO_PinModeSet(SPI_SCK_port, SPI_SCK_pin, gpioModePushPull, INITIAL_LEVEL_HIGH);
//	GPIO_PinModeSet(SPI_CS_port, SPI_CS_pin, gpioModePushPull, INITIAL_LEVEL_LOW);		/* CS is set low, so that the transition from low to high is made after the accelerometer is powered ON */
//	GPIO_PinModeSet(SPI_PWR_port, SPI_PWR_pin, gpioModePushPull, INITIAL_LEVEL_LOW);	/* Initially set to 0, so that as soon as the power is on the CS line is made to go high */
//
//	GPIO_DriveModeSet(LED_PORT,gpioDriveModeLowest);
//	GPIO_PinModeSet(LED_PORT, LED_pin, gpioModePushPull, INITIAL_LEVEL_LOW);
//}
//
//void timer_init(void)
//{
//	TIMER_Init_TypeDef timer_init;
//	timer_init.enable = false;
//	timer_init.debugRun = false;
//	timer_init.prescale = timerPrescale256;			/* Max 1 sec */
//	timer_init.clkSel = timerClkSelHFPerClk;	/*HFPER is selected*/
//	timer_init.count2x = false;
//	timer_init.ati  = true;
//	timer_init.fallAction = timerInputActionNone ;	/*Action on falling input edge*/
//	timer_init.riseAction = timerInputActionNone;
//	timer_init.mode = timerModeDown;
//	timer_init.dmaClrAct = false;
//	timer_init.quadModeX4 = false;
//	timer_init.oneShot = true;
//	timer_init.sync = false;
//	TIMER_Init(TIMER0, &timer_init);
//
//	TIMER0->IFC = TIMER0->IF;
//	TIMER0->IEN = TIMER_IEN_UF;
//	NVIC_EnableIRQ(TIMER0_IRQn);
//}
//
//void sleepDelay(uint32_t time)		/* Pass the parameter in micro sec */
//{
//	uint32_t cnt;
//	uint32_t freq = CMU_ClockFreqGet(cmuClock_TIMER0);
//	cnt = (freq/256);
//	cnt = (((float)(time))/((float)1000000)) * cnt;
//	TIMER0->CNT = cnt;
//	blockSleepStatus(EM1);
//	TIMER_Enable(TIMER0, true);
//	while(TIMER0->CNT != 0)	/* Sleep until the time elapses*/
//		sleep();
//	unblockSleepStatus(EM1);
//}
//
//void TIMER0_IRQHandler(void)
//{
//	uint32_t	int_flag;
//	int_flag = TIMER0->IF;
//	TIMER0->IFC = int_flag;
//	TIMER0->CNT = 0;
//	TIMER_Enable(TIMER0, false);
//	TIMER0->IFC = int_flag;
//}
//
//void spi_init()
//{
//	USART_InitSync_TypeDef spiInit;
//	spiInit.autoTx = false;	//Disable AUTOTX mode. [Transmits as long as RX is not full. If TX is empty, underflows are generated.]
//	spiInit.baudrate = 5000000;	/*7MHz*/
//	spiInit.clockMode = usartClockMode3;	//Clock polarity/phase mode - Clock idle high, sample on rising edge.
//	spiInit.databits = usartDatabits8;		// Number of databits in frame.  8 databits.
//	spiInit.enable = usartDisable;			// Specifies whether TX and/or RX shall be enabled when init completed. - Disable both receiver and transmitter.
//	spiInit.master = true;					// LG is acting as SPI master
//	spiInit.msbf = true;					//Select if to send most or least significant bit first.
//	spiInit.prsRxCh = usartPrsRxCh0;
//	spiInit.prsRxEnable = false;
//	spiInit.refFreq = 0;					// 0 to use currently configured reference clock
//	USART_InitSync(USART0, &spiInit);
//	/* Route SPI pins to the appropriate SPI port */
//	USART0->CTRL |= (USART_CTRL_SYNC);// | USART_CTRL_AUTOCS );// | USART_CTRL_LOOPBK | USART_CTRL_AUTOCS);
//	USART0->ROUTE &= ~(_USART_ROUTE_LOCATION_MASK );
//	USART0->ROUTE |= (USART_ROUTE_LOCATION_LOC0 );
//	USART0->ROUTE |= (USART_ROUTE_CLKPEN | USART_ROUTE_TXPEN | USART_ROUTE_RXPEN); //USART_ROUTE_CSPEN /* CS line is manually handled */
//	USART0->CMD = USART_CMD_CLEARTX | USART_CMD_CLEARRX | USART_CMD_MASTEREN | USART_CMD_TXEN | USART_CMD_RXEN;	// Insure that no data is in the transmit & receive buffers before enabling
//	USART0->IFC = USART0->IF;
//	//USART0->IEN = 0;
//	USART_Enable(USART0, usartEnable);
//}
//
//void bma280_write_byte(uint8_t reg_add, uint8_t data)
//{
//	GPIO_PinOutClear(SPI_CS_port, SPI_CS_pin);
//	uint32_t write_request;
//	write_request = (0 << 7) | reg_add;
//	write_request = (data << 8) | write_request;			/* data on write is 2nd byte */
//	while ((USART0->STATUS & USART_STATUS_TXBL) == 0);		/* If SPI is busy, wait for completion specified as Transmit buffer is empty */
//	USART0->TXDOUBLE = write_request;						/* Send read request to SPI */
//	while ((USART0->STATUS & USART_STATUS_TXC) == 0);		/* Poll for data requested completed transmission */
//	USART0->CMD = USART_CMD_CLEARRX;						/* Empty receive buffer that filled during write operation */
//	GPIO_PinOutSet(SPI_CS_port, SPI_CS_pin);
//}
//
//uint8_t bma280_read_byte(uint8_t address)
//{
//	uint16_t data = 0;
//	uint8_t cmd = 0;
//	GPIO_PinOutClear(SPI_CS_port, SPI_CS_pin);
//	cmd = (1<<7) | address;	/*Read cmd is 1*/
//	while ((USART0->IF & USART_IF_TXBL) == 0);
//	USART0->TXDATA = cmd;
//	while ((USART0->IF & USART_IF_TXC) == 0);
//	USART0->TXDATA = 0xFF;
//	while ((USART0->IF & USART_IF_RXDATAV) == 0);
//	data = USART0->RXDATA;
//	while ((USART0->IF & USART_IF_RXDATAV) == 0);
//	data = USART0->RXDATA;
//	while ((USART0->IF & USART_IF_TXC) == 0);
//	GPIO_PinOutSet(SPI_CS_port, SPI_CS_pin);
//	return data;
//}
//
//void bma_init()
//{
//	uint8_t	temp_byte1 = 0;
//	uint8_t	temp_byte2 = 0;
//	uint8_t	temp_byte3 = 0;
//	uint8_t	temp_byte4 = 0;
//	uint8_t	temp_byte5 = 0;
//	uint8_t	temp_byte6 = 0;
//	uint8_t	temp_byte7 = 0;
//	uint8_t	temp_byte8 = 0;
//	uint8_t	temp_byte9 = 0;
//	uint8_t	temp_byte10 = 0;
//	uint8_t	temp_byte11 = 0;
//	uint8_t	temp_byte12 = 0;
//	uint8_t	temp_byte13 = 0;
//	uint8_t	temp_byte14 = 0;
//	uint8_t	temp_byte15 = 0;
//	uint8_t	temp_byte16 = 0;
//	sleepDelay(2000);					//2ms delay
//	//bma280_write_byte(0x7E, 0xb6);	// CMD reg - 0x7E.  Soft reset occurs when 0xB6 is written to this register
//	sleepDelay(2000);
//	temp_byte1 = 0;
//	temp_byte1 = bma280_read_byte(0x00);	// Chip ID register address is 0x00 // The register contains the chip identification code.
//	if (temp_byte1 == 0xD1)					// BMA180 chip id is 0xD1
//	{
//
//	}
//	sleepDelay(2000);
//	temp_byte2 = bma280_read_byte(0x40);	/* ACC_CONF read - def value is 0x28 */
//	//sleepDelay(2000);
//	temp_byte3 = bma280_read_byte(0x7B);	/* STEP_CONF1 read - def value is 0x03 */
//	//sleepDelay(2000);
//	temp_byte4 = bma280_read_byte(0x41);	/* ACC_RANGE read - def value is 0x03 */
//	//sleepDelay(2000);
//	temp_byte5 = bma280_read_byte(0x7A);	/* STEP_CONF0 read - def value is 0x15 */
//	//sleepDelay(2000);
//	temp_byte6 = bma280_read_byte(0x03);	/* PMU_STATUS read - def value is 0x00 */
//	//sleepDelay(2000);
//	bma280_write_byte(0x7E, 0x11);			/* CMD write to put the accelerometer in normal mode */
//	sleepDelay(2000);						/* Without this delay it leads to erroneous value */
//	temp_byte7 = bma280_read_byte(0x03);	/* PMU_STATUS read - changed to 0x10*/
//	//sleepDelay(2000);
//	bma280_write_byte(0x41, 0x05);			/* Setting the accelerometer g range to 4g */
//	sleepDelay(2000);
//	bma280_write_byte(0x40, (0x0B <<0));	/* Acc odr set to 125Hz */
//	sleepDelay(2000);
//	temp_byte8 = bma280_read_byte(0x78);	/* STEP_CNT_0 read - def value is 0x00 */
//	//sleepDelay(2000);c
//	temp_byte9 = bma280_read_byte(0x79);	/* STEP_CNT_1 read - def value is 0x00 */
//	//sleepDelay(2000);
//	temp_byte10 = bma280_read_byte(0x78);	/* STEP_CNT_0 read - def value is 0x00 */
//	//sleepDelay(2000);
//	temp_byte11 = bma280_read_byte(0x79);	/* STEP_CNT_1 read - def value is 0x00 */
//	//sleepDelay(2000);
//	bma280_write_byte(0x7B, (0x0B <<0));	/* STEP_CONF1 write - step_cnt_en is set and the min_step_buff is changed to 011 */
//	sleepDelay(2000);
//	bma280_write_byte(0x7A, (0x1D <<0));	/* STEP_CONF0 write - 0b0001 1101. min_threshold is set to 11 and steptime_min is set to 101 */
//	sleepDelay(2000);
//	temp_byte12 = bma280_read_byte(0x7B);	/* STEP_CONF1 read - def value is 0x03 */
//	//sleepDelay(2000);
//	temp_byte13 = bma280_read_byte(0x78);	/* STEP_CNT_0 read - def value is 0x00 */
//	//sleepDelay(2000);
//	temp_byte14 = bma280_read_byte(0x79);	/* STEP_CNT_1 read - def value is 0x00 */
//	//sleepDelay(2000);
//	temp_byte15 = bma280_read_byte(0x78);	/* STEP_CNT_0 read - def value is 0x00 */
//	//sleepDelay(2000);
//	temp_byte16 = bma280_read_byte(0x79);	/* STEP_CNT_1 read - def value is 0x00 */
//	//sleepDelay(2000);
//
//}
//
//int main(void)
//{
//	int step_cnt_val = 0;
//  /* Chip errata */
//  CHIP_Init();
//  blockSleepStatus(EM3);
//  CmuSetup();
//  GpioSetup();
//  timer_init();
//  blockSleepStatus(EM1);
//  spi_init();
//
//  GPIO_PinOutSet(SPI_PWR_port, SPI_PWR_pin);
//  //sleepDelay(1000);									/* 1ms delay */
//  GPIO_PinOutSet(SPI_CS_port, SPI_CS_pin);
//  GPIO_PinOutClear(SPI_CS_port, SPI_CS_pin);
//  GPIO_PinOutSet(SPI_CS_port, SPI_CS_pin);
//
//  bma_init();
//  while (1)
//  {
//	  step_cnt_val = bma280_read_byte(0x78);
//	  sleepDelay(2000);
//	  if(step_cnt_val > 25)
//	  {
//		  GPIO_PinOutSet(LED_PORT, LED_pin);
//	  }
//  }
//}
