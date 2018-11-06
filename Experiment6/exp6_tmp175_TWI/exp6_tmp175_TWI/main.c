/*
 * exp6_tmp175_TWI.c
 *
 * Created: 2018/10/8 下午 02:14:35
 * Author : zxpay
 */ 
#define F_CPU 11059200
#include <avr/io.h>
#include <stdbool.h>
#include "M128_Danny_Timer.h"
#include "ASA_Lib.h"

#define TMP175_ADDRESS 0x48
#define START_ACK 0x08
#define M128_ACK_ADDRESS_RECIEVE 0x40
#define M128_ACK_ADDRESS_TRANSMIT 0x18
#define M128_ACK_DATA_TRANSMIT 0x28
#define TCA9534A_SLAVE_ADDRESS 0x38

void startTWI();
void stopTWI();
void setTWIAddress(uint8_t address, bool writeModeOrNot);
void setTWIData(uint8_t send_data, bool ackOrNot);
uint8_t getTWIData(bool ackOrNot);
void ASA_ID_init();
void ASA_ID_set(uint8_t data);
void setAlloutput();

bool flag = false;
ISR(INT4_vect){
	if(flag==false){
		setTimer0_CompareInterrupt(107, 1024);
		flag = true;
		EIMSK &= ~(1<<INT4);
		cli();
		openTimer0();
	}
	
	printf("========== Warning!!! ==========\n");
}

volatile int cnt = 0;
char LEDdata = 1;

ISR(TIMER0_COMP_vect){
	cnt++;				
	startTWI();
	setTWIAddress(TCA9534A_SLAVE_ADDRESS, true);
	setTWIData(0x01, false);  // command for config mode		
	setTWIData(LEDdata, false);
					
	stopTWI();
	
	if(cnt%3==0){
		LEDdata <<= 1;
		if(LEDdata==0){
			LEDdata = 1;
		}
	}
	
	if(cnt>=100){
		startTWI();
		setTWIAddress(TCA9534A_SLAVE_ADDRESS, true);
		setTWIData(0x01, false);  // command for config mode
		setTWIData(0, false);
		LEDdata = 1;
		cnt = 0;
		flag = false;
		TIMSK &= ~(1<<OCIE0);
		cli();
		EIMSK |= (1<<INT4);
		sei();
	}
}


int main(void){
	float data;
	ASA_M128_set();
	
	// Set SCL Frequency, in temp175 high speed mode => 400 khz
	// SCL_freq = CPU_Clock / (16 + 2*(TWBR) * 4^TWPS)     ,PS:TWPS:presaclar
	TWSR |= (1<<TWPS0);    // Prescaler 4
	//TWSR &= ~(1<<TWPS1);
	TWBR = 0x20;
	
	ASA_ID_init();
	ASA_ID_set(1);
	uint8_t dataH, dataL;
	setAlloutput();
	printf("========= Transmission =========\n");
	startTWI();
	printf("ACK ST:%x\n", (TWSR & 0xF8));
	
	setTWIAddress(TMP175_ADDRESS, true);
	printf("ACK Address:%x\n", (TWSR & 0xF8));
	
	setTWIData(0x00, false);
	printf("ACK SET:%x\n", (TWSR & 0xF8));

	
	printf("========= Stop =========\n");
	stopTWI();
	
	// setting INT4
/*
	DDRE &= ~(1<<PE4);
	EICRB |= (1<<ISC41);
	EIMSK |= (1<<INT4);
	sei();*/
	
	printf("========= End mode Setting =========\n");
	int configData;
	int mode;
	while (1) {
		printf("alert:%x\n", PINE);
		printf("please enter mode\n");
		scanf("%d", &mode);
		switch(mode){
			case 0:
				printf("Enter config data\n");
				scanf("%d", &configData);
				printf("Config data (hex) = %x\n", configData);
				startTWI();
				printf("ACK ST:%x\n", (TWSR & 0xF8));

				setTWIAddress(TMP175_ADDRESS, true);
				printf("ACK Address:%x\n", (TWSR & 0xF8));

				setTWIData(0x01, false);
				printf("ACK SET:%x\n", (TWSR & 0xF8));
				
				setTWIData(configData, false);
				printf("ACK SET:%x\n", (TWSR & 0xF8));

				stopTWI();
			break;;
			case 1:
				startTWI();
				printf("ACK ST:%x\n", (TWSR & 0xF8));

				setTWIAddress(TMP175_ADDRESS, true);
				printf("ACK Address:%x\n", (TWSR & 0xF8));

				setTWIData(0x00, false);
				printf("ACK SET:%x\n", (TWSR & 0xF8));

				stopTWI();
				
				printf("========= Recieve =========\n");
				startTWI();
				setTWIAddress(TMP175_ADDRESS, false);
				printf("ACK Address:%x\n", (TWSR & 0xF8));

				dataH = getTWIData(true);
				printf("ACK RD:%x\n", (TWSR & 0xF8));
				printf("DataH:%x\n", dataH);
		
				dataL = getTWIData(false);
				printf("ACK RD:%x\n", (TWSR & 0xF8));
				printf("DataL:%x\n", dataL);
		
				TWCR &= (1<<TWEN);
				stopTWI();
				printf("ACK:%x\n", TWSR);
		
				printf("final data is %d, %d\n", dataH, dataL);
				data = (((dataH<<8)+dataL) >> 4)*0.0625;
				printf("Temperature is %.2f\n", data);
				printf("Start Main Code ~~~\n");

			break;
			case 2:
				// set alert Temp_HIGH
				startTWI();
				printf("ACK ST:%x\n", (TWSR & 0xF8));

				setTWIAddress(TMP175_ADDRESS, true);
				printf("ACK Address:%x\n", (TWSR & 0xF8));

				setTWIData(0x03, false);
				printf("ACK SET:%x\n", (TWSR & 0xF8));
				
				setTWIData(100, true);
				printf("ACK SET:%x\n", (TWSR & 0xF8));

				stopTWI();
			break;
			case 3:
				// set alert Temp_LOW
				startTWI();
				printf("ACK ST:%x\n", (TWSR & 0xF8));

				setTWIAddress(TMP175_ADDRESS, true);
				printf("ACK Address:%x\n", (TWSR & 0xF8));

				setTWIData(0x02, false);
				printf("ACK SET:%x\n", (TWSR & 0xF8));
				
				setTWIData(0, true);
				printf("ACK SET:%x\n", (TWSR & 0xF8));

				stopTWI();
				break;

		}

    }
}

void startTWI(){
	// Transmit start condition
	
	TWCR |= (1<<TWSTA);  // TWI Start
	TWCR |= (1<<TWINT);  // Clear TWING flag
	TWCR |= (1<<TWEN);   // TWI Enable
	// Wait for TWINT flag set. This indicates that the START condition has been transmitted.
	while (!(TWCR & (1<<TWINT)));
	// Check for the acknowledgement
	if((TWSR & 0xF8)!= START_ACK){
		printf("Start fail !!!\n");
	}
}

void setTWIAddress(uint8_t address, bool writeModeOrNot){
	if(writeModeOrNot){
		TWDR = ((address<<1) & (~1));
	}
	else{
		TWDR = ((address<<1) | (1));
	}
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)))
	// Check for the acknowledge
	_delay_ms(1);
	if(writeModeOrNot){
		if((TWSR & 0xF8) != M128_ACK_ADDRESS_TRANSMIT){
			printf("Transmission mode, slave not response !!!\n");
			printf("Status is %x\n", (TWSR & 0xF8));
		}
	}
	else{
		if((TWSR & 0xF8) != M128_ACK_ADDRESS_RECIEVE){
			printf("Read mode, slave not response !!!\n");
			printf("Status is %x\n", (TWSR & 0xF8));
		}
	}
}

void setTWIData(uint8_t send_data, bool ackOrNot){
	TWDR = send_data;
	TWCR = (1<<TWINT)|(1<<TWEN)|(ackOrNot<<TWEA);
	
	while (!(TWCR & (1<<TWINT)));
	_delay_ms(1);
	if((TWSR & 0xF8) != M128_ACK_DATA_TRANSMIT){
		printf("Set data ACK not valid !!!\n");
		printf("Status is %x\n", (TWSR & 0xF8));
	}
}

uint8_t getTWIData(bool ackOrNot){
	if(ackOrNot){
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	}
	else{
		TWCR = (1<<TWINT)|(1<<TWEN);
	}
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

void stopTWI(){
	// Transmit stop condition
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void ASA_ID_init() {
	DDRB |= (1<<PB7)|(1<<PB6)|(1<<PB5);
};

void ASA_ID_set(uint8_t data) {
	if (data>7) {
		return;
	}
	PORTB &= ~(7<<PB5);
	PORTB |= (data<<PB5);

};

void setAlloutput(){	
	printf("Init IO\n");
				
	startTWI();					
	setTWIAddress(TCA9534A_SLAVE_ADDRESS, true);					
	setTWIData(0x03, false);  // command for config mode			
	setTWIData(0, false);
					
	stopTWI();
					
	// write all output 0 V
	startTWI();
	setTWIAddress(TCA9534A_SLAVE_ADDRESS, true);				
	setTWIData(0x01, false);  // command for config mode				
	setTWIData(0, false);			
	stopTWI();

}

