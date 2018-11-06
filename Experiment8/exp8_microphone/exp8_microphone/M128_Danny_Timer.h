#ifndef M128_DANNY_TIMER_H
#define M128_DANNY_TIMER_H


void setTimer0_CompareInterrupt(char OCR_Data, uint16_t prescalerLavel);
void openTimer0();
void closeTimer0();

void setTimer2_CompareInterrupt(char OCR_Data, uint16_t prescalerLavel);
void openTimer2();
void closeTimer2();

void setTimer1B_CompareInterrupt(uint16_t OCR_Data, uint16_t prescalerLavel);
void openTimer1B();
void closeTimer1B();

void openTimer3();
void closeTimer3();

uint8_t getTimer0PrescalerLavel(uint16_t prescalerLavel);
uint8_t getTimer123PrescalerLavel(uint16_t prescalerLavel);
void openWhichTimer(int whichTimer);
void closeWhichTimer(int whichTimer);


#endif



