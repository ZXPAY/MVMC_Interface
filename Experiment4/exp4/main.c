#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "ASA_Lib.h"
#include "lib/SDC00/ASA_lib_SDC00.h"
#include "lib/M128_Danny_ADC.h"

int main(){
    // Setup, Only run one time.
    ASA_M128_set();

    TCCR0 |= (1<<WGM00) | (1<<WGM01) | (1<<CS00) | (1<<CS02) | (1<<COM01);
    DDRB |= (1<<4);

    // Main code
    
    while(true){
        int mode;
        bool runFlag = true;
        printf("Please Enter Mode ~\n");
        scanf("%d", &mode);
        switch(mode){
            case 0:
                printf("Enter OCR data\n");
                uint16_t OCRdata;
                scanf("%d", &OCRdata);
                OCR0 = OCRdata;
            break;
            default:
                printf("Warning !!! No mode found !!!\n");
            break;
        }
        
    }

}

