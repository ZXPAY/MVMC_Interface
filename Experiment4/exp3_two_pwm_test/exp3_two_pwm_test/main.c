/*
 * exp3_two_pwm_test.c
 *
 * Created: 2018/9/26 上午 09:14:00
 * Author : zxpay
 * 
 * 測試Atmega128的PWM功能
 */ 
#define F_CPU 11059200
#include <avr/io.h>
#include <stdbool.h>
#include "ASA_Lib.h"
#include "M128_Danny_Timer.h"

void testOC0();
void testOC1AMode5();
void testOC1AMode6();
void testOC1AMode7();
void testOC1AMode14();
void testOC1AMode15();
void testOC1B();
void testOC1C();
void testOC2();
void testOC3CMode14();  // --> not work

int main(void){
	ASA_M128_set();
	DDRB = 0xff;
    while (1) {
		printf("Enter mode ...\n");
		int mode;
		uint16_t OCRdata;
		uint16_t ICRdata;
		scanf("%d", &mode);
		switch(mode){
			case 0:
				printf("OCR0?\n");
				scanf("%d", &OCRdata);
				testOC0();
				OCR0 = OCRdata;
			break;
			case 1:
				printf("OCR1?\n");
				scanf("%d", &OCRdata);
				testOC1AMode5();
				OCR1A = OCRdata;
			break;
			case 2:
				printf("OCR1?\n");
				scanf("%d", &OCRdata);
				testOC1AMode6();
				OCR1A = OCRdata;
			break;
			case 3:
				printf("OCR1?\n");
				scanf("%d", &OCRdata);
				testOC1AMode7();
				OCR1A = OCRdata;
			break;
			case 4:
				printf("ICR1?\n");
				scanf("%d", &ICRdata);
				ICR1 = ICRdata;
				printf("OCR1?\n");
				scanf("%d", &OCRdata);
				OCR1A = OCRdata;
				testOC1AMode14();
			break;
			case 5:
				printf("OCR1?\n");
				scanf("%d", &OCRdata);
				testOC1AMode15();
				OCR1A = OCRdata;
			break;
			case 6:
				printf("OCR2?\n");
				scanf("%d", &OCRdata);
				testOC2();
				OCR2 = OCRdata;
			break;
			case 7:
				printf("ICR3?\n");
				scanf("%d", &ICRdata);
				ICR3 = ICRdata;
				printf("OCR3?\n");
				scanf("%d", &OCRdata);
				OCR3C = OCRdata;
				testOC3CMode14();
			break;
		}
    }
}

// fast PWM mode
void testOC0(){
	TCCR0 = 0;
	// fast PWM mode
	TCCR0 |= (1<<WGM00) | (1<<WGM01);
	// Clear OC0 on compare match, set OC0 at BOTTOM (non-inverting mode)
	TCCR0 |= (1<<COM01);
	// prescaler 8 -> can change period
	TCCR0 |= (1<<CS01);
}

void testOC1AMode5(){
	TCCR1A = 0;
	TCCR1B = 0;
	// Fast PWM mode 5
	TCCR1A |= (1<<WGM10);
	// channel A
	TCCR1A |= (1<<COM1A1);
	// prescaler 8
	TCCR1B |= (1<<CS01);
}
void testOC1AMode6(){
	TCCR1A = 0;
	TCCR1B = 0;
	// Fast PWM mode 6
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12);
	// channel A
	TCCR1A |= (1<<COM1A1);
	// prescaler 8
	TCCR1B |= (1<<CS01);
}
void testOC1AMode7(){
	TCCR1A = 0;
	TCCR1B = 0;
	// Fast PWM mode 7
	TCCR1A |= (1<<WGM10) | (1<<WGM11);
	TCCR1B |= (1<<WGM12);
	// channel A
	TCCR1A |= (1<<COM1A1);
	// prescaler 8
	TCCR1B |= (1<<CS01);
}
void testOC1AMode14(){
	TCCR1A = 0;
	TCCR1B = 0;
	// Fast PWM mode 14
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12) | (1<<WGM13);
	// channel A
	TCCR1A |= (1<<COM1A1);
	// prescaler 8
	TCCR1B |= (1<<CS01);
	
	// Notice : ICR must be bigger than OCR
	// ICR : change period 
	//ICR1 = 1000;
	// OCR : change duty cycle
	//OCR1A = 500;
}
void testOC1AMode15(){
	TCCR1A = 0;
	TCCR1B = 0;
	// Fast PWM mode 15
	TCCR1A |= (1<<WGM10) | (1<<WGM11);
	TCCR1B |= (1<<WGM12) | (1<<WGM13);
	// channel A, toggle 50% duty cycle
	TCCR1A |= (1<<COM1A0); 
	// prescaler 8
	TCCR1B |= (1<<CS01);
	OCR1A = 4096;
}

void testOC1B(){

}

void testOC1C(){

}

void testOC2(){
	TCCR2 = 0;
	// fast PWM mode
	TCCR2 |= (1<<WGM20) | (1<<WGM21);
	// Clear OC0 on compare match, set OC0 at BOTTOM (non-inverting mode)
	TCCR2 |= (1<<COM21);
	// prescaler 8
	TCCR2 |= (1<<CS21);
}

void testOC3CMode14(){
	TCCR3A = 0;
	TCCR3B = 0;
	// Fast PWM mode 14
	TCCR3A |= (1<<WGM31);
	TCCR3B |= (1<<WGM32) | (1<<WGM33);
	// channel A
	TCCR3A |= (1<<COM3C1);
	// prescaler 8
	TCCR3B |= (1<<CS31);
	
	// Notice : ICR must be bigger than OCR
	// ICR : change period
	//ICR1 = 1000;
	// OCR : change duty cycle
	//OCR1A = 500;
}

