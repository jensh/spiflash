/* (c) 2016-07-01 Jens Hauke <jens.hauke@4k2.de>              -*- linux-c -*- */
#ifndef _SPI_H_
#define _SPI_H_

#include <inttypes.h>

#ifdef __cplusplus
#define _spi_h_par {
extern "C" _spi_h_par
#endif

void spi_init(void);
uint8_t spi_transfer(uint8_t data);
void spi_cs_set(void);
void spi_cs_clr(void);

#ifdef __cplusplus
}
#endif
#endif /* _SPI_H_ */
