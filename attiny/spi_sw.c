/*
 * Copyright (c) 2017 Jens Hauke. All rights reserved.
 *
 * 2017-12-02 Jens Hauke <jens@4k2.de>
 */
/*
 * Software SPI implementation (spi.h) for ATTiny
 *
 * ATMEL ATTINY45
 *
 *              +-\/-+
 *        PB5  1|    |8  Vcc
 * SPI CS PB3  2|    |7  PB2 SPI CLK
 *        PB4  3|    |6  PB1 SPI MISO
 *        GND  4|    |5  PB0 SPI MOSI
 *              +----+
 *
 * Communicate with Winbond W25Q80BV SPI Flash.
 *
 *          SPI Flash
 *           +-\/-+
 * /CS      1|    |8  Vcc
 *  DO(IO1) 2|    |7  /HOLD(IO3)
 * /WP(IO2) 3|    |6  CLK
 * GND      4|    |5  DI(IO0)
 *           +----+
 *
 * ATtiny45          W25Q80BV
 * 2 PB3(CS)  ----   1 (CS)
 * 5 PB0(MOSI)----   5 (DI)
 * 6 PB1(MISO)----   2 (DO)
 * 7 PB2(SCK) ----   6 (CLK)
 *      Vcc   ----   8 (Vcc)  3.3V !
 *      GND   ----   4 (GND)
 *      Vcc -[2.7kΩ]-7 (HOLD)  Pull high!
 *             n.c.  3 (WP)
 */

/* Default pin configuration */
#ifndef SPI_CS_BIT
#define SPI_CS_BIT	3 /* PB3 */
#endif
#ifndef SPI_CLK_BIT
#define SPI_CLK_BIT	2 /* PB2 */
#endif
#ifndef SPI_MISO_BIT
#define SPI_MISO_BIT	1 /* PB1 */
#endif
#ifndef SPI_MOSI_BIT
#define SPI_MOSI_BIT	0  /* PB0 */
#endif

#include "../spi.h"


void spi_init(void) {
	DDRB =
		(DDRB & ~((1 << SPI_MISO_BIT))) | // 0: inputs
		((1 << SPI_CS_BIT) | (1 << SPI_MOSI_BIT) | (1 << SPI_CLK_BIT)); // 1: outputs
	PORTB =
		(PORTB & ~((1 << SPI_CLK_BIT) )) | // lo
		((1 << SPI_MOSI_BIT) | (1 << SPI_MISO_BIT) | (1 << SPI_CS_BIT)); // hi
}


uint8_t spi_transfer(uint8_t data) {
	uint8_t cnt;
	for (cnt = 0; cnt < 8; cnt++) {
		if (data & 128) {
			PORTB |= (1 << SPI_MOSI_BIT); // hi
		} else {
			PORTB &= ~(1 << SPI_MOSI_BIT); // lo
		}
		PORTB |= (1 << SPI_CLK_BIT); // hi

		data = (data << 1);
		if (PINB & (1 << SPI_MISO_BIT)) data++;

		PORTB &= ~(1 << SPI_CLK_BIT); // lo
	}
	PORTB |= (1 << SPI_MOSI_BIT); // hi
	return data;
}


void spi_cs_set(void) {
	PORTB &= ~(1 << SPI_CS_BIT);
}


void spi_cs_clr(void) {
	PORTB |= (1 << SPI_CS_BIT);
}
