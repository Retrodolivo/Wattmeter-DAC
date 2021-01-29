#include "main.h"

static float initial_voltage = 0;
static unsigned char ftdi_spi_data_arr[sizeof(float)];
static uint8_t byte_counter = 0;
static float ftdi_spi_fdata = 0;

/*is used to build incoming byte array data to float value*/
static float bytes_to_float(unsigned char *byte_arr)
{
	return ftdi_spi_fdata = *(float*)(byte_arr);
}

static void init_routine(void)
{
	spi_init();
	_delay_ms(25);
	DAC1220_reset();
	_delay_ms(25);
//	DAC1220_init();
	command_calibrate_dac();
	spi_init_slave();
}

ISR(SPI_STC_vect)							//SPI Interrupt upon byte receive 
{
	if(byte_counter < sizeof(float))
	{
		ftdi_spi_data_arr[byte_counter] = SPDR;
		byte_counter++;
		if(byte_counter == sizeof(float))
		{ 
			DAC1220_set_voltage(bytes_to_float(ftdi_spi_data_arr));
			byte_counter = 0;
		}
	}
	
}

int main(void)
{
	sei();
	_delay_ms(500);
	init_routine();
//--------------------------------------------	
	DAC1220_set_voltage(initial_voltage);
	
	while (1) 
	{	
		
	}	
}

