#include "ASA_Lib_DAC00.h"
#include <avr\io.h>
// #include <avr\interrupt.h>
// #include <stdio.h>
// #include <util\delay.h>

//Internal Definitions
// General
#define ASA_ID_NUMS 8

// SPI
#define SPI_PORT	PORTB
#define SPI_PIN		PINB
#define SPI_DDR		DDRB
#define SPI_MISO	PB3
#define SPI_MOSI	PB2
#define SPI_SCK		PB1
#define SPI_SS      PB0

//DIO
#define DIO_PORT	PORTF
#define DIO_PIN		PINF
#define DIO_DDR		DDRF
#define DIO_RW_PORT	PORTG
#define DIO_RW_PIN	PING
#define DIO_RW_DDR	DDRG
#define DIO0		PF4
#define DIO1		PF5
#define DIO2		PF6
#define DIO3		PF7
#define DIO_RD		PG3
#define DIO_WR		PG4

//ASA_ID
#define ADDR_PORT	PORTB
#define ADDR_PIN	PINB
#define ADDR_DDR	DDRB
#define ADDR0       PB5
#define ADDR1       PB6
#define ADDR2       PB7

//Internal Function Prototypes
static void DAC_initialize(void);
// char ASA_DAC00_set(char ASA_ID, char LSByte, char Mask, char shift, char Data);
// char ASA_DAC00_put(char ASA_ID, char LSByte, char Bytes, void *Data_p);
static char SPI_Transfer(char Data);
static char SPI_Receive(void);
static char ASA_ID_set(unsigned char ASA_ID);
static char ASA_ID_check(unsigned char ASA_ID);

//Global Variables
static char DAC00_put_reg[ASA_ID_NUMS][2];
static char DAC00_get_reg[ASA_ID_NUMS][1];
static char DAC00_set_reg[ASA_ID_NUMS];

//Functions
static void DAC_initialize(void) {
	SPI_DDR|=0xF7;
	DIO_DDR|=0xF0;
	SPCR |= (1<<SPE)|(0<<DORD)|(1<<CPOL)|(1<<MSTR)|(0<<SPR0)|(0<<CPHA);
	SPSR |= (1<<SPI2X);
}

char ASA_DAC00_set(char ASA_ID, char LSByte, char Mask, char shift, char Data) {
	char temp=0;

	if(ASA_ID > 0x07||ASA_ID==0)
	{
		return 1;
	}
    DAC_initialize();
	ASA_ID_set(0);
	ASA_ID_set(ASA_ID);
	if(LSByte !=200)
	{
		return 2;
	}
	if(shift > 7)
	{
		return 3;
	}
	if (Mask==0X40&&shift==6&& Data==1)
	{

		DAC00_put_reg[(unsigned char)ASA_ID][0]= 0;
		DAC00_put_reg[(unsigned char)ASA_ID][1]= 208;
		SPI_Transfer(208);
		SPI_Transfer(0);
		return 0;
	}
	else
	{
	Data = (Data<<shift);
	temp = (temp & ~Mask)|(Data & Mask);
	DAC00_set_reg[(unsigned char)ASA_ID] = (DAC00_set_reg[(unsigned char)ASA_ID] & ~Mask)|(Data & Mask);
	return 0;
	}
}
char ASA_DAC00_put(char ASA_ID, char LSByte, char Bytes, void *Data_p) {
	if(ASA_ID > 0x07||ASA_ID==0) {
		return 1;
	}
	ASA_ID_set(0);
	ASA_ID_set(ASA_ID);	//select module
	DAC_initialize();
	if(LSByte > 0)
	{
		return 2;
	}
	if(Bytes!= 2)
	{
		return 3;
	}
	DAC00_put_reg[(unsigned char)ASA_ID][0]= ((unsigned char*)Data_p)[0];
	DAC00_put_reg[(unsigned char)ASA_ID][1]= ((unsigned char*)Data_p)[1]|DAC00_set_reg[(unsigned char)ASA_ID];
	SPI_Transfer(DAC00_put_reg[(unsigned char)ASA_ID][1]);
	SPI_Transfer(DAC00_put_reg[(unsigned char)ASA_ID][0]);

	return 0;
}

char ASA_DAC00_get(char ASA_ID, char LSByte, char Bytes, void *Data_p) {
	ASA_ID_set(ASA_ID);	//select module
	if(ASA_ID > 0x07||ASA_ID==0)
	{
		return 1;
	}
	if(LSByte<100||LSByte>101)
	{
		return 2;
	}
	if( LSByte==100)
	{
		if( Bytes==1)
		{
			((unsigned char*)Data_p)[0]=DAC00_set_reg[(unsigned char)ASA_ID];
		}
		else
		{
			return 3;
		}
	}
	if( LSByte==101)
	{
		if( Bytes==2)
		{
			((unsigned char*)Data_p)[0] = DAC00_put_reg[(unsigned char)ASA_ID][0];
			((unsigned char*)Data_p)[1] = DAC00_put_reg[(unsigned char)ASA_ID][1];
		}
		else
		{
			return 3;
		}
	}
	if(SPDR == 0xff)
	{
		return 4;
	}
	else
	{
		return 0;
	}
}

static char SPI_Transfer(char Data) {
	SPDR = Data;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}

static char SPI_Receive(void) {
	SPDR = 0x00;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}

static char ASA_ID_check(unsigned char ASA_ID) {
	if(ASA_ID>=0 && ASA_ID<=7)
		return 0;

	else
		return 1;
}

static char ASA_ID_set(unsigned char ASA_ID) {
	if(ASA_ID_check(ASA_ID)==0)
	{
		unsigned char ASA_ID_temp;

		ASA_ID_temp = ADDR_PORT;
		ASA_ID_temp &= ~(0x07 << ADDR0);
		ASA_ID_temp |= ((ASA_ID & 0x07) << ADDR0);
		ADDR_PORT  = ASA_ID_temp;
		return 0;
	}

	else
		return 1;
}
