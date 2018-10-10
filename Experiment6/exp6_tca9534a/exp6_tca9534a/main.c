/*
 * exp6_tca9534a.c
 *
 * Created: 2018/10/10 上午 11:57:02
 * Author : zxpay
 */ 
#define F_CPU 11059200
#include <avr/io.h>
#include <stdbool.h>
#include "ASA_Lib.h"

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

/*
	first step : setting input or output
	

*/
int main(void){
	ASA_M128_set();
	
	// Set SCL Frequency, in temp175 high speed mode => 400 khz
	// SCL_freq = CPU_Clock / (16 + 2*(TWBR) * 4^TWPS)     ,PS:TWPS:presaclar
	TWSR |= (1<<TWPS0);    // Prescaler 4
	//TWSR &= ~(1<<TWPS1);
	TWBR = 0x20;
	ASA_ID_init();
	ASA_ID_set(1);
	int commandData, writeData;
    while (1) {
		int mode;
		printf("Please enter mode \n");
		scanf("%d", &mode);
		switch(mode){
			case 0:
				printf("Please enter command data\n");
				scanf("%d", &commandData);
				printf("Please enter data\n");
				scanf("%d", &writeData);
				
				printf("HEX CMD : %x\n", commandData);
				printf("HEX DATA : %x\n", writeData);
				
				printf("===== START =====\n");
				
				startTWI();
				printf("START ACK : %x\n", (TWSR & 0xf8));
				
				setTWIAddress(TCA9534A_SLAVE_ADDRESS, true);
				printf("Address ACK : %x\n", (TWSR & 0xf8));
				
				setTWIData(commandData, false);
				printf("set ACK : %x\n", (TWSR & 0xf8));
				
				setTWIData(writeData, false);
				printf("set ACK : %x\n", (TWSR & 0xf8));
				
				stopTWI();
				
				printf("===== END =====\n");
			break;
			case 1:
				// configure the data input or output
				printf("===== START =====\n");
				printf("Please enter I/O, 0->input, 1->output\n");
				scanf("%d", &writeData);
				writeData = ~(writeData);
				printf("HEX DATA : %x\n", writeData);
				
				printf("===== START =====\n");
				
				startTWI();
				printf("START ACK : %x\n", (TWSR & 0xf8));
				
				setTWIAddress(TCA9534A_SLAVE_ADDRESS, true);
				printf("Address ACK : %x\n", (TWSR & 0xf8));
				
				setTWIData(0x03, false);  // command for config mode
				printf("set ACK : %x\n", (TWSR & 0xf8));
				
				setTWIData(writeData, false);
				printf("set ACK : %x\n", (TWSR & 0xf8));
				
				stopTWI();
				
				// write all output 0 V
				startTWI();
				printf("START ACK : %x\n", (TWSR & 0xf8));
				setTWIAddress(TCA9534A_SLAVE_ADDRESS, true);
				printf("Address ACK : %x\n", (TWSR & 0xf8));
				
				setTWIData(0x01, false);  // command for config mode
				printf("set ACK : %x\n", (TWSR & 0xf8));
				
				setTWIData(0, false);
				printf("set ACK : %x\n", (TWSR & 0xf8));
				
				stopTWI();
				
				printf("===== END =====\n");
			break;
			case 2:
				printf("===== START =====\n");
				printf("Please enter output data\n");
				scanf("%d", &writeData);
				printf("HEX DATA : %x\n", writeData);
				
				startTWI();
				printf("START ACK : %x\n", (TWSR & 0xf8));
				
				setTWIAddress(TCA9534A_SLAVE_ADDRESS, true);
				printf("Address ACK : %x\n", (TWSR & 0xf8));
				
				setTWIData(0x01, false);  // command for config mode
				printf("set ACK : %x\n", (TWSR & 0xf8));
				
				setTWIData(writeData, false);
				printf("set ACK : %x\n", (TWSR & 0xf8));
				
				stopTWI();
				
				printf("===== END =====\n");
			break;
			case 3:
				// Read Input data
				printf("===== START =====\n");
				startTWI(START_ACK);
				printf("START ACK : %x\n", (TWSR & 0xf8));

				setTWIAddress(TCA9534A_SLAVE_ADDRESS, true);
				printf("Address Write ACK : %x\n", (TWSR & 0xf8));

				setTWIData(0x00, false);  // command for config mode
				printf("set ACK : %x\n", (TWSR & 0xf8));

				TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
				while (!(TWCR & (1<<TWINT)));
				printf("Repeat Start : %x\n", (TWSR & 0xf8));
				
				setTWIAddress(TCA9534A_SLAVE_ADDRESS, false);
				printf("Address Read ACK : %x\n", (TWSR & 0xf8));
				
				//data = TWDR;
				printf("data is %x\n", getTWIData(false));
				
				TWCR &= ~(1<<TWEN);
				stopTWI();

				printf("===== END =====\n");
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
	printf("TWDR:%x\n", TWDR);
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

