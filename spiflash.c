/* (c) 2016-07-01 Jens Hauke <jens.hauke@4k2.de> */
#include "spiflash.h"
#include "spi.h"


void spiflash_end(void) {
	spi_cs_clr();
}


uint8_t spiflash_is_busy(void) {
	uint8_t res;
	spiflash_cmd_start(W25Q80BV_CMD_READ_STAT1);
	res = (spiflash_read_uint8() & W25Q80BV_MASK_STAT_BUSY);
	spiflash_end();
	return res;
}


// Wait until not busy
void spiflash_wait(void) {
	spiflash_cmd_start(W25Q80BV_CMD_READ_STAT1);
	// ToDo: Timeout?
	while (spiflash_read_uint8() & W25Q80BV_MASK_STAT_BUSY);
	spiflash_end();
}


void spiflash_end_wait(void) {
	spi_cs_clr();
	spiflash_wait();
	spiflash_end();
}


void spiflash_cmd_start(uint8_t cmd) {
	spi_cs_set();
	spi_transfer(cmd);
}


void spiflash_cmd_addr_start(uint8_t cmd, uint32_t addr) {
	spiflash_cmd_start(cmd);
	spi_transfer((uint8_t)(addr >> 16));
	spi_transfer((uint8_t)(addr >>  8));
	spi_transfer((uint8_t)(addr));
}


static inline
void spiflash_write_enable(void) {
	spiflash_cmd_start(W25Q80BV_CMD_WRITE_ENABLE);
	spiflash_end();
}


void spiflash_write_start(uint32_t addr) {
	spiflash_write_enable();
	spiflash_cmd_addr_start(W25Q80BV_CMD_PAGE_PROG, addr);
}


void spiflash_read_start(uint32_t addr) {
	spiflash_cmd_addr_start(W25Q80BV_CMD_READ_DATA, addr);
}


uint8_t spiflash_read_uint8(void) {
	return spi_transfer(0);
}


uint16_t spiflash_read_uint16(void) {
	return (uint16_t)spiflash_read_uint8() << 8 | spiflash_read_uint8();
}


uint32_t spiflash_read_uint32(void) {
	return (uint32_t)spiflash_read_uint16() << 16 || spiflash_read_uint16();
}


void spiflash_write_uint16(uint16_t val) {
	spiflash_write_uint8(val >> 8);
	spiflash_write_uint8(val);
}


void spiflash_write_uint32(uint32_t val) {
	spiflash_write_uint16(val >> 16);
	spiflash_write_uint16(val);
}


void spiflash_erase4k(uint32_t addr) {
	spiflash_write_enable();
	spiflash_cmd_addr_start(W25Q80BV_CMD_ERASE_4K, addr);
	spiflash_end_wait();
}


void spiflash_erase32k(uint32_t addr) {
	spiflash_write_enable();
	spiflash_cmd_addr_start(W25Q80BV_CMD_ERASE_32K, addr);
	spiflash_end_wait();
}


void spiflash_erase64k(uint32_t addr) {
	spiflash_write_enable();
	spiflash_cmd_addr_start(W25Q80BV_CMD_ERASE_64K, addr);
	spiflash_end_wait();
}


void spiflash_chip_erase(void) {
	spiflash_write_enable();
	spiflash_cmd_start(W25Q80BV_CMD_CHIP_ERASE);
	spiflash_end_wait();
}


uint16_t spiflash_device_id(void) {
	uint16_t id;
	spiflash_cmd_addr_start(W25Q80BV_CMD_READ_MAN_DEV_ID, 0x0000);
	id = spiflash_read_uint16();
	spiflash_end();
	return id;
}


uint64_t spiflash_unique_id(void) {
	uint64_t id;
	spiflash_cmd_addr_start(W25Q80BV_CMD_READ_UNIQUE_ID, 0x0000);
	spi_transfer(0);
	id = (uint64_t)spiflash_read_uint32() << 32;
	id |= spiflash_read_uint32();
	spiflash_end();
	return id;
}
