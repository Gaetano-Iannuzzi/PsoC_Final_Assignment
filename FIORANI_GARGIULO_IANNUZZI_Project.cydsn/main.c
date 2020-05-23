/**
 * @file main.c
 *
 * 
 *
 *@authors Simone Fiorani, Alessandra Gargiulo, Gaetano Iannuzzi
 *@date 2020
*/

#include "project.h"
#include <stdio.h>
#include <string.h>
#include "SPI_Interface.h"
#include "LIS3DH_Registers.h"
/* EEPROM 25LC256 Library */
#include "25LC256.h"
#include "ErrorCodes.h"

#define UART_PutBuffer UART_PutString(bufferUART)
char bufferUART[100];



int main(void) {
    
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    
    /* Start UART */
    UART_Start();
    
    /* Start SPI Master */
    SPIM_1_Start();
    SPIM_2_Start();
    
    CyDelay(10);
    
    /*Enable FIFO Mode */
    uint8_t ctrl_reg5 = ACC_readByte(LIS3DH_CTRL_REG5);
    if (ctrl_reg5 != LIS3DH_FIFO_ENABLE_CTRL_REG5){
        ctrl_reg5 = LIS3DH_FIFO_ENABLE_CTRL_REG5;
        ACC_writeByte(LIS3DH_CTRL_REG5,ctrl_reg5);
    }
    
    uint8_t fifo_reg = ACC_readByte(LIS3DH_FIFO_CTRL_REG);
    if( fifo_reg != LIS3DH_FIFO_MODE_CTRL_REG) {
        fifo_reg = LIS3DH_FIFO_MODE_CTRL_REG;
        ACC_writeByte(LIS3DH_FIFO_CTRL_REG,fifo_reg);
    }

    
    for(;;){
   
    
    }
    
}

/* [] END OF FILE */
