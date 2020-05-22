/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

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

void  ACC_readMultibytes(uint8_t addr, uint8_t* dataRX, uint8_t nBytes) {

    /* Prepare the TX data packet: instruction + address */
	uint8_t dataTX[1] =  {(ACC_MULTI_READ  | addr)};
	/* Prepare the RX byte */
	/* Read the nBytes */
	ACC_SPI_Interface_Multi_RW(dataTX,1 , dataRX, nBytes);

}
void ACC_writeMultibytes(uint8_t addr, uint8_t* data, uint8_t nBytes) {
	    
	
    CyDelayUs(1);
    
	/* Prepare the TX packet of size nBytes+1 
       [ Write Instruction - Address MSB - Address LSB - +++data+++ ]
    */
	uint8_t dataTX[1+nBytes];
    dataTX[0] = (ACC_MULTI_WRITE | addr);
    /* Copy the input data in the memory */
	memcpy(&dataTX[0], data, nBytes);
	
	/* Nothing to RX: point to a dummy variable */
	uint8_t temp = 0;
	
	SPI_Interface_Multi_RW(dataTX, 3+nBytes, &temp, 0);
	
}

/* [] END OF FILE */