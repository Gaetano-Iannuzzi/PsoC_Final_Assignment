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
#include "LIS3DH.h"

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
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    /*Selection of the sampling frequency in Normal Mode*/
    uint8_t ctrl_reg1 =0;
    /* 1Hz */
    
    ctrl_reg1 = ACC_readByte(LIS3DH_CTRL_REG1);
    if (ctrl_reg1 != LIS3DH_1HZ_CTRL_REG1) 
    {
        ctrl_reg1 = LIS3DH_1HZ_CTRL_REG1;
        ACC_writeByte(LIS3DH_CTRL_REG1,ctrl_reg1);
    }
    
//    /* 10Hz */
//    ctrl_reg1 = ACC_readByte(LIS3DH_CTRL_REG1);
//    if (ctrl_reg1 != LIS3DH_10HZ_CTRL_REG1) 
//    {
//        ctrl_reg1 = LIS3DH_10HZ_CTRL_REG1;
//        ACC_writeByte(LIS3DH_CTRL_REG1,ctrl_reg1);
//    }
//    
//    /* 25Hz */
//    ctrl_reg1 = ACC_readByte(LIS3DH_CTRL_REG1);
//    if (ctrl_reg1 != LIS3DH_25HZ_CTRL_REG1) 
//    {
//        ctrl_reg1 = LIS3DH_25HZ_CTRL_REG1;
//        ACC_writeByte(LIS3DH_CTRL_REG1,ctrl_reg1);
//    }
//    
//    /* 50Hz */
//    ctrl_reg1 = ACC_readByte(LIS3DH_CTRL_REG1);
//    if (ctrl_reg1 != LIS3DH_50HZ_CTRL_REG1) 
//    {
//        ctrl_reg1 = LIS3DH_50HZ_CTRL_REG1;
//        ACC_writeByte(LIS3DH_CTRL_REG1,ctrl_reg1);
//    }
    
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
        s=4;
    }
    
//    if(ctrl_reg4 != LIS3DH_4G_CTRL_REG4)
//    {
//        ctrl_reg4 = LIS3DH_4G_CTRL_REG4;
//        ACC_writeByte(LIS3DH_CTRL_REG4,ctrl_reg4);
//        s=8;
//    }
//    
//     if(ctrl_reg4 != LIS3DH_8G_CTRL_REG4)
//    {
//        ctrl_reg4 = LIS3DH_8G_CTRL_REG4;
//        ACC_writeByte(LIS3DH_CTRL_REG4,ctrl_reg4);
//        s=16;
//    }
//    
//     if(ctrl_reg4 != LIS3DH_16G_CTRL_REG4)
//    {
//        ctrl_reg4 = LIS3DH_16G_CTRL_REG4;
//        ACC_writeByte(LIS3DH_CTRL_REG4,ctrl_reg4);
//        s=48;
//    }
    
    
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
    int16_t i=0,j=0;
    int16_t OutX,OutY,OutZ;
    uint8_t Packet[40];
    int16_t samples=0;
    int16_t X,Y,Z;
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    
    uint8_t OutArray[8]; // 6 bytes for the Output data + 1 byte for Header + 1 for the footer
    OutArray[0] = header;
    OutArray[7] = footer;
    int flag=0;
    for(;;)
    {
        if(flag==0){
        if( INT_1_Read() == 1)
        {
            ACC_readMultibytes(LIS3DH_OUT_X_L, &ReadData[0],60);
            
             
                   EEPROM_writePage((0x0000),(uint8_t*)ReadData ,4); 
           
            
            for ( i=0; i<60; i++)
            {
                for(j=0; j<40; j++) 
                {
                    
                    OutX = (int16)((ReadData[i] | (ReadData[i+1]<<8)))>>6;
                    OutY = (int16)((ReadData[i+2] | (ReadData[i+3]<<8)))>>6;
                    OutZ = (int16)((ReadData[i+4] | (ReadData[i+5]<<8)))>>6;
                    i+=5;                
                    Packet[j]= OutX>>4;
                    Packet[j+1] = (OutX<<6 | OutY>>6);
                    Packet[j+2] = (OutY<<4 | OutZ>>8);
                    Packet[j+3] = (OutZ);
                    EEPROM_writePage((0x0001+j+samples),(uint8_t*) Packet,4);
                    EEPROM_waitForWriteComplete();
                    j+=3;
                }
            }
            
            j=0;
            i=0;
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
            samples +=1;
            flag=1;
        }
        }
        
        if(flag==1)
            {
                EEPROM_readPage(0x0001,(uint8_t*) Packet,4);
               
            X= (((Packet[2] & 0xF0)>>4)| Packet[1]>>4);
            X = X*s;// Multiply the value for 4 because the sensitivity is 4 mg/digit
            OutArray[1] = (uint8_t)(X & 0xFF);
            OutArray[2] = (uint8_t)(X >> 8);
            
            Y= ((Packet[3]>>2)|(((Packet[2]& 0x0F))>>2));
            Y= Y*s;
            OutArray[3] = (uint8_t)(Y & 0xFF);
            OutArray[4] = (uint8_t)(Y >> 8);
             
            Z= (((Packet[3]& 0x03)<<8)|Packet[4]);
            Z= Z*s;
            OutArray[5] = (uint8_t)(Z & 0xFF);
            OutArray[6] = (uint8_t)(Z >> 8);
          
    
  
        }
    }
    
}

/* [] END OF FILE */
