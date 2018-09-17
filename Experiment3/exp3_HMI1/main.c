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

#define HMI_NAME "HMITEST"
#define HMI_EXTENSION "txt"

#define Buff_Max_Size 64

int ascii2num(char asciiNum);
void initCircleBuffer();
void setCircleBuffer(uint16_t data);
bool isCircleBufferFull();
uint16_t myADCData[Buff_Max_Size];
uint16_t getCircleBuffer(char pos);
uint16_t endCNT = 0;
uint16_t dataADC2 = 0, dataADC3 = 0;
uint32_t ADCCounter = 0;
uint32_t timer1Counter = 0;
uint16_t MAX_QUERY_NUMBERS = 10;
uint16_t RecordNumbers = 0;
bool OK_Flag = false;
bool BufferFullFlag;
struct CircleBuffer{
    uint16_t ADC3Buffer[Buff_Max_Size];
    uint16_t ADC3CounterStart;
    uint16_t ADC3CounterEnd;
}CircleBuffer;

char dataBuffer[16];

ISR(ADC_vect){
    char buff[8];
    if(!OK_Flag){
        setConversion();
        dataADC3 = ADCL + (ADCH << 8);
        myADCData[ADCCounter] = dataADC3;
        ADCCounter++;
        // sprintf(buff, "%d", dataADC3);
        // setSDCWords(buff);
        // printf("data is %d\n", dataADC3);
        if(ADCCounter >= MAX_QUERY_NUMBERS){     
            // closeTimer0();
            closeTimer1B();
            closeSDC();
            OK_Flag = true;
            printf("OK->%d, %lu\n", ADCCounter, timer1Counter);
        }
    }
}
ISR(TIMER0_COMP_vect){
    static bool ToggleFlag = true;
    if(ToggleFlag){
        PORTA |= (1<<7);
        ToggleFlag = false;
    }
    else{
        PORTA &= ~(1<<7);
        ToggleFlag = true;
    }
}
ISR(TIMER2_COMP_vect){
    
}
ISR(TIMER1_COMPB_vect){
    // char buff[8];
    // if(!OK_Flag){
    //     setConversion();
    //     dataADC3 = ADCL + (ADCH << 8);
    //     ADCCounter++;
    //     if(ADCCounter >= MAX_QUERY_NUMBERS){   
    //         closeSDC();
    //         // closeTimer0();
    //         closeTimer1B();
    //         OK_Flag = true;
    //         // printf("OK->%d, %d\n", ADCCounter, MAX_QUERY_NUMBERS);
    //     }
    //     setCircleBuffer(dataADC3);
    // }
    if(!OK_Flag){
        setConversion();
        dataADC3 = ADCL + (ADCH << 8);
        // myADCData[ADCCounter] = dataADC3;
        ADCCounter++;
        if(ADCCounter >= MAX_QUERY_NUMBERS){     
            closeTimer1B();
            OK_Flag = true;
            printf("OK->%d, %lu\n", ADCCounter, timer1Counter);
        }
    }
    timer1Counter++;
    setCircleBuffer(dataADC3);
}

ISR(TIMER3_COMPB_vect){
    
}

int main(){
    // Setup, Only run one time.
    char check;
    ASA_M128_set();
    uint32_t LoopCounter = 0;
    DDRA |= 1<<7;
    initCircleBuffer();

    setIO(DDRF, ADC3_NUM, INPUT);
    setupADC3();
    setConversion();
    setFreeRunning(true);
    setTimer1B_CompareInterrupt(21, 256);  // 2 k 
    // setTimer0_CompareInterrupt(11, 1024);  
    createFile(HMI_NAME, HMI_EXTENSION); // create new file
    printf("How many ADC data do you want ?");
    printf("Please enter the number do you want ...\n");
    uint16_t dataNumbers;
    // scanf("%d", &dataNumbers);
    M128_HMI_get(2, &dataNumbers);
    // if(dataNumbers>=600) dataNumbers = 600;
    printf("DataNumbers is %d\n", dataNumbers);
    MAX_QUERY_NUMBERS = dataNumbers;
    printf("Run Main code, start to get data ...\n");
    // Main code
    // char a[8]; uint16_t w = 123;
    // sprintf(a, "%d", w);
    // setSDCWords(a);
    // closeSDC();
    // printf("FEFE\n");
    // while(true);
    // openTimer0();
    // while(true);
    setSDCName(HMI_NAME, HMI_EXTENSION);
    setSDCContinuous();
    // _delay_ms(500);
    openTimer1B();
    // setInterruptADCEnable(true);
    DDRA |= (1<<5);
    while(true){
        // printf("%lu, %d, %lu\n", ADCCounter, dataADC3, timer1Counter);
        // _delay_ms(200);
        static int startCNT = 0;
        char dataBuff[6];
        cli();
        PORTA |= (1<<5);
        int end;
        if(CircleBuffer.ADC3CounterEnd < CircleBuffer.ADC3CounterStart){
            end = CircleBuffer.ADC3CounterEnd+Buff_Max_Size;
        }
        else{
                end = CircleBuffer.ADC3CounterEnd;
        }
    //    printf("S:%d, E:%d\n", CircleBuffer.ADC3CounterStart, end);
        for(int i=CircleBuffer.ADC3CounterStart;i<end;i++){
            initBuffer(dataBuffer);
            sprintf(dataBuff, "%d\r\n", getCircleBuffer(i));
            setSDCWords(dataBuff, 5);
            // printf("D:%d\n", getCircleBuffer(i));
            RecordNumbers++;
        }
        CircleBuffer.ADC3CounterStart = CircleBuffer.ADC3CounterEnd;     

        // for(int i=startCNT;i<endCNT;i++){       
        //     initBuffer(dataBuffer);
        //     sprintf(dataBuff, "%d\r\n", myADCData[i]);
        //     setSDCWords(dataBuff, getBufferSize(dataBuff));
        //     // printf("=>%d\n", myADCData[i]);
        //     RecordNumbers++;
        // }

        // if(RecordNumbers%200==0){
        //     closeSDC();
        //     setSDCName(HMI_NAME, HMI_EXTENSION);
        //     setSDCContinuous();
        // }

        PORTA &= ~(1<<5);
        // printf("Num:%d\n", RecordNumbers);
        sei();
        // printf("=>%d\n", startCNT);


        if(OK_Flag && (RecordNumbers==dataNumbers)){
            closeSDC();
            

            printf("OK, Read SDC Data and send to matlab ...\n");
            setSDCName(HMI_NAME, HMI_EXTENSION);
            setSDCRead();
            
            printf("%lu\n", timer1Counter);
            

            for(uint16_t n=0;n<dataNumbers;n++){
                char readBuff[6];
                int calculateData = 0;
                initBuffer(readBuff);
                getSDCData(readBuff, 5);
                int digits = getBufferSizeUseCRLF(readBuff);
                // printf("digits:%d\n", digits);
                
                for(int k=0;k<digits;k++){
                    int dpow=1;
                    for(int j=0;j<(digits-k-1);j++) dpow *= 10;
                    calculateData += dpow*ascii2num(readBuff[k]);
                    // printf("%d\n", ascii2num(readBuff[k]));
                } 
                // printf("%d\n", calculateData);
                // DataAcquisitionSystem.data[n] = calculateData;
                printf("%d\n", calculateData);
            }
            closeSDC();

            while(true);
        }
        // if(LoopCounter % 10000 == 0){
        //     printf("Run ...\n");
        // }
        LoopCounter++;
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
            return 0;
        break;
    }
}

void initCircleBuffer(){
    initBuffer(CircleBuffer.ADC3Buffer);
    CircleBuffer.ADC3CounterStart = 0;
    CircleBuffer.ADC3CounterEnd = 0;
}

void setCircleBuffer(uint16_t data){
    if(!isCircleBufferFull()){
        CircleBuffer.ADC3Buffer[CircleBuffer.ADC3CounterEnd] = data;
        CircleBuffer.ADC3CounterEnd++;
    }
    else{
        CircleBuffer.ADC3CounterEnd = 0;
        CircleBuffer.ADC3Buffer[CircleBuffer.ADC3CounterEnd] = data;
        CircleBuffer.ADC3CounterEnd++;
    }
}
uint16_t getCircleBuffer(char pos){
    if(pos >= Buff_Max_Size){
        return CircleBuffer.ADC3Buffer[pos-Buff_Max_Size];
    }
    else{
        return CircleBuffer.ADC3Buffer[pos];
    }
}

bool isCircleBufferFull(){
    if((CircleBuffer.ADC3CounterStart) >= Buff_Max_Size){
        return true;
    }
    else{
        return false;
    }
}
