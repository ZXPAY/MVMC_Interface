#ifndef M128_DANNY_ADC_H
#define M128_DANNY_ADC_H

void setEnableADC(bool isEnable);
void setADCDivision(uint8_t Factor);
void setFreeRunning(bool isOpen);
void setMuxADC(uint8_t whichADC);
void setVoltageRefADC(uint8_t modeNumber);
void setInterruptADCEnable(bool isEnable);
void setIO(volatile uint8_t *DDR, uint8_t pinNumber, bool isOutput);
void setupADC2();
void setupADC3();
int analogReadADC2();
int analogReadADC3();


#endif
