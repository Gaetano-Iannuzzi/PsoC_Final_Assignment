#include    "LIS3DH_Registers.h"

uint8_t ACC_readByte(uint8_t addr) {

	/* Prepare the TX data packet: instruction + address */
	uint8_t dataTX[1] = { (ACC_SINGLE_READ  | addr) };
	
	/* Prepare the RX byte */
	uint8_t dataRX = 0;
	
	/* Read 1 byte from addr */
	ACC_SPI_Interface_Multi_RW(dataTX, 1, &dataRX, 1);
	
	return dataRX;
    
}

void ACC_writeByte(uint8_t addr, uint8_t dataByte) {
    
	/* Prepare the TX packet */
    uint8_t dataTX[2] = { (ACC_SINGLE_WRITE | addr) , dataByte};
	/* Nothing to RX... */
	uint8_t temp = 0;
	
	/* Write 1 byte to addr */
	ACC_SPI_Interface_Multi_RW(dataTX, 2, &temp, 0);
    
	    
}
