/* (c) 2016-07-01 Jens Hauke <jens.hauke@4k2.de>
 *
 * Implementation of spi.h for Arduino
 */
#include "spi.h"

#include <SPI.h>


void spi_init(void) {
	SPI.begin();
	// Slow down ti 500kHz
	SPI.setClockDivider(SPI_CLOCK_DIV8);
}


uint8_t spi_transfer(uint8_t data) {
	return SPI.transfer(data);
}


void spi_cs_set(void) {
	digitalWrite(10, LOW);
}


void spi_cs_clr(void) {
	digitalWrite(10, HIGH);
}
