/*
 * exp4_test.c
 *
 * Created: 2018/9/17 下午 12:55:13
 * Author : zxpay
 */ 

#define F_CPU 11059200
#include <avr/io.h>
#include <stdbool.h>
#include "lib/ASA_Lib.h"
#include "lib/M128_Danny_Timer.h"

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

/*
	Test PWM using PB7 (OC2/OC1C)
	focnPWM = fclk/(N*(1+Top))   ==> 1/focnPWM = period
	set OCRX to change duty cycle, if use OC1(timer1), OCR1 0~255,
	OCR0 0 means always 0 V, OCR0 255 means always 5 V.
	Prescaler frequency
*/

int main(void){
    // Setup, Only run one time.
    ASA_M128_set();

    //TCCR0 |= (1<<WGM00) | (1<<WGM01) | (1<<CS00) | (1<<CS02) | (1<<COM01);
    //DDRB |= (1<<4);
	
	// Fast PWM mode, prescaler 1024
	setTimerPrescaler(1, 1);
	setTimer1Mode(14);
	setPWMTimer1Channel(ChannelA);
	setPWMTimer1Channel(ChannelB);
	setPWMTimer1Channel(ChannelC);
	setPWMFrequency(5000);
	setPWMDutyCycle(ChannelA, 20);
	setPWMDutyCycle(ChannelB, 50);
	setPWMDutyCycle(ChannelC, 80);
	
    // Main code
    
    while(true){
	    int mode;
		uint16_t OCRdata;
		uint16_t prescalerLevel;
	    printf("Please Enter Mode ~\n");
	    scanf("%d", &mode);
	    switch(mode){
			// Change Duty cycle
		    case 0:
				printf("Enter OCR0 data\n");
				OCRdata = 0;
				scanf("%d", &OCRdata);
				OCR0 = OCRdata;
			break;

			// change period
			case 1:
				printf("Enter prescalarLevel\n");
				prescalerLevel = 0;
				scanf("%d", &prescalerLevel);
				printf("Prescalar is %d\n", getTimer0PrescalerLevel(prescalerLevel));
				TCCR0 = 0;
				TCCR0 |= (1<<WGM00) | (1<<WGM01) | (1<<COM01) | getTimer0PrescalerLevel(prescalerLevel);
				printf("TCCR : %d\n", TCCR0);
			break;
			case 2:
				printf("Enter Frequency for ICR\n");
				uint16_t freqICR;
				scanf("%d", &freqICR);
				ICR1 = (11059200)/(freqICR) - 1;
				printf("ICR is %d\n", ICR1);
			break;
			case 3:
				printf("Enter prescalarLevel\n");
				prescalerLevel = 0;
				scanf("%d", &prescalerLevel);
				printf("Prescaler is %d\n", getTimer123PrescalerLevel(prescalerLevel));
				TCCR1A = 0;
				TCCR1B = 0;
				TCCR1A |= (1<<WGM11) | (1<<COM1C1);
				TCCR1B |= (1<<WGM12) | (1<<WGM13) | getTimer123PrescalerLevel(prescalerLevel);
				printf("TCCRA : %d\n", TCCR1A);
				printf("TCCRB : %d\n", TCCR1B);
			break;
			case 4:
				printf("Enter Frequency for OCRA\n");
				uint16_t freqOCRA;
				scanf("%d", &freqOCRA);
				//OCR1A = (11059200)/(8*freqICR) - 1;
				OCR1A = freqOCRA;
				printf("OCR1A is %d\n", OCR1A);
			break;
			case 5:
				printf("Enter Frequency for OCRB\n");
				uint16_t freqOCRB;
				scanf("%d", &freqOCRB);
				//OCR1A = (11059200)/(8*freqICR) - 1;
				OCR1B = freqOCRB;
				printf("OCR1A is %d\n", OCR1B);
			break;
			case 6:
				printf("Enter Frequency for OCRC\n");
				uint16_t freqOCRC;
				scanf("%d", &freqOCRC);
				//OCR1A = (11059200)/(8*freqICR) - 1;
				OCR1C = freqOCRC;
				printf("OCRC is %d\n", OCR1C);
			break;
			case 7:
				printf("Enter Frequency for freqTCCR1A\n");
				uint16_t freqTCCR1A;
				scanf("%d", &freqTCCR1A);
				//OCR1A = (11059200)/(8*freqICR) - 1;
				TCCR1A = freqTCCR1A;
				printf("freqTCCR1A is %d\n", freqTCCR1A);
			break;
			case 8:
				printf("Enter Frequency for freqTCCR1B\n");
				uint16_t freqTCCR1B;
				scanf("%d", &freqTCCR1B);
				//OCR1A = (11059200)/(8*freqICR) - 1;
				TCCR1B = freqTCCR1B;
				printf("freqTCCR1B is %d\n", freqTCCR1B);
			break;
			default:
				printf("Warning !!! No mode found !!!\n");
			break;
	    }
	    
    }
}

