/* (c) 2016-07-01 Jens Hauke <jens.hauke@4k2.de>
 *
 * Communicate with Winbond W25Q80BV serial SPI Flash.
 *
 *     SPI Flash
 *           +-\/-+
 * /CS      1|    |8  Vcc
 *  DO(IO1) 2|    |7  /HOLD(IO3)
 * /WP(IO2) 3|    |6  CLK
 * GND      4|    |5  DI(IO0)
 *           +----+
 *
 * Connections:
 *
 * Arduino
 * Pro Mini          W25Q80BV
 * 10 (CS)    ----   1 (CS)
 * 11 (MOSI)  ----   5 (DI)
 * 12 (MISO)  ----   2 (DO)
 * 13 (SCK)   ----   6 (CLK)
 *      Vcc   ----   8 (Vcc)  3.3V !
 *      GND   ----   4 (GND)
 *      Vcc -[2.7kΩ]-7 (HOLD)  Pull high!
 *             n.c.  3 (WP)
 */

#include <spiflash.h>

#define PAGE_SIZE SPIFLASH_PAGE_SIZE

uint32_t addr = 0x000000;


void setup() {
	Serial.begin(57600);
	Serial.println(F("Hello!"));

	spiflash_init();

	uint16_t id;
	Serial.print(F("DevId:"));
	id = spiflash_device_id();
	Serial_hex(id);
	Serial.println();

	uint64_t unique;
	Serial.print(F("Unique:"));
	unique = spiflash_unique_id();
	Serial_hex(unique);
	Serial.println();

	Serial.println(F("r: read page"));
	Serial.println(F("+: addr += PAGE_SIZE, read page"));
	Serial.println(F("-: addr -= PAGE_SIZE, read page"));
	Serial.println(F("w: write 0x22 and 0x11 in page"));
	Serial.println(F("e: erase 4k sector"));
	Serial.println(F("d: dump all"));
}


char hex_nibble(uint8_t val) {
	return val < 10 ? val + '0' : val + 'A' - 10;
}


void Serial_hex(uint8_t val) {
	Serial.print(hex_nibble(val / 16));
	Serial.print(hex_nibble(val & 0xf));
}


void Serial_hex(uint16_t val) {
	Serial_hex((uint8_t)(val >> 8));
	Serial_hex((uint8_t)val);
}


void Serial_hex(uint32_t val) {
	Serial_hex((uint8_t)(val >> 24));
	Serial_hex((uint8_t)(val >> 16));
	Serial_hex((uint8_t)(val >> 8));
	Serial_hex((uint8_t)val);
}


void Serial_hex(uint64_t val) {
	Serial_hex((uint32_t)(val >> 32));
	Serial_hex((uint32_t)val);
}


void read_page(uint32_t addr) {
	spiflash_read_start(addr);

	for (uint32_t i = 0; i < PAGE_SIZE; i++) {
		if (i % 16 == 0) {
			Serial_hex(addr + i);
		}
		if (i % 4 == 0) {
			Serial.print(' ');
		}
		uint8_t byte = spiflash_read_uint8();
		Serial_hex(byte);
		if (i % 16 == 15) {
			Serial.println();
		}
	}

	spiflash_read_end();
}


void write_page(uint32_t addr) {
	addr += PAGE_SIZE - 1;
	// Writing to 0x10ff wrap addr to begin of page: 0x1000, NOT to 0x1100.

	spiflash_write_start(addr);

	// You can clear bits, but not set bits.
	// Use spiflash_erase4k() or spiflash_chip_erase() to set all bits.
	spiflash_write(0x11);
	spiflash_write_uint8(0x22);

	spiflash_write_end();
}


void erase_page(uint32_t addr) {
	Serial.print(F("Erasing.."));
	Serial_hex(addr);

	spiflash_erase4k(addr);

	Serial.println(F("done"));
}


void dump_all(void) {
	uint32_t size = spiflash_capacity();

	for (uint32_t addr = 0; addr < size; addr += PAGE_SIZE) {
		read_page(addr);
	}
}


bool print_ask = true;

void loop() {
	if (print_ask) Serial.println(F("[-+rwed]?"));
	print_ask = false;
	if (!Serial.available()) return;
	print_ask = true;

	switch (Serial.read()) {
	case '-':
		addr -= 2 * PAGE_SIZE;
		// Fall through
	case '+':
		addr += PAGE_SIZE;
		// Fall through
	case 'r':
		read_page(addr);
		break;
	case 'w':
		write_page(addr);
		read_page(addr);
		break;
	case 'e':
		erase_page(addr);
		delay(1000);
		break;
	case 'd':
		dump_all();
		break;
	default:
		delay(1000);
	};
}


/*
 * Local Variables:
 *  compile-command: "arduino --upload --preserve-temp-files -v Dump.ino"
 * End:
 */
