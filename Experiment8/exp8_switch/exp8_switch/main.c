/*
 * exp8_switch.c
 *
 * Created: 2018/11/5 下午 02:42:16
 * Author : zxpay
 */ 

#define F_CPU 11059200
#include <avr/io.h>
#include "ASA_Lib.h"
#include <inttypes.h>
#include <stdbool.h>
#define  PRESS_CONFIDENCE 10

int main(void){
	ASA_M128_set();
	DDRC &= ~(1<<PC7);
	
    /* Replace with your application code */
	printf("Main code start !!!\n");
	uint16_t cnt = 0;
	bool state = false;
    while (1) {
		uint8_t data = (PINC&(1<<PC7))>>PC7;
		if(data == 1){
			cnt++;
			if(cnt>PRESS_CONFIDENCE && state==false){
				printf("Pressed\n");
				state = true;
			}
			
		}
		else{
			state = false;
			cnt = 0;
		}
    }
}

