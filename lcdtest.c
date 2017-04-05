#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#define DDR_USI             DDRB
#define PORT_USI            PORTB
#define PIN_USI             PINB
#define SDA_BIT				(1<<0)
#define	SCL_BIT				(1<<2)
//#define PORT_USI_SDA        PB0
//#define PORT_USI_SCL        PB2
//#define PIN_USI_SDA         PINB0
//#define PIN_USI_SCL         PINB2

static inline void scl(uint8_t high) {
	if(high) {
		DDR_USI&=~SCL_BIT;
	} else {
		DDR_USI|=SCL_BIT;
	}
}

static inline void sda(uint8_t high) {
	if(high) {
		DDR_USI&=~SDA_BIT;
	} else {
		DDR_USI|=SDA_BIT;
	}
}

void sendmsg(uint8_t *msg, uint8_t len) {
	// start condition
	sda(0);
	_delay_us(500);
	scl(0);
	_delay_us(500);
	while(len--) {
		uint8_t byte=*msg;
		uint8_t i;
		for(i=0;i<8;i++) {
			sda(byte&0x80);
			_delay_us(200);
			scl(1);
			_delay_us(400);
			scl(0);
			_delay_us(200);
			byte<<=1;
		}
		sda(1);
		_delay_us(200);
		scl(1); // here we should have an ack
		_delay_us(400);
		scl(0);
		_delay_us(200);
		msg++;
	}
	_delay_us(200);
	scl(1);
	_delay_us(500);
	sda(1);
	
}


int main(void) {
	PORT_USI&=~(SDA_BIT|SCL_BIT);
	
	
	
	uint8_t msg[]={0b01110000, 0b01000000};

	sendmsg(msg, 2);

}