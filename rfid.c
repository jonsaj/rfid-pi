#include <stdio.h>
#include <bcm2835.h>

void main(){
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
	bcm2835_spi_setBitOrder(
}
