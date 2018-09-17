#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "ASA_Lib.h"
#include "ASA_Lib_DAC00.h"
#include <math.h>
#include <inttypes.h>


#define SDC_ID 4

#define DAC_ID 1
#define DAC_LSBYTE 200
#define DAC_SET_CHANNEL_MASK 0x30
#define DAC_SET_CHANNEL_SHIFT 4
#define DAC_SET_ASYNCHRONOUS_MASK 0x80
#define DAC_SET_ASYNCHRONOUS_SHIFT 7
#define setSynchronous 0
#define setAsynchronous 1

#define CASE0_Name "CASE0"
#define CASE1_Name "CASE1"
#define CASE2_Name "CASE2"
#define CASE3_Name "CASE3"

#define MAX_DAC_VALUE 4095
#define MIN_DAC_VALUE 0
#define pi 3.141592

volatile char timer0Counter = 0;
volatile char timer1Counter = 0;
char sendBuffer[128];
void createFile(char fileName[]);
void createAllCaseFiles();
void initSendBuffer();
int getSendBufferSize();
void setSDCName(char name[], char secondName[]);
void setSDCWords();
void setSDCContinuous();
void closeSDC();

void setTimer0_CompareInterrupt(char OCR_Data);
void openTimer0();
void closeTimer0();
void openTimer1();
void closeTimer1();
void testSDC();

uint16_t sinData[100];
uint16_t cosData[100];
int sinNumberCounter = 0;
volatile bool timer1Flag = false;

ISR(TIMER0_COMP_vect){
    // int data = 0;
    // char check;
    // data = (sin(timer0Counter*2*pi/100)+1)*(2047);
    // check = ASA_DAC00_put(DAC_ID, 0, 2, &data);
    // if(check != 0){
    //     printf("DAC put Error ... %d\n", check);
    // }
    // if(timer0Counter>=100){
    //     timer0Counter = 0;
    // }
    timer0Counter++; 
}

ISR(TIMER1_COMPB_vect){ 
    // PORTA = 0x00;
    PORTA |= (1<<7);
    char check;
    check = ASA_DAC00_put(DAC_ID, 0, 2, &sinData[timer1Counter]);
    if(check != 0){
        printf("DAC put Error ... %d\n", check);
    }
    else{

    }
    if(timer1Counter>=99){
        timer1Counter = 0;
        sinNumberCounter++;
    }
    timer1Counter++;
    PORTA &= ~(1<<7);
}

void setTimer0_CompareInterrupt(char OCR_Data){
    TCCR0 |= 1<<WGM01;
    TCCR0 |= (1<<CS00)|(1<<CS01);
    TCNT0 = 0;
    OCR0 = OCR_Data;
    TIMSK = 1<<OCIE0;
}

void setTimer1B_CompareInterrupt(uint16_t OCR_Data){ 
    TCCR1B |= 1 << WGM12; 
    TCCR1B |= (1<<CS10); // pre scalar 0
    TCNT1 = 0; 
    OCR1A = OCR_Data; 
    TIMSK |= 1<<OCIE1B; 
} 
 

void openTimer0(){
    TIMSK |= (1<<OCIE0);
    sei();
}
void closeTimer0(){
    TIMSK &= ~(1<<OCIE0);
    cli();
}
void openTimer1(){
    timer1Flag = true;
    TIMSK |= (1<<OCIE1B);
    sei();
}
void closeTimer1(){
    timer1Flag = false;
    TIMSK &= ~(1<<OCIE1B);
    cli();
}

int main(){
    DDRA |= 1<<7;
    ASA_M128_set();
    for(int i=0;i<100;i++){
        sinData[i] = (sin(i*2*pi/100)+1)*(2047);
        cosData[i] = (cos(i*2*pi/100)+1)*(2047);
    }

    printf("Run main code ~\n");

    // Run main code 
    while(true){
        uint32_t LoopCounter = 0;
        uint16_t OCRData;
        int counter = 0;
        int mode;
        char check;
        int data = 0;
        unsigned long freq;
        printf("Please enter mode ~ \n");
        scanf("%d", &mode);
        switch(mode){
            case 0:
                cli();
                printf("Run Mode 0 ~ single channel\n");
                setSDCName(CASE0_Name, "bin");
                setSDCContinuous();
                while(counter<200){
                    for(int i=0;i<100;i++){
                        ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_CHANNEL_MASK, DAC_SET_CHANNEL_SHIFT, 0);  // set channel 0
                        ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_ASYNCHRONOUS_MASK, DAC_SET_ASYNCHRONOUS_SHIFT, setAsynchronous);  // asynchronous
                        check = ASA_DAC00_put(DAC_ID, 0, 2, &sinData[i]);
                        if(check != 0){
                            printf("DAC put Error ... %d\n", check);
                        }
                        else{
                            // sprintf(sendBuffer, "counter:%d,value:%d\r\n", i, data);
                            setSDCWords();
                        }
                        //printf("%d\n", data);
                     }
                     counter++;
                }
                closeSDC();
            break;
            case 1:
                cli();
                printf("Run Mode 0 ~ Muti channel\n");
                setSDCName(CASE1_Name, "bin");
                setSDCContinuous();
                while(counter<200){
                    for(int i=0;i<100;i++){
                        ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_CHANNEL_MASK, DAC_SET_CHANNEL_SHIFT, 0);  // set channel 0
                        ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_ASYNCHRONOUS_MASK, DAC_SET_ASYNCHRONOUS_SHIFT, setAsynchronous);  // asynchronous
                        check = ASA_DAC00_put(DAC_ID, 0, 2, &sinData[i]);
                        if(check != 0){
                            printf("DAC put Error ... %d\n", check);
                        }
                        else{
                            sprintf(sendBuffer, "counter:%d,sin:%d,", i, data);
                            setSDCWords();
                        }
                        ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_CHANNEL_MASK, DAC_SET_CHANNEL_SHIFT, 1);  // set channel 1
                        ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_ASYNCHRONOUS_MASK, DAC_SET_ASYNCHRONOUS_SHIFT, setAsynchronous);  // asynchronous
                        check = ASA_DAC00_put(DAC_ID, 0, 2, &cosData[i]);
                        if(check != 0){
                            printf("DAC put Error ... %d\n", check);
                        }
                        else{
                            // sprintf(sendBuffer, "cos:%d,\r\n", data);
                            setSDCWords(); 
                        }
                        //printf("%d\n", data);
                     }
                     counter++;
                }
                closeSDC();
            break;
            case 2:
                sinNumberCounter = 0;
                setSDCName(CASE2_Name, "bin");
                setSDCContinuous();
                ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_CHANNEL_MASK, DAC_SET_CHANNEL_SHIFT, 0);  // set channel 0
                ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_ASYNCHRONOUS_MASK, DAC_SET_ASYNCHRONOUS_SHIFT, setAsynchronous);  // asynchronous 
                printf("Please enter the Freq (Hz) ch1\n");
                scanf("%lu", &freq);
                OCRData = 11059200/(2*(freq*100/2)) - 1;
                if(OCRData < 0) OCRData = 0;
                if(OCRData > 65535) OCRData = 65535;
                setTimer1B_CompareInterrupt(OCRData);
                printf("OCRData -> %d\n", OCRData);
                openTimer1();
                // DDRA = 0xff;
                while(sinNumberCounter < 1000){
                    // PORTA = 0xff;
                    // if(counter%10 == 0){
                        cli();
                        setSDCWords();
                        sei();
                    // }
                    // counter++;
                }
                closeTimer1();
                closeSDC();
            break;
            case 3:
                closeTimer1();
                closeSDC();
            break;
            case 4:
                sprintf(sendBuffer, "Hello\n");
                printf("---> %d\n", getSendBufferSize());
            break;
            case 5:
                testSDC();
            break;
            case 6:
                check = ASA_SDC00_put(SDC_ID, 64, 8, "Hello111");
                if(check != 0) {printf("1 fail ...%d\n", check);}

                check = ASA_SDC00_put(SDC_ID, 72, 3, "txt");
                if(check != 0) {printf("2 fail ...%d\n", check);}

                check = ASA_SDC00_set(SDC_ID, 200, 0x07, 0, 0x05); // open 覆蓋
                if(check != 0) {printf("Open fail ...%d\n", check);}

                check = ASA_SDC00_set(SDC_ID, 200, 0x01, 0, 0x00); // close
                if(check != 0) {printf("Close fail ...%d\n", check);}

                setSDCName("Hello111", "txt");
                setSDCWords("Hello World~\r\n");
                setSDCWords("Hello World~\r\n");
                setSDCWords("Hello World~\r\n");
                setSDCWords("Hello World~\r\n");
            break;
            case 7:
                createAllCaseFiles();
            break;
            case 8:
                setSDCName(CASE3_Name, "bin");
                setSDCContinuous();
                setSDCWords("Hello\r\nHHH");
                closeSDC();
            break;
            case 9:
                sinNumberCounter = 0;
                setSDCName(CASE2_Name, "bin");
                setSDCContinuous();

                closeSDC();
                ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_CHANNEL_MASK, DAC_SET_CHANNEL_SHIFT, 0);  // set channel 0
                ASA_DAC00_set(DAC_ID, DAC_LSBYTE, DAC_SET_ASYNCHRONOUS_MASK, DAC_SET_ASYNCHRONOUS_SHIFT, setAsynchronous);  // asynchronous 
                printf("Please enter the Freq (Hz) ch1\n");
                scanf("%lu", &freq);
                for(int k=0;k<100;k++){
                    sinData[k] = ((5*sin(k*2*pi/100)) + (2.5*sin(10*k*2*pi/100))+7.5)*273;
                    printf("data is : %d\n", sinData[k]);
                    ASA_SDC00_put(SDC_ID, 0, 2, sinData[k]);
                }
                OCRData = 11059200/(2*(freq*100/2)) - 1;
                if(OCRData < 0) OCRData = 0;
                if(OCRData > 65535) OCRData = 65535;
                setTimer1B_CompareInterrupt(OCRData);
                printf("OCRData -> %d\n", OCRData);
                openTimer1();
                // DDRA = 0xff;
                while(sinNumberCounter < 1000){
                    // PORTA = 0xff;
                    // if(counter%10 == 0){
                        // cli();
                        // setSDCWords();
                        // sei();
                    // }
                    // counter++;
                }
                closeTimer1();
                closeSDC();
            break;
        }

    }
    
    return 0;
}

void createFile(char fileName[]){
    char check;
    check = ASA_SDC00_put(SDC_ID, 64, 8, fileName);
    if(check != 0) {printf("1 fail ...%d\n", check);}

    check = ASA_SDC00_put(SDC_ID, 72, 3, "bin");
    if(check != 0) {printf("2 fail ...%d\n", check);}

    check = ASA_SDC00_set(SDC_ID, 200, 0x07, 0, 0x05); 
    if(check != 0) {printf("Open fail ...%d\n", check);}

    check = ASA_SDC00_set(SDC_ID, 200, 0x01, 0, 0x00); // close
    if(check != 0) {printf("Close fail ...%d\n", check);}
}

void createAllCaseFiles(){
    createFile(CASE0_Name);
    createFile(CASE1_Name);
    createFile(CASE2_Name);
    createFile(CASE3_Name);
}


void testSDC(){
    char check;
    check = ASA_SDC00_put(SDC_ID, 64, 8, "Hello");
    if(check != 0) {printf("1 fail ...%d\n", check);}

    check = ASA_SDC00_put(SDC_ID, 72, 3, "txt");
    if(check != 0) {printf("2 fail ...%d\n", check);}

    check = ASA_SDC00_set(SDC_ID, 200, 0x07, 0, 0x05); // open 覆蓋
    if(check != 0) {printf("Open fail ...%d\n", check);}

    char sendTestString[17];
    sprintf(sendTestString, "Hello World !!!\r\n");
    check = ASA_SDC00_put(SDC_ID, 0, 17, sendTestString);
    if(check != 0) {printf("Write fail ...%d\n", check);}

    check = ASA_SDC00_set(SDC_ID, 200, 0x01, 0, 0x00); // close
    if(check != 0) {printf("Close fail ...%d\n", check);}
}


void initSendBuffer(){
    memset(sendBuffer, 0, sizeof(sendBuffer));
}

// 續寫模式
void setSDCContinuous(){
    char check;
    check = ASA_SDC00_set(SDC_ID, 200, 0x07, 0, 0x07); // open 續寫
    if(check != 0) {printf("Open fail ...%d\n", check);}
}

// 蓋寫模式
void setSDCReplace(){
    char check;
    check = ASA_SDC00_set(SDC_ID, 200, 0x07, 0, 0x05); // open 覆蓋
    if(check != 0) {printf("Open fail ...%d\n", check);}
}

void setSDCWords(){
    char check;
    // sprintf(sendBuffer, dataString);
    check = ASA_SDC00_put(SDC_ID, 0, 2, sinData[timer1Counter]);
    if(check != 0) {printf("Write fail ...%d\n", check);}
    
    // initSendBuffer();
}

void closeSDC(){
    char check;
    check = ASA_SDC00_set(SDC_ID, 200, 0x01, 0, 0x00); // close
    if(check != 0) {printf("Close fail ...%d\n", check);}
}

void setSDCName(char name[], char secondName[]){
    char check;
    check = ASA_SDC00_put(SDC_ID, 64, 8, name);
    if(check != 0) {printf("SD name fail ...%d\n", check);}

    check = ASA_SDC00_put(SDC_ID, 72, 3, secondName);
    if(check != 0) {printf("SD second name fail ...%d\n", check);}
}

int getSendBufferSize(){
    int counter = 0;
    for(int i=0;i<sizeof(sendBuffer);i++){
        if(sendBuffer[i] != '\0'){
            counter++;
        }
        else{
            return counter;
        }
    }
}

