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
#include "InterruptRoutines.h"
/* EEPROM 25LC256 Library */
#include "25LC256.h"
#include "Menu_Functions.h"
#include "OnBoardLed.h"
#define UART_PutBuffer UART_PutString(bufferUART)
char bufferUART[100];



int main(void) {
    
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    
    /* Start UART */
    UART_Start();
//    int TurnedON=0;
    /* Start SPI Master */
    SPIM_1_Start();
    SPIM_2_Start();
    Led_Start();
    isr_1_StartEx(Custom_isr_1);
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
//    isr_MENU_StartEx(Custom_ISR_MENU);
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
    int s; 
    
    if(ctrl_reg4 != LIS3DH_2G_CTRL_REG4)
    {
        ctrl_reg4 = LIS3DH_2G_CTRL_REG4;
        ACC_writeByte(LIS3DH_CTRL_REG4,ctrl_reg4);
        s =4;
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
    uint8_t ReadData[600];
    uint8_t TransferData[3];
    int16_t i=0,j=0,k=0,m=0;
    int16_t OutX,OutY,OutZ;
    uint8_t Packet[80],Packet_Read[80];
    int16_t samples=0;
    int16_t X,Y,Z;
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    int32_t OutX32,OutY32,OutZ32; //int32 values of acceleration after the cast of the floating point
    float32 AccX,AccY,AccZ; //floating point values in m/s^2
    uint8_t OutArray[14]; // 6 bytes for the Output data + 1 byte for Header + 1 for the footer
    OutArray[0] = header;
    OutArray[13] = footer;
    int flag=0;
    for(;;)
    {
        if(flag<2){
        if( giro==1)
        { 
           Led_Update(999,499);
            ACC_readMultibytes(LIS3DH_OUT_X_L, &ReadData[0],60);
           
            for ( i=0; i<60; i+=6)
            {
                  
                OutX = (int16)((ReadData[i] | (ReadData[i+1]<<8)))>>6;
                OutY = (int16)((ReadData[i+2] | (ReadData[i+3]<<8)))>>6;
                OutZ = (int16)((ReadData[i+4] | (ReadData[i+5]<<8)))>>6;               
                Packet[j]= OutX>>4;
                Packet[j+1] = ((OutX<<4) | (OutY>>6));
                Packet[j+2] = ((OutY<<2) | (OutZ>>8));
                Packet[j+3] = (OutZ);
                EEPROM_writePage((0x0010+j+samples),& Packet[j],4);
                EEPROM_waitForWriteComplete();
                j+=4;
            
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
            samples +=10;
            flag+=1;
            giro = 0;
        }
        }
        
        if(flag==2)
            {
            
                EEPROM_readPage(0x0010,&Packet_Read[0],80);
               Pin_ExternalLED_Write(1);
            for(k=0;k<80;k+=4){
                
                X= (((Packet_Read[k+1] & 0xF0)>>4)| Packet_Read[k]<<4);
                
                AccX=  X*4*9.806*0.001; // Multiply the value for 2 because the sensitivity is 2 mg/digit and 9.806*0.001 m/s^2
                        OutX32 =  AccX*1000; //Cast the floating point value to an int32 
                                                   //without loosing information of 3 decimals using the multiplication by 1000
                    OutArray[1] = (uint8_t)(OutX32 & 0xFF);
                    OutArray[2] = (uint8_t)(OutX32 >>8);
                    OutArray[3] = (uint8_t)(OutX32 >>16);
                    OutArray[4] = (uint8_t)(OutX32 >>24);
                Y= ((Packet_Read[k+2]>>2)|(((Packet_Read[k+1]& 0x0F))<<6));
               
                AccY = Y*4*9.806*0.001; // Multiply the value for  2 because the sensitivity is 2 mg/digit and 9.806*0.001 m/s^2
                        OutY32 = AccY*1000; //Cast the floating point value to an int32 
                                                  //without loosing information of 3 decimals using the multiplication by 1000  
                        OutArray[5] = (uint8_t)(OutY32 & 0xFF);
                        OutArray[6] = (uint8_t)(OutY32 >> 8);
                        OutArray[7] = (uint8_t)(OutY32 >>16);
                        OutArray[8] = (uint8_t)(OutY32 >>24);
                        
            Z= ((Packet_Read[k+3]|(Packet_Read[k+2]& 0x03)<<8));
            
             AccZ = Z*4*9.806*0.001; // Multiply the value for 2 because the sensitivity is 2 mg/digit and 9.806*0.001 m/s^2
                        OutZ32 = AccZ*1000;//Cast the floating point value to an int32 
                                               //without loosing information of 3 decimals using the multiplication by 1000  
                        OutArray[9] = (uint8_t)(OutZ32 & 0xFF);
                        OutArray[10] =(uint8_t)(OutZ32 >> 8);
                        OutArray[11] = (uint8_t)(OutZ32 >>16);
                        OutArray[12] = (uint8_t)(OutZ32 >>24);
         
            
            sprintf(bufferUART, "** EEPROM Read = %d %d %d \r\n", (uint8_t) X,(uint8_t)Y,(uint8_t)Z);
            UART_PutBuffer;
            
            UART_PutString("*************************************\r\n");
////            
//////            return 0;
//            UART_PutArray(OutArray, 14); //Send data to Uart (values in [mg])
                   
        }
            Pin_ExternalLED_Write(0);
                     return 0;  
    }
    }
}

/* [] END OF FILE */
