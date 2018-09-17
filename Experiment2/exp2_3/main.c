/*
    experiment 3
    date:2018/09/05 
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "ASA_Lib.h"
#include "lib/DAC00/ASA_Lib_DAC00.h"
#include "lib/M128_Danny_SDC.h"
#include "lib/M128_Danny_ADC.h"
#include "lib/M128_Danny_Timer.h"

#define DAC_ID 1
#define DAC_LSBYTE 200
#define DAC_SET_CHANNEL_MASK 0x30
#define DAC_SET_CHANNEL_SHIFT 4
#define DAC_SET_ASYNCHRONOUS_MASK 0x80
#define DAC_SET_ASYNCHRONOUS_SHIFT 7
#define setSynchronous 0
#define setAsynchronous 1

#define ADC2_NUM 2 // port F
#define ADC3_NUM 3 // port F
#define GrabDataNumbers 200
#define INPUT false
#define OUTPUT true

#define HMI_TEST_NAME "matlab_data"
#define HMI_TEST_EXTENSION "txt"


int ascii2num(char asciiNum);

struct matlabStruct{
    uint16_t rawData[GrabDataNumbers];
}matlabStruct;

ISR(TIMER0_COMP_vect){

}
ISR(TIMER2_COMP_vect){
    
}
ISR(TIMER1_COMPB_vect){

}
ISR(TIMER3_COMPB_vect){
    
}

int main(){
    // Setup, Only run one time.
    char check;
    ASA_M128_set();
    createFile(HMI_TEST_NAME, HMI_TEST_EXTENSION);
    
    printf("Get data from matlab ...\n");
    // get matlab data
    check = M128_HMI_Form_get("ui16x200", 400, &matlabStruct);
    // if(check != 0) printf("Error ...%d\n", check);

    // save to SD card
    setSDCName(HMI_TEST_NAME, HMI_TEST_EXTENSION);
    setSDCContinuous();
    char sendBuff[32];
    int saveBuffSize[GrabDataNumbers];
    for(int i=0;i<GrabDataNumbers;i++){
        initBuffer(sendBuff);
        printf("Raw data is %d\n", matlabStruct.rawData[i]);
        sprintf(sendBuff, "%d\r\n", matlabStruct.rawData[i]);
        saveBuffSize[i] = getBufferSize(sendBuff);
        setSDCWords(sendBuff, getBufferSize(sendBuff));
    }
    closeSDC();

    // get data from SD card
    setSDCName(HMI_TEST_NAME, HMI_TEST_EXTENSION);
    setSDCRead();
    for(int i=0;i<GrabDataNumbers;i++){
        int dataDigit = 0;
        char getBuff[32];
        initBuffer(getBuff);
        check = getSDCData(getBuff, saveBuffSize[i]);
        for(int i=0;i<32;i++){
            if(getBuff[i] != '\0' && getBuff[i] != 10 && getBuff[i] != 13){
                dataDigit++;
            }
            else{
                break;
            }
        }
        // printf("Digits : %d, %d\n", dataDigit, saveBuffSize[i]);
        if(check == 0){
            int dataCalculate = 0;
            for(int i=0;i<dataDigit;i++){
                int ePow=1;
                for(int k=0;k<(dataDigit-i-1);k++) ePow *= 10;
                dataCalculate += (ePow * ascii2num(getBuff[i]));
                // printf("Buff[i] : %d\n", getBuff[i]);
            }
            printf("DAC Output : %d\n", dataCalculate);
            // output the DAC to show the diagram
            ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_CHANNEL_MASK, DAC_SET_CHANNEL_SHIFT, 0);  // set channel 0
            ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_ASYNCHRONOUS_MASK, DAC_SET_ASYNCHRONOUS_SHIFT, setAsynchronous);  // asynchronous
            check = ASA_DAC00_put(DAC_ID, 0, 2, &dataCalculate);
            if(check != 0) printf("DAC put Error ... %d\n", check);
                        
        }
    }
    closeSDC();

    while(true){
   
        // if(LoopCounter % 1000 == 0){
        //     printf("Run ...\n");
        // }
        // LoopCounter++;
    }

}


int ascii2num(char asciiNum){
    switch(asciiNum){
        case '0':
            return 0;
        break;
        case '1':
            return 1;
        break;
        case '2':
            return 2;
        break;
        case '3':
            return 3;
        break;
        case '4':
            return 4;
        break;
        case '5':
            return 5;
        break;
        case '6':
            return 6;
        break;
        case '7':
            return 7;
        break;
        case '8':
            return 8;
        break;
        case '9':
            return 9;
        break;
        default:
            printf("No Ascii found !!!\n");
            return 0;
        break;
    }
}


