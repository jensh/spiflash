SPI flash
=========

Read/Write/Erase from SPI flash.

Currently supported SPI flash devices:
 * W25Q80BV winbond (without dual/quad speeds) 8MBit (1MByte)


Arduino
=======
 * tested with Arduino Pro Mini.
 * tested with ATTiny45.



Files
=====

```
spiflash.h              spiflash interface.
spiflash.c              spiflash implementation.
README.md               this file
examples/Dump/Dump.ino  Example sketch for the Arduino IDE
spi.h                   spi prototypes you have to implement for your MCU
utility/spi.cpp         spi implementation for Arduino (MOSI,MISO,SCK,CS(=10))
attiny/spi_sw.c		sw-spi implementation for attiny (MOSI(=PB0),MISO(=PB1),SCK(=PB2),CS(=PB3))
```
