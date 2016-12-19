#include <stdio.h>
//#include <bcm2835.h>
#include <wiringPiSPI.h>
#include <unistd.h>

//register definitions
#define REG_BITFRAMING	0x0D;
#define REG_COMIRQ	0x04;
#define REG_COMIRQEN	0x02;
#define REG_COMMAND	0x01;
#define REG_ERROR	0x06;
#define REG_FIFODATA	0x09;
#define REG_FIFOLEVEL	0x0A;
#define REG_MODE	0x11;
#define REG_RXMODE	0x13;
#define REG_TIMERMODE	0x2A;
#define REG_TIMERSCALE	0x2B;
#define REG_TIMERRLH	0x2C;
#define REG_TIMERRLL	0x2D;
#define REG_TXASK	0x15;
#define REG_TXCONTROL	0x14;
#define REG_TXMODE	0x12;
#define REG_VERSION	0x37;

//Commands
//pcd
#define COMM_IDLE	0x00;
#define COMM_MEM	0x01;
#define COMM_GENRANDID	0x02;
#define COMM_CALCCRC	0x03;
#define COMM_TRANSMIT	0x04;
#define COMM_NOCMDCHG	0x07;
#define COMM_RECEIVE	0x08;
#define COMM_TRANSVE	0x0C;


#define COMM_MIFAUTH	0x0E;

#define COMM_SOFTRESET	0x15;

//picc
#define COMM_ANTICOLL_1	0x93;
#define COMM_ANTICOLL_2	0x20;
#define COMM_AUTHKEY_A	0x60;
#define COMM_AUTHKEY_B	0x61;
#define COMM_HALT_1	0x50;
#define COMM_READ	0x30;
#define COMM_REQUEST	0x26;
#define COMM_SELECT_1	0x93;
#define COMM_SELECT_2	0x70;
#define	COMM_WRITE	0xA0;

//picc responses
#define RESP_ANSRQ	0x04;
#define RESP_SELECTACK	0x08;
#define RESP_ACK	0x0A;

void selfTest(){
	unsigned char data = 'a';
	int i = 0;
	for(i=0;i<30;i++){
		wiringPiSPIDataRW(0,&data,1);
		printf("data is 0x%02x %c\n",data,data);
		if(data) data += 0x1;
		else data = 'A';
		usleep(500000);
	} 
}

int main(){

	int fd = wiringPiSPISetup(0,1000000);
	if(fd < 0){
		printf("SPI init failed!\n");
		return 1;
	}
	// selfTest();
	unsigned char data = 0x0;
	wiringPiSPISetupRW(REG_VERSION, &data, 1);
	printf("0x%02xCHIPTYPE: %2x, VERISION: %2x\n", data,data >> 4, data & 0xF);
	wiringPiSPISetupRW(REG_VERSION, &data, 1);
	printf("0x%02xCHIPTYPE: %2x, VERISION: %2x\n", data,data >> 4, data & 0xF);
	return 0;
}

