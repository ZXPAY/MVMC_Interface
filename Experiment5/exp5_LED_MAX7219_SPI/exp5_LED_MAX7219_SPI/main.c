/*
 * exp5_LED_MAX7219_SPI.c
 *
 * Created: 2018/10/5 上午 11:46:21
 * Author : zxpay
 */ 
#define F_CPU 11059200
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ASA_Lib.h"

#define PIN_MOSI PB2
#define PIN_MISO PB3
#define PIN_SCK PB1
#define PIN_SS PB0
#define PIN_ASA_CS PF4

void ASA_ID_set(uint8_t data);
void ASA_ID_init();
void ASA_cs_init();
void ASA_cs(uint8_t en);
void setSPIData(uint8_t cData);
void setSPITransfer(uint8_t address, uint8_t value);
void init8_8Matrix();
void testZeroToZ();
void setPatternTo8_8Matrix(uint8_t *data);
void setPatternAnimation(uint8_t* data, uint8_t shift_left_times, uint8_t shift_right_times, uint16_t delayTime, bool leftToRight);
void testArrowAnimation();

uint8_t smile[8] = {  
	0b00000000,
	0b01000010,
	0b00000000,
	0b00011000,
	0b01000010,
	0b01000010,
	0b00100100,
	0b00011000
	};
uint8_t arrow_right[8] = {
	0b00000000,
	0b00000000,
	0b00010000,
	0b00001000,
	0b11111100,
	0b00001000,
	0b00010000,
	0b00000000
	};
uint8_t arrow_left[8] = {
	  0b00000000,
	  0b00000000,
	  0b00100000,
	  0b01000000,
	  0b11111100,
	  0b01000000,
	  0b00100000,
	  0b00000000
	};
uint8_t middle_finger[8] = {
	  0b00011000,
	  0b00011000,
	  0b00011000,
	  0b00011000,
	  0b11111110,
	  0b10101010,
	  0b10101011,
	  0b11111110
};

uint8_t disp1[38][8]={
	{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C}, //Char 0
	{0x10,0x18,0x14,0x10,0x10,0x10,0x10,0x10}, //Char 1
	{0x7E,0x2,0x2,0x7E,0x40,0x40,0x40,0x7E}, //Char 2
	{0x3E,0x2,0x2,0x3E,0x2,0x2,0x3E,0x0}, //Char 3
	{0x8,0x18,0x28,0x48,0xFE,0x8,0x8,0x8}, //Char 4
	{0x3C,0x20,0x20,0x3C,0x4,0x4,0x3C,0x0}, //Char 5
	{0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x0}, //Char 6
	{0x3E,0x22,0x4,0x8,0x8,0x8,0x8,0x8}, //Char 7
	{0x0,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E}, //Char 8
	{0x3E,0x22,0x22,0x3E,0x2,0x2,0x2,0x3E}, //Char 9
	{0x8,0x14,0x22,0x3E,0x22,0x22,0x22,0x22}, //Char A
	{0x3C,0x22,0x22,0x3E,0x22,0x22,0x3C,0x0}, //Char B
	{0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x0}, //Char C
	{0x7C,0x42,0x42,0x42,0x42,0x42,0x7C,0x0}, //Char D
	{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C}, //Char E
	{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40}, //Char F
	{0x3C,0x40,0x40,0x40,0x40,0x44,0x44,0x3C}, //Char G
	{0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44}, //Char H
	{0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C}, //Char I
	{0x3C,0x8,0x8,0x8,0x8,0x8,0x48,0x30}, //Char J
	{0x0,0x24,0x28,0x30,0x20,0x30,0x28,0x24}, //Char K
	{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C}, //Char L
	{0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81}, //Char M
	{0x0,0x42,0x62,0x52,0x4A,0x46,0x42,0x0}, //Char N
	{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C}, //Char O
	{0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20}, //Char P
	{0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D}, //Char Q
	{0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21}, //Char R
	{0x0,0x1E,0x20,0x20,0x3E,0x2,0x2,0x3C}, //Char S
	{0x0,0x3E,0x8,0x8,0x8,0x8,0x8,0x8}, //Char T
	{0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C}, //Char U
	{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18}, //Char V
	{0x0,0x49,0x49,0x49,0x49,0x2A,0x1C,0x0}, //Char W
	{0x0,0x41,0x22,0x14,0x8,0x14,0x22,0x41}, //Char X
	{0x41,0x22,0x14,0x8,0x8,0x8,0x8,0x8}, //Char Y
	{0x0,0x7F,0x2,0x4,0x8,0x10,0x20,0x7F}, //Char Z
};


int main(void)
{
	ASA_M128_set();

	// set SPI pin as output
	DDRB |= (1<<PIN_MOSI) | (1<<PIN_SCK) | (1<<PIN_SS);
	SPCR |= (1<<SPR0);  // divide clock 16
	SPCR |= (1<<MSTR);  // SPI master mode
	SPCR |= (1<<SPE);  // enable SPI
	ASA_ID_init();
	ASA_ID_set(1);
	ASA_cs_init();
	init8_8Matrix();
	printf("Run Main Code ~\n");
	while (1){        
		int mode;
		int light, scanLimit, shutDown;
		printf("Please enter mode ~~~\n");
		scanf("%d", &mode);
		switch(mode){
			case 0:
				printf("Enter intensity you want to change (0~15) \n");
				scanf("%d", &light);
				if (light>15) light = 15;
				else if(light<0) light = 0;
				setSPITransfer(0x0a, light);   // set light intensity, 0 ~ 0x0f
			break;
			case 1:
				printf("Enter scan limit you want to change (0~7) \n");
				scanf("%d", &scanLimit);
				if (scanLimit>7) scanLimit = 7;
				else if(scanLimit<0) scanLimit = 0;
				setSPITransfer(0x0b, scanLimit);
			break;
			case 2:
				printf("Enter shutDown you want to change (0/1) \n");
				scanf("%d", &shutDown);
				if (shutDown>1) shutDown = 1;
				else if(shutDown<0) shutDown = 0;
				setSPITransfer(0x0c, shutDown);   // set light intensity, 0 ~ 0x0f
			break;
			case 3:
				testArrowAnimation();
			break;
			case 4:
				testZeroToZ();
			break;
			case 5:
				setPatternTo8_8Matrix(&smile[0]);
			break;
			case 6:
				for(int i=0;i<10;i++){
					setPatternAnimation(&middle_finger[0], 2, 2, 200, true);
					setPatternAnimation(&middle_finger[0], 2, 2, 200, false);
				}
				setPatternTo8_8Matrix(&smile[0]);
			break;
			case 97:
			// lamb test
			setSPITransfer(0x0f, 0x01);
			break;
			case 98:
				// lamb test
				setSPITransfer(0x0f, 0x00);
			break;
			case 99:
				// clear
				for(int i=1;i<9;i++){
					setSPITransfer(i, 0);
				}
			break;
		}
		
		printf("Run main code ~~~\n");
	}
	return 0;
}

void ASA_ID_set(uint8_t data) {
	if (data>7) {
		return;
	}
	PORTB &= ~(7<<PB5);
	PORTB |= (data<<PB5);
};

void ASA_ID_init() {
	DDRB |= (1<<PB7)|(1<<PB6)|(1<<PB5);
};

void ASA_cs_init(){
	DDRF |= (1<<PIN_ASA_CS);
}

void ASA_cs(uint8_t en) {
	if (en>1) {
		return;
	}
	if (en == 0) {
		PORTF &= ~(1<<PIN_ASA_CS);
		} else {
		PORTF |= (1<<PIN_ASA_CS);
	}
}

void setSPIData(uint8_t cData){
	SPDR = cData;        // transmit the byte to be sent

	while ( !(SPSR & (1<<SPIF)));
}

void init8_8Matrix(){
	setSPITransfer(0x09, 0x00);   // decode mode -> No decode for D0~D7
	setSPITransfer(0x0a, 0x03);   // set light intensity, 0 ~ 0x0f
	setSPITransfer(0x0b, 0x07);   // set scan limit
	setSPITransfer(0x0c, 0x01);   // shutdown
	setSPITransfer(0x0f, 0x00);   // display test
}

void setSPITransfer(uint8_t address, uint8_t value){
	_delay_ms(2);
	ASA_cs(1);      //致能 falling trigger, 轉換到ASA電路 會相反
	setSPIData(address);
	setSPIData(value);
	ASA_cs(0);
}

void setPatternTo8_8Matrix(uint8_t* data){
	for(int i=1;i<9;i++){
		setSPITransfer(i, *(data+i-1));
	}
}

void setPatternAnimation(uint8_t* data, uint8_t shift_left_times, uint8_t shift_right_times, uint16_t delayTime, bool leftToRight){
	uint8_t *ptr;
	ptr = malloc(8);
	int n, shift_n;
	if(leftToRight){
		for(shift_n=shift_left_times;shift_n>=0;shift_n--){
			for(n=0;n<8;n++){
				*(ptr+n) = *(data+n)<<shift_n;
			}
			setPatternTo8_8Matrix(ptr);
			for(int d=0;d<delayTime;d++) _delay_ms(1);
		}
		for(shift_n=1;shift_n<shift_right_times;shift_n++){
			for(n=0;n<8;n++){
				*(ptr+n) = *(data+n)>>shift_n;
			}
			setPatternTo8_8Matrix(ptr);
			for(int d=0;d<delayTime;d++) _delay_ms(1);
		}
	}
	else{
		for(shift_n=shift_right_times;shift_n>=0;shift_n--){
			for(n=0;n<8;n++){
				*(ptr+n) = *(data+n)>>shift_n;
			}
			setPatternTo8_8Matrix(ptr);
			for(int d=0;d<delayTime;d++) _delay_ms(1);
		}
		
		for(shift_n=1;shift_n<shift_left_times;shift_n++){
			for(n=0;n<8;n++){
				*(ptr+n) = *(data+n)<<shift_n;
			}
			setPatternTo8_8Matrix(ptr);
			for(int d=0;d<delayTime;d++) _delay_ms(1);
		}
			
	}

	
	
}

void testArrowAnimation(){
	setPatternAnimation(&arrow_right[0], 5, 8, 100, true);
	setPatternAnimation(&arrow_left[0], 7, 8, 100, false);
}

void testZeroToZ(){
	for(int y=0;y<38;y++) // Just replace the "j" to y...
	{
		for(int x=1;x<9;x++)
		setSPITransfer(x,disp1[y][x-1]);
		_delay_ms(100);
	}
}

