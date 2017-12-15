/*
 * nfci2c.c
 *
 *  Created on: Nov 30, 2017
 *      Author: Praveen
 */

#include "nfci2c.h"
unsigned int datax_xl,data1,data2,val1,val2,val_0,val_1=0,i2c_int_flag,datay_xl,dataz_xl,datax,datay,dataz,datax_mg,datay_mg,dataz_mg,data;
int new_data,look_for_30,look_for_15,look_for_45,look_for_n15,look_for_n30,look_for_n45,look_for_0;


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

void i2c_nfc_write()
{
//	I2C1->TXDATA = (NFC_I2C_ADDRESS<<1)|wr_bit;
	I2C1->TXDATA = NFC_I2C_ADDRESS;
		   I2C1->CMD=I2C_CMD_START;
		   while((I2C1->IF & I2C_IF_ACK)==0);
}

void i2c_wr_to_nfc_reg(uint16_t reg,uint16_t val)
{
	i2c_nfc_write();
	     I2C1->IFC=I2C_IFC_ACK;

	     I2C1->TXDATA = (reg);
	     while((I2C1->IF & I2C_IF_ACK)==0);
		I2C1->IFC=I2C_IFC_ACK;

		I2C1->TXDATA = (val);
		while((I2C1->IF & I2C_IF_ACK)==0);
			I2C1->IFC=I2C_IFC_ACK;

			I2C1->CMD=I2C_CMD_STOP;
}
uint32_t i2c_rd_from_nfc_reg(uint8_t regi)
{
	data=0;
//	i2c_nfc_write();
			     I2C1->IFC=I2C_IFC_ACK;

	   I2C1->TXDATA = (regi);
	   while((I2C1->IF & I2C_IF_ACK)==0);
	   I2C1->IFC=I2C_IFC_ACK;

	   I2C1->CMD=I2C_CMD_START;	//START EXECUTED ONCE FOR REPEATED START
	   I2C1->TXDATA = (NFC_I2C_ADDRESS<<1)|rd_bit;
	   while((I2C1->IF & I2C_IF_ACK)==0);
	   I2C1->IFC=I2C_IFC_ACK;

	   //I2C1->CMD=I2C_CMD_ACK;
	   while((I2C1->STATUS & I2C_STATUS_RXDATAV)==0);
	   data=I2C1->RXDATA;

	   I2C1->CMD=I2C_CMD_NACK;
	   I2C1->CMD=I2C_CMD_STOP;

	   return data;
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
	load_poweron();
	i2c_reset();

	//ACCELEROMETER CONFIGURATION
    i2c_wr_to_nfc_reg(0x60,0);
//
//	i2c_wr_to_accel_reg(CTRL_REG1_A, CTRL_REG1_A_CONFG);
//	i2c_wr_to_accel_reg(CTRL_REG2_A, CTRL_REG2_A_CONFG);
//	i2c_wr_to_accel_reg(CTRL_REG4_A, CTRL_REG4_A_CONFG);
//	//i2c_wr_to_accel_reg(ACT_THS_A,ACT_THS_A_VAL);
//
//    i2c_wr_to_accel_reg(IG_THS_X1_A,IG_THS_X1_A_VAL);
//	i2c_wr_to_accel_reg(IG_THS_Y1_A,IG_THS_Y1_A_VAL);
//	i2c_wr_to_accel_reg(IG_THS_Z1_A,IG_THS_Z1_A_VAL);
////	i2c_wr_to_accel_reg(IG_DUR1_A,IG_DUR1_A_VAL);
//
//	i2c_wr_to_accel_reg(IG_CFG1_A, IG_CFG1_A_CONFG);
//    i2c_wr_to_accel_reg(CTRL_REG3_A, CTRL_REG3_A_CONFG);
    look_for_0=1;


//   while(1)
//   {
//   }
}

/*void GPIO_ODD_IRQHandler(void)
{
	INT_Disable();
//	GPIO_IntConfig(i2c_int_xl_port, i2c_int_xl_pin,  i2c_int_xl_rising_edge, i2c_int_xl_falling_edge, i2c_int_xl_disable);
//	GPIO_IntConfig(i2c_int_xl_port, i2c_int_xl_pin,  i2c_int_xl_rising_edge, i2c_int_xl_falling_edge, i2c_int_xl_enable);
//	GPIO_IntClear(0x02);
//	data1=i2c_rd_from_ACCEL_reg(ACCEL_X_LSB);
//	data2=i2c_rd_from_ACCEL_reg(ACCEL_X_MSB);
//	datax_xl=(data2<<8)+data1;
//
//	data1=i2c_rd_from_ACCEL_reg(ACCEL_Y_LSB);
//	data2=i2c_rd_from_ACCEL_reg(ACCEL_Y_MSB);
//	datay_xl=(data2<<8)+data1;
//
//	data1=i2c_rd_from_ACCEL_reg(ACCEL_Z_LSB);
//	data2=i2c_rd_from_ACCEL_reg(ACCEL_Z_MSB);
//	dataz_xl=(data2<<8)+data1;
//
//	templ=i2c_rd_from_MAGNETOMETER_reg(MAG_TEMPL);

  INT_Enable();
}*/

