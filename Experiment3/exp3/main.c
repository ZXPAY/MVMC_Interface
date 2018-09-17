/*
    experiment 3
    date:2018/09/05 
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "ASA_Lib.h"
#include "lib/M128_Danny_SDC.h"
#include "lib/M128_Danny_ADC.h"
#include "lib/M128_Danny_Timer.h"

#define ADC2_NUM 2 // port F
#define ADC3_NUM 3 // port F
#define INPUT false
#define OUTPUT true

int dataADC2, dataADC3;

ISR(ADC_vect)
{
    static uint8_t ADC_Flag = 0;
    if(ADC_Flag == 0){
        setupADC2();
        dataADC2 = ADCL + (ADCH << 8);
        ADC_Flag = 1;
    }
	else if(ADC_Flag == 1){
        setupADC3();
        dataADC3 = ADCL + (ADCH << 8);
        ADC_Flag = 0;
    }
    // printf("Interrupt\n");
    setConversion();
}
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
    ASA_M128_set();
    uint32_t LoopCounter = 0;
    setIO(DDRF, ADC2_NUM, INPUT);
    setIO(DDRF, ADC3_NUM, INPUT);
    setupADC2();
    setupADC3();
    setFreeRunning(true);

    setTimer0_CompareInterrupt(255, 1024);
    setTimer1B_CompareInterrupt(65535, 1024);
    setTimer2_CompareInterrupt(255, 1024);
    setTimer3B_CompareInterrupt(65535, 1024);

    printf("Run Main Code ~~~\n");
    // Main code
    
    while(true){
        int mode;
        bool runFlag = true;
        printf("Please Enter Mode ~\n");
        scanf("%d", &mode);
        switch(mode){
            case 0:
                while(runFlag){
                    printf("Measure 0 V, enter any key to continue ...\n");
                    int any;
                    float voltage1;
                    scanf("%d", &any);
                    voltage1 = analogReadADC2()*5/1024;
                    printf("0V-> %.2f\n", voltage1);
                    printf("Measure 1.23 V, enter any key to continue ...\n");
                    int any2;
                    float voltage2;
                    scanf("%d", &any2);
                    voltage2 = analogReadADC2()*5/1024;
                    printf("1.23V-> %.2f\n", voltage2);
                    
                    printf("\n-> Gain is %.2f\n", 1.23/(voltage2-voltage1));

                    printf("\nAgain ? (0/1)");
                    scanf("%d", &runFlag);
                }
            break;
            case 1:

            break;
            case 11:
                 setInterruptADCEnable(true);
            break;
            case 12:
                 setInterruptADCEnable(false);
            break;
            case 13:
                setFreeRunning(true);
            break;
            case 14:
                setFreeRunning(false);
            break;
            case 15:
                printf("ADC2->%d\n", analogReadADC2());
            break;
            case 16:
                printf("ADC3->%d\n", analogReadADC3());
            break;
            case 17:
                printf("->%d,%d\n", dataADC2, dataADC3);
            break;
            case 18:
                createFile("test1", "bin");
            break;
            case 19:
                createFile("test1", "txt");
            break;
            case 20:
                setSDCName("test1", "txt");
                setSDCContinuous();
                char send[MAX_WORDS_SIZE] = "Hello\r\n";
                setSDCWords(send);
                closeSDC();
            break;
            case 21:
                setSDCName("test1", "txt");
                setSDCRead();
                char data[32];
                getSDCData(data, 32);
                for(int i=0;i<32;i++) printf("%c", data[i]);
                closeSDC();
            break;
            case 22:
                setSDCName("test1", "txt");
                setSDCRead();
                unsigned int dataTime;
                getSDCTime(dataTime);
                printf("datatime is %d\n", dataTime);
                closeSDC();
            break;
            case 23:
                setSDCName("test1", "txt");
                setSDCRead();
                char GoodOrNot;
                getSDCResponse(GoodOrNot);
                printf("datatime is %d\n", GoodOrNot);
                closeSDC();
            break;
            case 24:
                printf("Open which timer\n");
                int a;
                scanf("%d", &a);
                openWhichTimer(a);
            break;
            case 25:
                printf("Close which timer\n");
                int b;
                scanf("%d", &b);
                closeWhichTimer(b);
            break;
            case 29:
                testSDC();
            break;
            default:
                printf("Warning !!! No mode found !!!\n");
            break;
        }
        
    }

}

