/*
 * exp5_tmp121_SPI.c
 *
 * Created: 2018/10/5 上午 11:46:21
 * Author : zxpay
 */ 
#define F_CPU 11059200
#include <avr/io.h>
#include <stdbool.h>
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
uint8_t getSPIData();

int main(void)
{
	ASA_M128_set();

	double tempData;
	uint8_t H,L;
	printf("start!-----------------------------\n");
	// set SPI MOSI
	DDRB |= (1<<PIN_MOSI) | (1<<PIN_SCK) | (1<<PIN_SS);
	SPCR |= (1<<SPR0);  // divide clock 16
	SPCR |= (1<<MSTR);  // SPI master mode
	SPCR |= (1<<SPE);  // enable SPI
	ASA_ID_init();
	ASA_ID_set(1);
	ASA_cs_init();
	while (1){
		_delay_ms(250);          //轉換時間長，SCK腳位開始傳輸時脈後須維持 250ms才能完成一次轉換
		ASA_cs(1);               //致能 falling trigger, 轉換到ASA電路 會相反
		H = getSPIData(); //receive the High byte from TM121
		L = getSPIData(); //receive the Low  byte from TM121
		ASA_cs(0);               //禁能
		tempData = (double)((H<<5) + (L>>3)) *0.0625; //translate 2byte data to temperature
		printf("temperature = %.2f C\n", tempData);
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

uint8_t getSPIData(){
	SPDR = 0;        // transmit the byte to be sent

	// wait for the transfer to complete. if wait time > 1us, then break
	while ( !(SPSR & (1<<SPIF)));
	return SPDR;       // then return the byte the slave just returned
}
