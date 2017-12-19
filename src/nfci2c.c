/*
 * nfci2c.c
 *
 *  Created on: Nov 30, 2017
 *      Author: Praveen
 */

#include "nfci2c.h"
uint8_t data1,data,read_data[16],write_data[16];
int buff_inc = 0;

void load_poweron()
{
	CMU_ClockEnable(cmuClock_I2C1,true);
	CMU_ClockEnable(cmuClock_I2C0,true);

	GPIO_PinModeSet(i2c_scl_port, i2c_scl_pin, i2c_scl_port_mode, 1);
	GPIO_PinModeSet(i2c_sda_port, i2c_sda_pin, i2c_sda_port_mode, 1);



		 const I2C_Init_TypeDef i2cInit =
		 {
				 .enable= i2c_enable_init,
				 .master=i2c_master_init,
				 .refFreq=i2c_refFreq_init,
				 .freq=i2c_freq_init,
				 .clhr=i2c_clhr_init,
		 };

		 I2C1->CTRL &= ~I2C_CTRL_GCAMEN;
		 I2C1->CTRL &= ~I2C_CTRL_ARBDIS;
		 I2C1->ROUTE = i2c_sda_enable | i2c_scl_enable | (i2c_location);
		 I2C_Init(I2C1, &i2cInit);

		 if (I2C1->STATE & I2C_STATE_BUSY){
		 		I2C1->CMD = I2C_CMD_ABORT;
		 	}

}

void load_poweroff()
{
	GPIO_PinModeSet(i2c_scl_port, i2c_scl_pin, i2c_scl_port_mode, 0);
	GPIO_PinModeSet(i2c_sda_port, i2c_sda_pin, i2c_sda_port_mode, 0);
	CMU_ClockEnable(cmuClock_I2C1,false);
}

void i2c_nfc_writeauto()
{
	I2C1->CTRL |= I2C_CTRL_AUTOACK;
	I2C1->TXDATA = (NFC_I2C_ADDRESS<<1)|wr_bit;
    I2C1->CMD=I2C_CMD_START;
   	   while((I2C1->IF & I2C_IF_ACK)==0);
}

void i2c_nfc_write()
{
	I2C1->TXDATA = (NFC_I2C_ADDRESS<<1)|wr_bit;
    I2C1->CMD=I2C_CMD_START;
   	   while((I2C1->IF & I2C_IF_ACK)==0);
}

void i2c_wr_to_nfc(uint16_t reg)
{
	INT_Disable();
	i2c_nfc_writeauto();
	I2C1->IFC=I2C_IFC_ACK;

	I2C1->TXDATA = (reg);
	while((I2C1->IF & I2C_IF_ACK)==0);
	I2C1->IFC=I2C_IFC_ACK;

	for(int i=0;i<16;i++)
	{
		I2C1->TXDATA = *(global_buffer+i+buff_inc);
		while((I2C1->IF & I2C_IF_ACK)==0);
		I2C1->IFC=I2C_IFC_ACK;
		if (i == (16 - 2))
		{
			I2C1->CTRL &= ~I2C_CTRL_AUTOACK;
		}
	}
	buff_inc = buff_inc + 16;
	if(buff_inc == 256)
	{
		buff_inc = 0;
	}

	I2C1->CMD=I2C_CMD_NACK;
	I2C1->CMD=I2C_CMD_STOP;

//		char savi = 'Q';
//		for(int i=0;i<16;i++)
//		{
//			savi = *(global_buffer+i+buff_inc);
//		}
//		buff_inc = buff_inc + 16;
//		if(buff_inc == 256)
//		{
//			buff_inc = 0;
//		}
	for(long int i=0;i<500;i++);
	INT_Enable();
}

void i2c_rd_from_nfc(uint8_t regi)
{
	i2c_nfc_writeauto();
	I2C1->IFC=I2C_IFC_ACK;

	I2C1->TXDATA = (regi);
	while((I2C1->IF & I2C_IF_ACK)==0);
	I2C1->IFC=I2C_IFC_ACK;

	I2C1->CMD=I2C_CMD_STOP;

	for(int i=0;i<400;i++);

	I2C1->CMD=I2C_CMD_START;	//START EXECUTED ONCE FOR REPEATED START
	I2C1->TXDATA = (NFC_I2C_ADDRESS<<1)|rd_bit;

	while((I2C1->IF & I2C_IF_ACK)==0);
	I2C1->IFC=I2C_IFC_ACK;

	//I2C1->CMD=I2C_CMD_ACK;
	for (int i = 0; i < 16; i++)
	{
		while((I2C1->STATUS & I2C_STATUS_RXDATAV)==0);
		read_data[i]=I2C1->RXDATA;
		if (i == (16 - 2))
		{
			I2C1->CTRL &= ~I2C_CTRL_AUTOACK;
		}
	}

	I2C1->CMD=I2C_CMD_NACK;
	I2C1->CMD=I2C_CMD_STOP;

}

uint8_t i2c_rd_from_nfc_reg(uint8_t mema,uint8_t regi)
{
	i2c_nfc_write();
	I2C1->IFC=I2C_IFC_ACK;

	I2C1->TXDATA = (mema);
	while((I2C1->IF & I2C_IF_ACK)==0);
	I2C1->IFC=I2C_IFC_ACK;

	I2C1->TXDATA = (regi);
	while((I2C1->IF & I2C_IF_ACK)==0);
	I2C1->IFC=I2C_IFC_ACK;

	I2C1->CMD=I2C_CMD_STOP;

	for(int i=0;i<500;i++);

	I2C1->CMD=I2C_CMD_START;	//START EXECUTED ONCE FOR REPEATED START
	I2C1->TXDATA = (NFC_I2C_ADDRESS<<1)|rd_bit;

	while((I2C1->IF & I2C_IF_ACK)==0);
	I2C1->IFC=I2C_IFC_ACK;

	while ((I2C1->IF & I2C_IF_RXDATAV) == 0);
	data = I2C0->RXDATA;

	I2C1->CMD=I2C_CMD_NACK;
	I2C1->CMD = I2C_CMD_STOP;

	return data;
}

void i2c_wr_to_nfc_reg(uint8_t mema,uint8_t regi,uint8_t mask,uint8_t regdata)
{
	i2c_nfc_write();
	I2C1->IFC=I2C_IFC_ACK;

	I2C1->TXDATA = (mema);
	while((I2C1->IF & I2C_IF_ACK)==0);
	I2C1->IFC=I2C_IFC_ACK;

	I2C1->TXDATA = (regi);
	while((I2C1->IF & I2C_IF_ACK)==0);
	I2C1->IFC=I2C_IFC_ACK;

	I2C1->TXDATA = (mask);
	while((I2C1->IF & I2C_IF_ACK)==0);
	I2C1->IFC=I2C_IFC_ACK;

	I2C1->TXDATA = (regdata);
	while((I2C1->IF & I2C_IF_ACK)==0);
	I2C1->IFC=I2C_IFC_ACK;

	I2C1->CMD=I2C_CMD_NACK;
	I2C1->CMD = I2C_CMD_STOP;
}

void i2c_reset(void)
{
	if(I2C1->STATE & I2C_STATE_BUSY){
		 I2C1->CMD = I2C_CMD_ABORT;
	 }
	for(long int j=0;j<50000;j++);

	 /* In some situations (after a reset during an I2C transfer), the slave */
	   /* device may be left in an unknown state. Send 9 clock pulses just in case. */
	   for (int j = 0; j < 9; j++)
	   {
	     /*
	      * TBD: Seems to be clocking at appr 80kHz-120kHz depending on compiler
	      * optimization when running at 14MHz. A bit high for standard mode devices,
	      * but DVK only has fast mode devices. Need however to add some time
	      * measurement in order to not be dependable on frequency and code executed.
	      */
	     GPIO_PinModeSet(i2c_scl_port, i2c_scl_pin, i2c_scl_port_mode, 0);
	     GPIO_PinModeSet(i2c_scl_port, i2c_scl_pin, i2c_scl_port_mode, 1);
	   }
	   for(long int j=0;j<50000;j++){}//ADDED TO REMOVE STRAY CAP ON I2C PINS WHILE GPIO POWER UP
}

void i2c_setup(void)
{
	long int reg;
	load_poweron();
	i2c_reset();

//	i2c_wr_to_nfc(0x08);
//	i2c_wr_to_nfc(0x18);
//	i2c_wr_to_nfc(0x28);
}

unsigned int LAST_WRITTEN_ADDRESS=0x08;

void GPIO_ODD_IRQHandler(void){
	int intFlags;
	intFlags = GPIO->IF;
	GPIO->IFC = intFlags & 0x0000aaaa;	/* GPIO even interrupt handler so only clear even interrupts */
	INT_Disable();
	if(FDONflag == 0)
	{
		GPIO_IntConfig(Nfc_Fd_Port, Nfc_Fd_Pin, true, false, true);
		FDONflag = 1;
		i2c_wr_to_nfc(0x08);
	}
	else
	{
		GPIO_IntConfig(Nfc_Fd_Port, Nfc_Fd_Pin, false, true, true);
		FDONflag = 0;
	}

	//Start transmitting data from global buffer to NTAG via i2c to LAST_WRITTEN_ADDRESS
//		i2c_wr_to_nfc(LAST_WRITTEN_ADDRESS,global_buffer[LAST_WRITTEN_ADDRESS]);
	//i2c_wr_to_nfc(0x18);
	//LAST_WRITTEN_ADDRESS=((LAST_WRITTEN_ADDRESS+16)%255);
	INT_Enable();
}

