#include <stdio.h>
#include <bcm2835.h>

int main(){
	if(!bcm2835_init())
	{
		printf("bcm2835_init failed. Root maybe?\n");
		return 1;	
	}
	
	if(!bcm2835_spi_begin())
	{
		printf("bcm2835_spi_begin failed. Root maybe?\n");
		return 1;
	}
	printf("began spi\n");
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	printf("set bir order\n");
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); 
	printf("set data mode\n");
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
	printf("set clock divider\n");
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	printf("set chipselect\n");
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	printf("set chip select polarity\n");

	// Send a byte to the slave and simultaneously read a byte back from the slave
	// If you tie MISO to MOSI, you should read back what was sent
	uint8_t send_data = 0x23;
	uint8_t read_data = bcm2835_spi_transfer(send_data);
	printf("Sent to SPI: 0x%02X. Read back from SPI: 0x%02X.\n", send_data, read_data);
	if (send_data != read_data)
  		printf("Do you have the loopback from MOSI to MISO connected?\n");
	bcm2835_spi_end();
	bcm2835_close();
	return 0;
}
