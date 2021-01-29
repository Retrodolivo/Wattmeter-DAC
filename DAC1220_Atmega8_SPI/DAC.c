#include "DAC.h"

const uint32_t MAX_INPUT = 16777215;//pow(2,24) - 1
const char MAX_VOLTAGE = 5;

static uint32_t float_voltage_to_uint32_t(float voltage);
static void uint32_split_to_bytes(uint32_t input, unsigned char *bytes);

static uint8_t spi_read_byte();
static void spi_write_byte(uint8_t data_byte);
static void clock_pulse();
static void spi_delay();


//***********************DAC*******************************/
//txin - refer to datasheet
void DAC1220_reset()
{
	DAC_CLOCK_PIN_HIGH;
	_delay_us(269);//t = (512+800)/2 * txin
	DAC_CLOCK_PIN_LOW;
	spi_delay();
	DAC_CLOCK_PIN_HIGH;
	_delay_us(575);//t = (1800+1024)/2 * txin
	DAC_CLOCK_PIN_LOW;
	spi_delay();
	DAC_CLOCK_PIN_HIGH;
	_delay_us(905);//t = (2400+2048)/2 * txin 
	DAC_CLOCK_PIN_LOW;
	spi_delay();
}

void DAC1220_write3bytes(const uint8_t address, const uint8_t byte1, const uint8_t byte2, const uint8_t byte3)
{
	DAC_DATAPIN_DIR_OUTPUT;
	spi_write_byte(64+address);
	spi_write_byte(byte1);
	spi_write_byte(byte2);
	spi_write_byte(byte3);
	DAC_DATAPIN_DIR_INPUT;
	spi_delay();
}

void DAC1220_write2bytes(const uint8_t address, const uint8_t byte1, const uint8_t byte2)
{
	DAC_DATAPIN_DIR_OUTPUT;
	spi_write_byte(32+address);
	spi_write_byte(byte1);
	spi_write_byte(byte2);
	DAC_DATAPIN_DIR_INPUT;
	spi_delay();
}

void DAC1220_read2bytes(const uint8_t address, uint8_t* byte1, uint8_t* byte2)
{
	DAC_DATAPIN_DIR_OUTPUT;
	spi_write_byte(160+address);
	DAC_DATAPIN_DIR_INPUT;
	spi_delay();
	*byte1 = spi_read_byte();
	*byte2 = spi_read_byte();
	spi_delay();
}

void DAC1220_read3bytes(const uint8_t address, uint8_t* byte1, uint8_t* byte2, uint8_t* byte3)
{
	DAC_DATAPIN_DIR_OUTPUT;
	spi_write_byte(192+address);
	DAC_DATAPIN_DIR_INPUT;
	spi_delay();
	*byte1 = spi_read_byte();
	*byte2 = spi_read_byte();
	*byte3 = spi_read_byte();
	spi_delay();
}

void DAC1220_init()
{
	DAC1220_write2bytes(4, 32, 160);		//command reg: 20-bit resolution, straight binary
}

void DAC1220_selfcal()
{
	DAC1220_write2bytes(4, 32, 161);		//command reg: 20-bit resolution, straight binary, selfcalmode
}

void command_calibrate_dac()
{
	DAC1220_selfcal();
	_delay_ms(1000);//ждем пока калибровка закончится
}

static void uint32_split_to_bytes(uint32_t input, unsigned char *bytes)
{
	bytes[0] = (input >> 16) & 0xFF;
	bytes[1] = (input >> 8) & 0xFF;	
	bytes[2] = input & 0xFF;
}

//записываем значение выходного напряжения с плавающей точкой, которое хотим получить
static uint32_t float_voltage_to_uint32_t(float voltage)
{
	uint32_t bit_code;
	if(voltage < 0)
		voltage = 0;
	else if(voltage > MAX_VOLTAGE)
		voltage = MAX_VOLTAGE;
	
	bit_code = (uint32_t)(voltage/5.0 * MAX_INPUT + 0.5);
	if(bit_code > MAX_INPUT) 
		bit_code = MAX_INPUT;
	return bit_code;
}

void DAC1220_set_voltage(float voltage)
{	
	unsigned char bytes[3];
	uint32_split_to_bytes(float_voltage_to_uint32_t(voltage), bytes);
	DAC1220_write3bytes(0, bytes[0], bytes[1], bytes[2]);
}

//*******************Software SPI***************************/
static uint8_t spi_read_byte()
{
	uint8_t data_byte = 0;
	uint8_t bit_counter = 8;
	do
	{
		clock_pulse();
		data_byte <<= 1;
		data_byte &= 0xFE;       //ставим нулевой(0) бит в ноль
		if(PINB&PINB3)           //если на шине высокий уровень
		data_byte |= 0x01;		 //записываем значение очередного бита
	} while (--bit_counter);     //повторяем пока не получим 8 бит
	return data_byte;
}

static void spi_write_byte(uint8_t data_byte)
{
	uint8_t bit_counter = 8;
	do
	{
		if(data_byte&0x80)
		DAC_DATAPIN_HIGH;
		else
		DAC_DATAPIN_LOW;
		clock_pulse();
		data_byte <<= 1;
	} while (--bit_counter);
}

void spi_init()
{
	// CLOCK на выход; низкий уровень
	DAC_CLOCK_DIR_OUTPUT;
	DAC_CLOCK_PIN_LOW;
	
	//DAC_DATAPIN на вход;
	DAC_DATAPIN_DIR_INPUT;
}

static void clock_pulse()
{
	//Генерируем импульс CLOCK
	DAC_CLOCK_PIN_HIGH;
	spi_delay();
	DAC_CLOCK_PIN_LOW;
	spi_delay();
}

static void spi_delay()
{
	_delay_us(10);
}

