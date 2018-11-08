/*
 * exp9_brushless_motor.c
 *
 * Created: 2018/11/7 下午 04:58:20
 * Author : zxpay
 */ 
#define F_CPU 1105922
#include <avr/io.h>
#include "ASA_Lib.h"
#include "M128_Danny_Timer.h"

volatile char flag = 1;
ISR(TIMER0_COMP_vect){
	if(flag == 1){
		PORTA = 0b00100101;
		flag = 2;
	}
	else if(flag == 2){
		PORTA = 0b01100001;
		flag = 3;
	}
	else if(flag == 3){
		PORTA = 0b01000011;
		flag = 4;
	}
	else if(flag == 4){
		PORTA = 0b01010010;
		flag = 5;
	}
	else if(flag == 5){
		PORTA = 0b00010110;
		flag = 6;
	}
	else if(flag == 6){
		PORTA = 0b00110100;
		flag = 1;
	}
}


int main(void){
    ASA_M128_set();
	setTimer0_CompareInterrupt(255, 128);  // 120000 Hz
	
	DDRA = 0xff;
	
	printf("Run main code ~~~\n");
    openTimer0();
	for(int i=255;i>=40;i--){
		OCR0 = i;
		_delay_ms(1000);
	}
	while (1) {

    }
	return 0;
}

/*
	TCCR1A &= ~(1<<COM1A0);
	TCCR1A |= (1<<COM1A1);
	
	TCCR1A &= ~(1<<COM1B0);
	TCCR1A |= (1<<COM1B1);
	
	TCCR1A &= ~(1<<COM1C0);
	TCCR1A |= (1<<COM1C1);
	
	TCCR1A |= (1<<WGM10);
	TCCR1B |= (1<<WGM12)|(1<<CS11);
	
	DDRB |= (1<<PB5)|(1<<PB6)|(1<<PB7);
	
	
	OCR1A = 128;
	OCR1B = 128;
	OCR1C = 128;

*/