#include "FTDI_spi.h"

#define  SCK PORTB5
#define  MISO PORTB4
#define  MOSI PORTB3
#define  CS PORTB2

void spi_init_slave(void)
{
	DDRB |= (1<<MISO);
	DDRB &= ~((1<<CS) | (1<<MOSI) | (1<<SCK));
	SPCR = ((1<<SPE) | (1<<SPIE));				//Enable spi and interrupt
}
