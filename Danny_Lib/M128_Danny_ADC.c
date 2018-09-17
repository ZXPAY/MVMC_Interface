#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "M128_Danny_ADC.h"

void setEnableADC(bool isEnable){
    if(isEnable) ADCSRA |= (1 << ADEN); // Enable ADC
    else ADCSRA &= ~(1 << ADEN);
}

void setADCDivision(uint8_t Factor){
    // Clear division factor bit
    ADCSRA &= ~(0b00000111);
    switch(Factor){
        case 2:
            ADCSRA |= (1 << ADPS0);
        break;
        case 4:
            ADCSRA |= (1 << ADPS1);
        break;
        case 8:
            ADCSRA |= (1 << ADPS0) | (1 << ADPS1);
        break;
        case 16:
            ADCSRA |= (1 << ADPS2);
        break;
        case 32:
            ADCSRA |= (1 << ADPS0) | (1 << ADPS2);
        break;
        case 64:
            ADCSRA |= (1 << ADPS1) | (1 << ADPS2);
        break;
        case 128:
            ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
        break;
        default:
            printf("Error Factor found ... => Set division factor 8\n");
            ADCSRA |= (1 << ADPS0) | (1 << ADPS1);
        break;
    }
}

void setFreeRunning(bool isOpen){
    if(isOpen) ADCSRA |= (1 << ADFR);
    else ADCSRA &= ~(1 << ADFR);
}

void setMuxADC(uint8_t whichADC){
    // Clear MUX bits
    ADMUX &= ~(0b11111);
    if(whichADC >= 0 && whichADC <= 7) ADMUX |= whichADC;
    else printf("Invalid whichADC number ...%d\n", whichADC);
}

// 0 -> AREF, Internal Vref turned off
// 1 -> AVCC with external capacitor at AREF pin
// 2 -> Reserved
// 3 -> Internal 2.56V Voltage Reference with external capacitor at AREF pin
void setVoltageRefADC(uint8_t modeNumber){
    ADMUX &= ~(3 << 6);
    ADMUX |= (modeNumber << 6);
}

void setInterruptADCEnable(bool isEnable){
    if(isEnable){
        ADCSRA |= (1 << ADIE) | (1 << ADIF); 
        sei();
        setConversion();
    }
    else{
        ADCSRA &= ~(1 << ADIE);
        cli();
    } 
}

void setupADC2(){
    setADCDivision(2);
    setVoltageRefADC(3);
    setMuxADC(2);
    setEnableADC(true);
}

void setupADC3(){
    setADCDivision(2);
    setVoltageRefADC(3);
    setMuxADC(3);
    setEnableADC(true);
}

void setConversion(){
    ADCSRA |= (1 << ADSC); // Start A2D Conversions
}
bool isConversionComplete(){
    while((ADCSRA & (1<<ADSC)));
    while(!(ADCSRA & (1<<ADIF)));
    ADCSRA |= (1<<ADIF);
    return true;
}

void setIO(volatile uint8_t *DDR, uint8_t pinNumber, bool isOutput){
    if(isOutput) *DDR |= (1 << pinNumber);
    else *DDR &= ~(1 << pinNumber); 
}

int analogReadADC2(){
    setupADC2();
    setConversion();
    if(isConversionComplete){
        return (ADCL + (ADCH << 8));
    }
}

int analogReadADC3(){
    setupADC3();
    setConversion();
    if(isConversionComplete){
        return (ADCL + (ADCH << 8));
    }
}





