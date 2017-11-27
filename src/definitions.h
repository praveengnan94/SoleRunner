/*
 * definitions.h
 *
 *  Created on: Nov 16, 2017
 *      Author: Praveen
 */

#ifndef SRC_DEFINITIONS_H_
#define SRC_DEFINITIONS_H_

/** Status value used for showing the Energy Mode the device is currently in. */

#define EM0 0
#define EM1 1
#define EM2 2
#define EM3 3
#define COMPMAX 500

#define block_sleep_mode 0

#define LF_CLK 32768
#define ULF_CLK 1000


//DEFINITIONS FOR HEART RATE MODULE

#define HR_PWR_PORT				 gpioPortD
#define HR_PWR_PIN				 7U
#define ADC_PORT_HR    			 gpioPortD
#define ADC_PIN_HR				 6U
#define LETIMER_PERIOD 0.001
#define LED_ON_TIME 0.01	//1 milli second

//DEFINTIONS FOR I2C PINS AND PORTS

#define i2c_sda_port gpioPortC
#define i2c_sda_pin 4U
#define i2c_sda_port_mode gpioModeWiredAnd

#define i2c_scl_port gpioPortC
#define i2c_scl_pin 5U
#define i2c_scl_port_mode gpioModeWiredAnd

#define i2c_int_mg_port gpioPortB
#define i2c_int_mg_pin 12U
#define i2c_int_mg_port_mode gpioModeInput
#define i2c_int_mg_rising_edge true
#define i2c_int_mg_falling_edge true
#define i2c_int_mg_enable true
#define i2c_int_mg_disable false

#define i2c_int_xl_port gpioPortD
#define i2c_int_xl_pin 5U
#define i2c_int_xl_port_mode gpioModeInput
#define i2c_int_xl_rising_edge true
#define i2c_int_xl_falling_edge true
#define i2c_int_xl_enable true
#define i2c_int_xl_disable false

#define ACCEL_I2C_ADDRESS 0X1DU
#define MAGNETOMETER_I2C_ADDRESS 0X1EU
#define wr_bit 0
#define rd_bit 1

#define i2c_int_port_mode gpioModeInput

#define i2c_int_rising_edge false
#define i2c_int_falling_edge true
#define i2c_int_enable true
#define i2c_int_disable false
#define i2c_location _I2C_ROUTE_LOCATION_LOC0
#define i2c_scl_enable I2C_ROUTE_SCLPEN
#define i2c_sda_enable I2C_ROUTE_SDAPEN

//UART DEFITIONS
#define LEUART0_PORT gpioPortD
#define LEUART0_PIN 4
#define LEUART0_MODE gpioModePushPull
#define LEUART0_STATE 1
#define LEUART0_REF_FREQ 0
#define LEUART0_BAUD 9600
#define LEUART0_DATABITS leuartDatabits8
#define LEUART0_PARITY leuartNoParity
#define LEUART0_STOP_BITS leuartStopbits1

#define buffer_length 100
char circular_buffer[buffer_length];
int read_pntr;
int write_pntr;
int glbl;
int glbl_write;

//DEFINITIONS FOR ADC
#define ADC_SAMPLES 1
int adc_complete;
#define ADC_EM EM1				//Energy mode of ADC is EM1
#define CAL_TEMP_0 0x0FE081B2
#define ADC0_TEMP_0_READ_1V25 0x0FE081BE
#define adc_channel ADC0
#define temperature_pin adcSingleInputTemp
#define int_temp_reference adcRef1V25
#define adc_resolution adcRes6Bit
#define adc_acquire_time adcAcqTime2
#define ADC0_rep true
#define ADC0_warmup adcWarmupKeepScanRefWarm
#define adc_clk 1300000
#define ADC0_DMA_Arbitration dmaArbitrate1
#define TEMP_Sensor_DMA true


#endif /* SRC_DEFINITIONS_H_ */
