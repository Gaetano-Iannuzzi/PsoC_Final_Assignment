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
    
    /*Selection of the sampling frequency in Normal Mode*/
    uint8_t ctrl_reg1 =0;
    /* 1Hz */
    
    ctrl_reg1 = ACC_readByte(LIS3DH_CTRL_REG1);
    if (ctrl_reg1 != LIS3DH_1HZ_CTRL_REG1) 
    {
        ctrl_reg1 = LIS3DH_1HZ_CTRL_REG1;
        ACC_writeByte(LIS3DH_CTRL_REG1,ctrl_reg1);
    }
    
    /* 10Hz */
    ctrl_reg1 = ACC_readByte(LIS3DH_CTRL_REG1);
    if (ctrl_reg1 != LIS3DH_10HZ_CTRL_REG1) 
    {
        ctrl_reg1 = LIS3DH_10HZ_CTRL_REG1;
        ACC_writeByte(LIS3DH_CTRL_REG1,ctrl_reg1);
    }
    
    /* 25Hz */
    ctrl_reg1 = ACC_readByte(LIS3DH_CTRL_REG1);
    if (ctrl_reg1 != LIS3DH_25HZ_CTRL_REG1) 
    {
        ctrl_reg1 = LIS3DH_25HZ_CTRL_REG1;
        ACC_writeByte(LIS3DH_CTRL_REG1,ctrl_reg1);
    }
    
    /* 50Hz */
    ctrl_reg1 = ACC_readByte(LIS3DH_CTRL_REG1);
    if (ctrl_reg1 != LIS3DH_50HZ_CTRL_REG1) 
    {
        ctrl_reg1 = LIS3DH_50HZ_CTRL_REG1;
        ACC_writeByte(LIS3DH_CTRL_REG1,ctrl_reg1);
    }
    
     /* FIFO overrun interrupt on INT1 */
    uint8_t ctrl_reg3 = 0;
    ctrl_reg3 = ACC_readByte(LIS3DH_CTRL_REG3);
    if (ctrl_reg3 != LIS3DH_I1_WTM_OVR_CTRL_REG3)
    {
        ctrl_reg3 = LIS3DH_I1_WTM_OVR_CTRL_REG3;
        ACC_writeByte(LIS3DH_CTRL_REG3,ctrl_reg3);
    }
    
    /*Setting Control Register 4, Full Scale Range*/
    uint8_t ctrl_reg4 = ACC_readByte(LIS3DH_CTRL_REG4);
    
    /*Defining sensitivity*/
    int s =0; 
    
    if(ctrl_reg4 != LIS3DH_2G_CTRL_REG4)
    {
        ctrl_reg4 = LIS3DH_2G_CTRL_REG4;
        ACC_writeByte(LIS3DH_CTRL_REG4,ctrl_reg4);
        s=2;
    }
    
    if(ctrl_reg4 != LIS3DH_4G_CTRL_REG4)
    {
        ctrl_reg4 = LIS3DH_4G_CTRL_REG4;
        ACC_writeByte(LIS3DH_CTRL_REG4,ctrl_reg4);
        s=4;
    }
    
     if(ctrl_reg4 != LIS3DH_8G_CTRL_REG4)
    {
        ctrl_reg4 = LIS3DH_8G_CTRL_REG4;
        ACC_writeByte(LIS3DH_CTRL_REG4,ctrl_reg4);
        s=8;
    }
    
     if(ctrl_reg4 != LIS3DH_16G_CTRL_REG4)
    {
        ctrl_reg4 = LIS3DH_16G_CTRL_REG4;
        ACC_writeByte(LIS3DH_CTRL_REG4,ctrl_reg4);
        s=16;
    }
    
    
    /*Enable FIFO Mode */
    uint8_t ctrl_reg5 = ACC_readByte(LIS3DH_CTRL_REG5);
    if (ctrl_reg5 != LIS3DH_FIFO_ENABLE_CTRL_REG5)
    {
        ctrl_reg5 = LIS3DH_FIFO_ENABLE_CTRL_REG5;
        ACC_writeByte(LIS3DH_CTRL_REG5,ctrl_reg5);
    }
    
    /* FIFO Mode Selection */
    uint8_t fifo_reg = 0;
    /*FIFO */
    fifo_reg = ACC_readByte(LIS3DH_FIFO_CTRL_REG);
    if( fifo_reg != LIS3DH_FIFO_MODE_CTRL_REG) 
    {
        fifo_reg = LIS3DH_FIFO_MODE_CTRL_REG;
        ACC_writeByte(LIS3DH_FIFO_CTRL_REG,fifo_reg);
    }
    
    
     /*WTM 10 */
    fifo_reg = ACC_readByte(LIS3DH_FIFO_CTRL_REG);
    if( fifo_reg != LIS3DH_WTM10_MODE_CTRL_REG5) 
    {
        fifo_reg = LIS3DH_WTM10_MODE_CTRL_REG5;
        ACC_writeByte(LIS3DH_FIFO_CTRL_REG,fifo_reg);
    }
    
    uint8_t ReadData[60];
    uint8_t TransferData[3];
    int i=0;
    int16_t OutX,OutY,OutZ;
    
    
    for(;;)
    {
        if( INT_1_Read() == 1)
        {
            ACC_readMultibytes(LIS3DH_OUT_X_L, &ReadData[0],60);
            
//            for ( i=0; i<60; i++){
//                OutX = (int16)((ReadData[i] | (ReadData[i+1]<<8)))>>6;
//                OutY = (int16)((ReadData[i+2] | (ReadData[i+3]<<8)))>>6;
//                OutZ = (int16)((ReadData[i+4] | (ReadData[i+5]<<8)))>>6;
                
            
            
            /*Bypass Mode */
            fifo_reg = ACC_readByte(LIS3DH_FIFO_CTRL_REG);
            if( fifo_reg != LIS3DH_BYPASS_MODE_CTRL_REG) 
            {
                fifo_reg = LIS3DH_BYPASS_MODE_CTRL_REG;
                ACC_writeByte(LIS3DH_FIFO_CTRL_REG,fifo_reg);
            }
            /*FIFO */
            fifo_reg = ACC_readByte(LIS3DH_FIFO_CTRL_REG);
            if( fifo_reg != LIS3DH_FIFO_MODE_CTRL_REG) 
            {
                fifo_reg = LIS3DH_FIFO_MODE_CTRL_REG;
                ACC_writeByte(LIS3DH_FIFO_CTRL_REG,fifo_reg);
            }
        }
    
    
    }
    
}

/* [] END OF FILE */
