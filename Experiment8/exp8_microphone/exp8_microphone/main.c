/*
 * exp8_microphone.c
 *
 * Created: 2018/11/6 下午 12:34:15
 * Author : zxpay
 */ 
#define  F_CPU 11059200
#include <avr/io.h>
#include <stdbool.h>
#include <inttypes.h>
#include "ASA_Lib.h"
#include "M128_Danny_ADC.h"
#include "M128_Danny_Timer.h"

void collectData();
void sendData();
volatile bool OK_FLAG = false;

struct collect{
	uint16_t data[101];
}collect;

volatile uint16_t cnt = 0;
volatile uint16_t consumeC = 0;
ISR(TIMER0_COMP_vect){
	ADCSRA |= (1 << ADSC); // Start A2D Conversions
	collect.data[cnt] = (ADCL + (ADCH << 8));
	cnt++;
	if(cnt>=100) {
		cnt = 0; 
		closeTimer1B(); 
		collect.data[100] = consumeC;
		consumeC = 0; 
		OK_FLAG=true; 
		closeTimer0();
		}
}
ISR(TIMER1_COMPB_vect){
	consumeC++;
}

int main(void){
	ASA_M128_set();
    setupADC1();
	setTimer0_CompareInterrupt(12, 2);   // freq=>400000 Hz 
	setTimer1B_CompareInterrupt(172, 64);   // time comsume period : 1 ms
	
	printf("Run Main Code ~~~\n");
    while (1) {
		int press;
		printf("Turn angle and ready to run ,enter 1 to run\n");
		scanf("%d", &press);
		ADCSRA |= (1 << ADSC); // Start A2D Conversions
		openTimer1B();
		openTimer0();
		while(OK_FLAG == false);
		printf("Ready to send ...\n");
		sendData();
		
    }
}


void sendData(){
	char check = M128_HMI_Form_put("ui16x101", 202, &collect);
	if(check) printf("Error -> %d\n", check);
}
