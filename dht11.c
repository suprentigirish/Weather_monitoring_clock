#include <LPC21xx.h>
#include "dht11.h"
#include "delay.h"

#define DHT11_PIN 4

void dht11_request() {
    IODIR0 |= (1 << DHT11_PIN);
    IOCLR0 = (1 << DHT11_PIN);
    delay_ms(20);
    IOSET0 = (1 << DHT11_PIN);
}

void dht11_response() {
    IODIR0 &= ~(1 << DHT11_PIN);
    while ((IOPIN0 & (1 << DHT11_PIN)));
    while (!(IOPIN0 & (1 << DHT11_PIN)));
    while ((IOPIN0 & (1 << DHT11_PIN)));
}

u8 dht11_read_byte() {
    u8 data = 0;
	  int i;
    for (i = 0; i < 8; i++) {
        while (!(IOPIN0 & (1 << DHT11_PIN)));
        delay_us(30);
        data <<= 1;
        if (IOPIN0 & (1 << DHT11_PIN)) data |= 1;
        while (IOPIN0 & (1 << DHT11_PIN));
    }
    return data;
}
