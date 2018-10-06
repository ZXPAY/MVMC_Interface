/*
 * exp4_1.c
 *
 * Created: 2018/9/19 下午 01:56:52
 * Author : zxpay
 */ 
#define F_CPU 11059200
#include <avr/io.h>
#include <stdbool.h>
#include "lib/SDC00/ASA_lib_SDC00.h"
#include "lib/M128_Danny_SDC.h"
#include "ASA_Lib.h"
#include "M128_Danny_Timer.h"
#define pi 3.141592
#define MaxSinDataNumbers 200
#define PWMFrequency 20000
#define SDC_FILE_NAME "a"
#define dataNumbers 100

float data[dataNumbers];

uint16_t sinData[MaxSinDataNumbers];
int SinDataNumbers;

// Pin:
//     OC1A : PB5
//     OC1B : PB6
//     OC1C : PB7

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
	
	//setPWMDutyCycle(ChannelC, data[cnt]);
	setPWMDutyCycle(ChannelC, sinData[cnt]);
	//OCR1C = data[cnt];
	cnt++;
	if(cnt >= SinDataNumbers){
		cnt = 0;
	}
	
}

int main(void){
    ASA_M128_set();

/*
	setSDCName("A", "TXT");
	setSDCRead();
	for(int k=0;k<1000;k++){
		char readBuffx[6];
		int calculateDatax = 0;
		initBuffer(readBuffx);
		getSDCData(readBuffx, 6);
		int digitsx = getBufferSizeUseCRLF(readBuffx);
	}
	for(uint16_t n=0;n<dataNumbers;n++){
		//PORTA |= (1<<1);
		char readBuff[6];
		int calculateData = 0;
		initBuffer(readBuff);
		getSDCData(readBuff, 6);
		int digits = getBufferSizeUseCRLF(readBuff);
		// printf("digits:%d\n", digits);
		
		for(int k=0;k<digits;k++){
			int dpow=1;
			for(int j=0;j<(digits-k-1);j++) dpow *= 10;
			calculateData += dpow*ascii2num(readBuff[k]);
			// printf("%d\n", ascii2num(readBuff[k]));
		}
		//data[n] = calculateData*25/256;
		data[n] = calculateData*25/256;
		printf("%d\n", calculateData);
		//PORTA &= ~(1<<1);
	}
	closeSDC();*/
	
	float fr;
	printf("Enter freq\n");
	scanf("%f", &fr);
	SinDataNumbers = 66.67/(fr/200);
	for(int n=0;n<SinDataNumbers;n++){
		sinData[n] = ((sin(2*pi*n/SinDataNumbers))+1) * 50;
		printf("->%d\n", sinData[n]);
	}
	setTimerPrescaler(1, 1);
	setTimer1Mode(14);
	setPWMTimer1Channel(ChannelC);
	setPWMFrequency(PWMFrequency);
	
	setTimer0_CompareInterrupt(13, 1);
	openTimer0();
	
	printf("Run main code ~~~\n");
    while (1){

/*
		float frequency;
		printf("Enter frequency \n");
		scanf("%f", &frequency);
		closeTimer0();
		SinDataNumbers = 66.67/(frequency/200);
		for(int n=0;n<SinDataNumbers;n++){
			sinData[n] = ((sin(2*pi*n/SinDataNumbers))+1) * 50;
		}
		cnt = 0;
		openTimer0();*/
    }
}

