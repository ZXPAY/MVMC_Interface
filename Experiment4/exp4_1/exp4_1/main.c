/*
 * exp4_1.c
 *
 * Created: 2018/9/19 下午 01:56:52
 * Author : zxpay
 */ 
#define F_CPU 11059200
#include <avr/io.h>
#include "ASA_Lib.h"
#include "M128_Danny_Timer.h"
#define pi 3.141592
#define MaxSinDataNumbers 200

uint16_t sinData[MaxSinDataNumbers];
int SinDataNumbers;

// 10  -> 1388.89 hz  ps:200 dB才聽得到
// 20  -> 666.67 hz
// 30  -> 454.54 hz
// 40  -> 333.33 hz
// 50  -> 277.78 hz
// 100 -> 133 hz
// 150 -> 90.91 hz
// 200 -> 66.67 hz

volatile int cnt=0;
ISR(TIMER0_COMP_vect){
	PORTA |= (1<<0);
	setPWMDutyCycle(ChannelC, sinData[cnt]);
	cnt++;
	if(cnt >= SinDataNumbers){
		cnt = 0;
	}
	PORTA &= ~(1<<0);
}

int main(void){
    ASA_M128_set();
	float fr;
	printf("Enter freq\n");
	scanf("%f", &fr);
	SinDataNumbers = 66.67/(fr/200);
	DDRA |= (1<<0);
	uint32_t freq;
	printf("Enter PWM Frequency ...\n");
	scanf("%lu", &freq);
	setTimerPrescaler(1, 1);
	setTimer1Mode(14);
	setPWMTimer1Channel(ChannelC);
	setPWMFrequency(freq);
	
	for(int n=0;n<SinDataNumbers;n++){
		sinData[n] = ((sin(2*pi*n/SinDataNumbers))+1) * 50; 
	}
	
	setTimer0_CompareInterrupt(13, 8);
	openTimer0();
	
	printf("Run main code ~~~\n");
    while (1){
		float frequency;
		printf("Enter frequency \n");
		scanf("%f", &frequency);
		closeTimer0();
		SinDataNumbers = 66.67/(frequency/200);
		for(int n=0;n<SinDataNumbers;n++){
			sinData[n] = ((sin(2*pi*n/SinDataNumbers))+1) * 50;
		}
		cnt = 0;
		openTimer0();
    }
}

