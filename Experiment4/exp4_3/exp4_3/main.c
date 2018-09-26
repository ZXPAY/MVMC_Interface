/*
 * exp4_3.c
 *
 * Created: 2018/9/26 上午 11:34:00
 * Author : zxpay
 */ 
#define F_CPU 11059200
#include <avr/io.h>
#include <stdbool.h>
#include "ASA_Lib.h"
#include "M128_Danny_Timer.h"

#define pi 3.141592

void setPWMOC0();
void setPWMOC1A();

int main(void){
    ASA_M128_set();
	uint8_t sinData[100];
	for(int i=0;i<100;i++){
		sinData[i] = (sin(pi*i/100)) * 255;
		printf("%d\n", sinData[i]);
	}
	setPWMOC0();
	setPWMOC1A();
	printf("Run main code ~~~\n");
    while (1) {
		for(int i=0;i<100;i++){
			OCR1A = 0;
			OCR0 = sinData[i];
		}
		for(int i=0;i<100;i++){
			OCR0 = 0;
			OCR1A = sinData[i];
		}
    }
}

// freq = 5400 hz
void setPWMOC0(){
/*
	Test PWM using PB4
	focnPWM = fclk/(N*256)   ==> 1/focnPWM = period
	set OCRX to change duty cycle, if use OC0(timer0), OCR0 0~255,
	OCR0 0 means always 0 V, OCR0 255 means always 5 V.
	Prescaler frequency
	1    43200.0
	8    5400.0
	32   1350.0
	64   675.0
	128  337.5
	256  168.75
	1024 42.1875  
*/
	DDRB |= (1<<4);
	// period is 
	TCCR0 = 0;
	// fast PWM mode
	TCCR0 |= (1<<WGM00) | (1<<WGM01);
	// Clear OC0 on compare match, set OC0 at BOTTOM (non-inverting mode)
	TCCR0 |= (1<<COM01);
	// prescaler 8 -> can change period
	TCCR0 |= (1<<CS01);
	
	//OCR0 = 128;  // 50% duty cycle
}

// freq = 5400 hz
void setPWMOC1A(){
/*
	Test PWM using PB7 (OC2/OC1C)
	focnPWM = fclk/(N*(1+Top))   ==> 1/focnPWM = period
	set OCRX to change duty cycle, if use OC1(timer1), OCR1 0~255,
	OCR0 0 means always 0 V, OCR0 255 means always 5 V.
	Prescaler frequency
*/
	DDRB |= (1<<5);
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
	ICR1 = 255;   // freq = 43200 hz
	// OCR : change duty cycle
	//OCR1A = 128;
}



