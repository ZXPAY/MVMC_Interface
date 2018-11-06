/*
 * exp8_delta_resistor.c
 *
 * Created: 2018/11/5 下午 03:43:28
 * Author : zxpay
 */ 
#define F_CPU 11059200
#include <avr/io.h>
#include "ASA_Lib.h"
#include <inttypes.h>
#include <stdbool.h>

/*
	Using PF1 (ADC1) to collect data
	Experiment for photo resistor and potenmeters
*/
struct collect{
	uint16_t data[100];
}collect;

uint16_t ADC_Value = 0;

void updateADC();
void collectData();
void sendData();

int main(void){
	ASA_M128_set();
	DDRF &= ~(1<<PF1);
	ADMUX &= ~(1<<REFS1);
	ADMUX |= (1<<REFS0) | (1<<MUX0);
	ADCSRA |= (1 << ADPS0);
	ADCSRA |= (1<<ADEN);
	
    /* Replace with your application code */
	printf("Run main code ~~~\n");
	int press = 0;
    while (1) {
		for(int n=1;n<=5;n++){
			printf("Turn angle and ready to run %d, enter 1 to run\n", n);
			scanf("%d", &press);
			collectData();
			printf("Ready to send %d...\n", n);
			sendData();
		}
    }
}

void updateADC(){
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA&(1<<ADIF)));
	ADCSRA |= (1<<ADIF);
	ADC_Value = ADCL + (ADCH<<8);
}

void collectData(){
	for(int i=0;i<100;i++){
		updateADC();
		collect.data[i] = ADC_Value;
	}
}

void sendData(){
	char check = M128_HMI_Form_put("ui16x100", 200, &collect);
	if(check) printf("Error -> %d\n", check);
}
