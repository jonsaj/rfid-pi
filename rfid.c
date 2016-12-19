#include <stdio.h>
//#include <bcm2835.h>
#include <wiringPiSPI.h>
#include <unistd.h>

//channel definition
#define SPI_CHANNEL	0
#define SPI_SPEED	1000000

//register definitions
#define REG_COMMAND	0x01
#define REG_COMINTRQEN	0x02

#define REG_BITFRAMING	0x0D
#define REG_COMIRQ	0x04

#define REG_ERROR	0x06
#define REG_FIFODATA	0x09
#define REG_FIFOLEVEL	0x0A
#define REG_MODE	0x11
#define REG_RXMODE	0x13
#define REG_TIMERMODE	0x2A
#define REG_TIMERSCALE	0x2B
#define REG_TIMERRLH	0x2C
#define REG_TIMERRLL	0x2D
#define REG_TXASK	0x15
#define REG_TXCONTROL	0x14
#define REG_TXMODE	0x12
#define REG_VERSION	0x37
#define REG_AUTOTEST	0x36

//Commands
//pcd
#define COMM_IDLE	0x00
#define COMM_MEM	0x01
#define COMM_GENRANDID	0x02
#define COMM_CALCCRC	0x03
#define COMM_TRANSMIT	0x04
#define COMM_NOCMDCHG	0x07
#define COMM_RECEIVE	0x08
#define COMM_TRANSVE	0x0C
#define COMM_MIFAUTH	0x0E
#define COMM_SOFTRESET	0x15

//picc
#define COMM_ANTICOLL_1	0x93
#define COMM_ANTICOLL_2	0x20
#define COMM_AUTHKEY_A	0x60
#define COMM_AUTHKEY_B	0x61
#define COMM_HALT_1	0x50
#define COMM_READ	0x30
#define COMM_REQUEST	0x26
#define COMM_SELECT_1	0x93
#define COMM_SELECT_2	0x70
#define	COMM_WRITE	0xA0

#define TEST_SELFTEST	0x09

//picc responses
#define RESP_ANSRQ	0x04
#define RESP_SELECTACK	0x08
#define RESP_ACK	0x0A

void setRead(char *data){
	data[0] = data[0] & 0xff;
}

void setWrite(char *data){
	data[0] = data[0] & 0x7f;
}

void setAddr(char *data, const char addr){
	data[0] = (data[0] & 0x81) | (0x81 | (addr << 1));	
}

void clearLSB(char *data){
	data[0] = data[0] & 0xfe;
}

void setComm(char *data, const char comm){
	data[1] = (data[1] & ~(0xF)) & (0xF & comm);
}

void printBits(const char* message, char data){
	printf("%s ", message);
	int i;
	for (i = 0; i < 8; i++) {
		printf("%d", !!((data << i) & 0x80));
	}
	printf("\n");
}

int spiInit(){
	int fd = wiringPiSPISetup(SPI_CHANNEL,SPI_SPEED);
	if(fd < 0){
		printf("SPI init failed!\n");
		return 1;
	}
	return fd;

}

void piSelfTest(){
	unsigned char data = 'a';
	int i = 0;
	for(i=0;i<30;i++){
		wiringPiSPIDataRW(SPI_CHANNEL,&data,1);
		printf("data is 0x%02x %c\n",data,data);
		if(data) data += 0x1;
		else data = 'A';
		usleep(500000);
	} 
}

void getVersion(){
	unsigned char data;
	setRead(&data);
	setAddr(&data, REG_VERSION);
	clearLSB(&data);
	wiringPiSPIDataRW(SPI_CHANNEL, &data, 1);
	wiringPiSPIDataRW(SPI_CHANNEL, &data, 1);
	if(data >> 4 == 0x9) {
		printf("ChipType: MFRC522\n");
	} else {
		printf("ChipType: UNRECOGNIZED\n");
	}
	printf("Version: %i.0\n", data & 0xf);	
}

void softReset(){
	unsigned char data;
	clearLSB(&data);
	setWrite(&data);
	setAddr(&data,REG_COMMAND);
	setComm(&data,COMM_SOFTRESET);
	wiringPiSPIDataRW(SPI_CHANNEL, &data, 2);
	//usleep(500000); //unsure if sleep needed after softreset
	//considering that commandReg register has a PowerDown
	// soft power down field, I think no delay is needed
}

void clearInternalBuffer(){
//clear FIFO buffer
	//setAddr(&data,
	setComm(&data, COMM_MEM);
	wiringPiSPIDataRW(SPI_CHANNEL, &data, 1);
}

void rfidSelfTest(){
	softReset();
	clearBuffer();

	unsigned short testBytesCount = 64;
	unsigned char data[testBytesCount];
	data[0] = 0x0;
	setAddr(&data,REG_AUTOTEST);
	setWrite(&data);
	clearLSB(&data);
	data[1] = data[1] & TEST_SELFTEST;
	int i;
	for(i=0;i<testBytesCount;i++){
		wiringPiSPIDataRW(SPI_CHANNEL, &data, testBytesCount);	
	}

}

int main(){
	spiInit();
	getVersion();


	return 0;
}

