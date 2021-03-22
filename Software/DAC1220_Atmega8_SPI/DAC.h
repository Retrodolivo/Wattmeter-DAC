#ifndef DAC_H_
#define DAC_H_

/*Software SPI is using to control DAC1220*/

#include "main.h"

#define LOW 0
#define HIGH 1

#define DAC_DATAPIN PORTB0
#define DAC_CLOCKPIN PORTB1

#define DAC_DATAPIN_DIR_OUTPUT DDRB |= (1<<DAC_DATAPIN)
#define DAC_DATAPIN_DIR_INPUT DDRB &= ~(1<<DAC_DATAPIN)

#define DAC_DATAPIN_HIGH PORTB |= (1<<DAC_DATAPIN)
#define DAC_DATAPIN_LOW PORTB &= ~(1<<DAC_DATAPIN)

#define DAC_CLOCK_DIR_OUTPUT DDRB |= (1<<DAC_CLOCKPIN)
#define DAC_CLOCK_DIR_INPUT DDRB &= ~(1<<DAC_CLOCKPIN)

#define DAC_CLOCK_PIN_HIGH PORTB |= (1<<DAC_CLOCKPIN)
#define DAC_CLOCK_PIN_LOW PORTB &= ~(1<<DAC_CLOCKPIN)
/************************DAC*******************************/
void DAC1220_reset(void);
void DAC1220_write3bytes(const uint8_t address, const uint8_t byte1, const uint8_t byte2, const uint8_t byte3);
void DAC1220_write2bytes(const uint8_t address, const uint8_t byte1, const uint8_t byte2);
void DAC1220_read2bytes(const uint8_t address, uint8_t* byte1, uint8_t* byte2);
void DAC1220_read3bytes(const uint8_t address, uint8_t* byte1, uint8_t* byte2, uint8_t* byte3);
void DAC1220_init();
void DAC1220_selfcal();
void DAC1220_set_voltage(float voltage);
void command_calibrate_dac();
/*******************Software SPI***************************/
void spi_init();
/**********************************************************/

#endif /* DAC_H_ */