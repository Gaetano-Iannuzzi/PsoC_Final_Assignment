/**
 * @file main.c
 * @brief Main file of the EEPROM 25LC256 Project.
 * 
 * This file implements a functionality test of the 25LC256 SPI EEPROM
 * interfaced using the library hereby implemented.
 *
 * @author Mattia Pesenti
 * @date 13-May-2020
*/

#include "project.h"
#include <stdio.h>
#include <string.h>
#include "SPI_Interface.h"
#include "LIS3DH_Registers.h"

#define UART_PutBuffer UART_PutString(bufferUART)
char bufferUART[100];

/* EEPROM 25LC256 Library */
#include "25LC256.h"

int main(void) {
    
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    
    /* Start UART */
    UART_Start();
    
    /* Start SPI Master */
    SPIM_1_Start();
    SPIM_2_Start();
    
    CyDelay(10);
    
    UART_PutString("*********    EEPROM TEST    *********\r\n");
    
    /* Definition of the extern from 25LC256.c */
    //uint8_t eeprom_Status = 0;
    
    /* Value to Write */
    uint8_t data = 55;
    
    /* Write */
    EEPROM_writeByte(0x0000, data);
    EEPROM_waitForWriteComplete();
    
    /* Read */
    uint8_t data_read = EEPROM_readByte(0x0000);
    
    sprintf(bufferUART, "** EEPROM Read = %d (%d)\r\n", data_read, data);
    UART_PutBuffer;
    
    UART_PutString("*************************************\r\n");
    
   
    
    UART_PutString("******* ACCE TEST *********\r\n");
    uint8_t status_read = 0;
    status_read = ACC_readByte(LIS3DH_CTRL_REG1);
    sprintf(bufferUART, "** ACCE Read = (%d)\r\n", status_read);
    UART_PutBuffer;
    
    UART_PutString("*************************************\r\n");
    
    return 0;
}

/* [] END OF FILE */
