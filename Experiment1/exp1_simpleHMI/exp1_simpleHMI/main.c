/*
 * exp1_simpleHMI.c
 *
 * Created: 2018/8/7 下午 01:03:08
 * Author : zxpay
 */ 
#define F_CPU 11592000
#include <avr/io.h>
#include <inttypes.h>
#include "ASA_Lib.h"
#include "testHMI.h"

int main(void){
	ASA_M128_set();
    printf("main code start !!!\n");
    while (1) {
		uint16_t LoopCounter = 0;
		int mode;
		printf("\nPlease Enter Mode ?\n");
		scanf("%d", &mode);
		switch(mode){
			case 0:
				printf("Run mode 0 \n");
				testM128_HMI_put();
			break;
			case 1:
				printf("Run mode 1 \n");
				testM128_HMI_get();
			break;
			case 2:
				while(LoopCounter<2000){
					
					LoopCounter++;
				}
			break;
		}
    }
}

