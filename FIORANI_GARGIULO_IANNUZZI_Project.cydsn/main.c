/**
 *LABORATORY FINAL PROJECT 
 *
 *@file main.c
 *
 * 
 *
 *@authors Simone Fiorani, Alessandra Gargiulo, Gaetano Iannuzzi
 *@date 2020
*/

#include "project.h"

#include <stdio.h>
#include <string.h>

#include "SPI_Interface.h"      // Functions for R/W through SPI protocol
#include "LIS3DH.h"             // Functions for R/W Accelerometer Registers
#include "InterruptRoutines.h"  // Functions for the different ISRs
#include "25LC256.h"            // Functions for R/W EEPROM registers
#include "Menu_Functions.h"     // Functions for the help and host commands
#include "OnBoardLed.h"         // Functions for on-board led
#include "FIFO_Functions.h"     // Functions for enabling FIFO mode on accelerometer

#define UART_PutBuffer UART_PutString(bufferUART)
char bufferUART[100];

int main(void) {
    
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    
    /* Start UART */
    UART_Start();
    
    ADC_DelSig_Start();
    /* Start SPI Masters */
    SPIM_1_Start();
    SPIM_2_Start();
    
    /* Timer_start */
    Timer_Button_Start();
    /* Start OnBoard Led */
    Led_Start();
    
    /* Start ISR coming from accelerometer WTM */
    isr_ACC_StartEx(Custom_isr_ACC);
    
    /* Start ISR coming from ADC of temperature sensor */
//    isr_ADC_StartEx(Custom_ISR_ADC);
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    /* Start the ISR that manage the help and the UART command from host machine*/
    isr_MENU_StartEx(Custom_ISR_MENU);
    isr_Button_StartEx(Custom_Button_ISR);
//    isr
    /* Recover last accelerometer configuration from EEPROM */
    uint8_t ConfExist;
    
    ConfExist = EEPROM_readByte(EEPROM_FREQ_CONFIG);
    ACC_writeByte(LIS3DH_CTRL_REG1, ConfExist);
    
    ConfExist = EEPROM_readByte(EEPROM_FULLSCALE_CONFIG);
    ACC_writeByte(LIS3DH_CTRL_REG4, ConfExist);
    
    ConfExist = EEPROM_readByte(EEPROM_TEMP_CONFIG);
    if (ConfExist == CELSIUS)       {   TempUnit = CELSIUS;      }
    if (ConfExist == FAHRENHEIT)    {   TempUnit = FAHRENHEIT;   }
    
    ConfExist = EEPROM_readByte(EEPROM_START_STOP_CONFIG);
    if (ConfExist == START) Registration_Active = START;
    if (ConfExist == STOP)  Registration_Active = STOP;
    
    Sensitivity = EEPROM_readByte(EEPROM_SENSITIVITY_VALUE); // Sensitivity that depends on full scale range
    
    EEPROM_readPage(EEPROM_ADDRESS_INDEX, (uint8_t*)&samples, 2);
    

    
     /* FIFO overrun interrupt on INT1 */
    uint8_t ctrl_reg3 = 0;
    ctrl_reg3 = ACC_readByte(LIS3DH_CTRL_REG3);
    if (ctrl_reg3 != LIS3DH_I1_WTM_OVR_CTRL_REG3)
    {
        ctrl_reg3 = LIS3DH_I1_WTM_OVR_CTRL_REG3;
        ACC_writeByte(LIS3DH_CTRL_REG3,0x04);
    }
    
    /*Enable FIFO Mode */
    uint8_t ctrl_reg5 = ACC_readByte(LIS3DH_CTRL_REG5);
    if (ctrl_reg5 != LIS3DH_FIFO_ENABLE_CTRL_REG5)
    {
        ctrl_reg5 = LIS3DH_FIFO_ENABLE_CTRL_REG5;
        ACC_writeByte(LIS3DH_CTRL_REG5,ctrl_reg5);
    }
    
    /* FIFO Mode Selection */
    
     /*WTM 10 */
    fifo_reg = ACC_readByte(LIS3DH_FIFO_CTRL_REG);
    if( fifo_reg != LIS3DH_WTM9_MODE_CTRL_REG5) 
    {
        fifo_reg = LIS3DH_WTM9_MODE_CTRL_REG5;
        ACC_writeByte(LIS3DH_FIFO_CTRL_REG,fifo_reg);
    }
    
    uint8_t ReadData[60];
    uint16_t i=0,j=0,k=0,n=0,w=0;
    uint8_t  indexT=0;
    
    
    int16_t OutX,OutY,OutZ;
    uint8_t Packet[60];
    uint8_t Packet_Read[60];
    uint16_t m=0x00,q=0x00;
    int16_t X,Y,Z,T;
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    int32_t OutX32,OutY32,OutZ32; //int32 values of acceleration after the cast of the floating point
    float32 AccX,AccY,AccZ; //floating point values in m/s^2
    uint8_t OutArray[14]; // 6 bytes for the Output data + 1 byte for Header + 1 for the footer
    OutArray[0] = header;
    OutArray[13] = footer;
    uint16_t campioni = 0x00;
 
 
    
    int PacketInEEPROM = 0;
    
    UART_PutString("*********    HELP: POSSIBLE SETTINGS    *********\r\r");
    UART_PutString("f. Full Scale\r\np. Sampling Freq\r\nt. Set Temperature Unit\r\nv. Print Data on BCP\r\nb. Start Registration\r\ns. Stop Registration\r\n\r");        
    UART_PutString("***********************************************\r\n");
    
    for(;;)
    {
        if (Registration_Active == 1)
        {
            if ((WTM_Full == 1) && (PacketInEEPROM <= MAX_PACKET_IN_EEPROM))
            {    
                ACC_readMultibytes(LIS3DH_OUT_X_L, &ReadData[0],60);
                i+=6; 
                for ( i=6; i<60; i+=6)
                {
                    
                      
//                    OutX = (int16)((ReadData[i] | (ReadData[i+1]<<8)))>>6;
//                    OutY = (int16)((ReadData[i+2] | (ReadData[i+3]<<8)))>>6;
//                    OutZ = (int16)((ReadData[i+4] | (ReadData[i+5]<<8)))>>6;
//                    if( PacketReadyFlag==1)
//                    {
                        
                    Packet[j]   = (ReadData[i+1]>>2);
                    Packet[j+1] = ((ReadData[i+3]>>4)|(ReadData[i]>>2)|(ReadData[i+1]<<6));
                    Packet[j+2] = ((ReadData[i+5]>>6)|(ReadData[i+2]>>4)|(ReadData[i+3]<<4));
                    Packet[j+3] = ((ReadData[i+4]>>6)|(ReadData[i+5]<<2));
                    //se ne scrivo 
                    
                    // Creation of Packet for EEPROM writing
                    
                    
                        


                        EEPROM_writePage((FIRST_EEPROM_REG+m+samples),(uint8_t*) &Packet[j],4);
                        EEPROM_waitForWriteComplete();
//                        Packet[j+4] = value_temp[indexT] & 0xFF;
//                        Packet[j+5] = value_temp[indexT] >> 8;
//                       Packet[j+4]= 0;
//                       Packet[j+5]= 0;
//                      EEPROM_writePage((FIRST_EEPROM_REG+m+samples+0x04),(uint8_t*) &Packet[j+4],2);
//                      EEPROM_waitForWriteComplete();
//                        indexT++;
                      
//                    }
                    
                    // Writing of packets in EEPROM
////                    EEPROM_writePage((FIRST_EEPROM_REG+m+samples),(uint8_t*) &Packet[j],6);
////                    EEPROM_waitForWriteComplete();
                     j=j+4;       // Index that manage which 4-byte packet write in EEPROM
                     m=m+0x04;    // Increment the addrress in which start the 4-byte packet
//                    EEPROM_readPage(FIRST_EEPROM_REG+m+samples,(uint8_t*)&Packet_Read[j],6);
//                    EEPROM_readPage(EEPROM_ADDRESS_INDEX,(uint8_t*)&address_index[0],2);
//                    int16 OX= (int16) (((Packet_Read[j+1]<<2)|(Packet_Read[j]<<10)))>>6;
//                    int16 OY = (int16) ((Packet_Read[j+2]<<4)|(Packet_Read[j+1]<<12))>>6;
//                    int16 OZ = (int16) ((Packet_Read[j+3]<<6)|(Packet_Read[j+2]<<14))>>6;
//                    int16 OT = (int16) (Packet_Read[j+4] | (Packet_Read[j+5]<<8));
                    sprintf(bufferUART, "** EEPROM Read = %d \r\n",  samples);
                    UART_PutBuffer;
            
                    UART_PutString("*************************************\r\n");
                    
                }
            
                j=0;    // In order to start from Packet[0] in next FIFO reading
                i=0;
                indexT=0;
                FIFO_Enable();
                PacketReadyFlag=0;
                p=0;
                samples = samples + 0x24;
                m= 0x00;
                PacketInEEPROM++;
                WTM_Full = 0;
                address_index = samples;

            }
            else if(PacketInEEPROM > MAX_PACKET_IN_EEPROM)
            {
                Led_Update(449,224);    //EEPROM_FULL
            }
        }
        
        if(ActiveVisualization == 1)
        {
           
           for ( n = 0; n < 36*PacketInEEPROM ; n+=36)
            {
                for(w=0; w<36;w+=4)
                {
                   
                    EEPROM_readPage(FIRST_EEPROM_REG+q+campioni,(uint8_t*)&Packet_Read[w],4);
                    
 
                    
                    X= (int16) (((Packet_Read[w+1]<<2)|(Packet_Read[w]<<10)))>>6;
                    Y = (int16) ((Packet_Read[w+2]<<4)|(Packet_Read[w+1]<<12))>>6;
                    Z = (int16) ((Packet_Read[w+3]<<6)|(Packet_Read[w+2]<<14))>>6;
//                    
//                    EEPROM_readPage(FIRST_EEPROM_REG+q+campioni+0x04,(uint8_t*)&Packet_Read[w+4],2);
//                    T = (int16) (Packet_Read[w+4] | (Packet_Read[w+5]<<8));
                
                    AccX=  X*(int16)Sensitivity*9.806*0.001; // Multiply the value the sensitivity and 9.806*0.001 m/s^2
                    OutX32 =  AccX*100; //Cast the floating point value to an int32 
                                                       //without loosing information of 2 decimals using the multiplication by 1000
                    OutArray[1] = (uint8_t)(OutX32 & 0xFF);
                    OutArray[2] = (uint8_t)(OutX32 >>8);
                    OutArray[3] = (uint8_t)(OutX32 >>16);
                    OutArray[4] = (uint8_t)(OutX32 >>24);
                    

//                    Y = (int16) ((Packet_Read[w+2]<<4)|(Packet_Read[w+1]<<12))>>6;
                    AccY = Y*(int16)Sensitivity*9.806*0.001; // Multiply the value for the sensitivity and 9.806*0.001 m/s^2
                    OutY32 = AccY*100; //Cast the floating point value to an int32 
                                              //without loosing information of 2 decimals using the multiplication by 1000  
                    OutArray[5] = (uint8_t)(OutY32 & 0xFF);
                    OutArray[6] = (uint8_t)(OutY32 >> 8);
                    OutArray[7] = (uint8_t)(OutY32 >>16);
                    OutArray[8] = (uint8_t)(OutY32 >>24);
                        
//                    Z = (int16) ((Packet_Read[w+3]<<6)|(Packet_Read[w+2]<<14))>>6;
                     AccZ = Z*(int16)Sensitivity*9.806*0.001; // Multiply the value for the sensitivity  and 9.806*0.001 m/s^2
                    OutZ32 = AccZ*100;//Cast the floating point value to an int32 
                                           //without loosing information of 2 decimals using the multiplication by 1000  
                    OutArray[9] = (uint8_t)(OutZ32 & 0xFF);
                    OutArray[10] =(uint8_t)(OutZ32 >> 8);
                    OutArray[11] = (uint8_t)(OutZ32 >>16);
                    OutArray[12] = (uint8_t)(OutZ32 >>24);
                    
//
//                    OutArray[13] = (uint8_t)(T & 0xFF);
//                    OutArray[14] = (uint8_t)(T >>8);
//         
//            
//                sprintf(bufferUART, "** EEPROM Read = %d %d %d %d \r\n",  X*Sensitivity,Y*Sensitivity,Z*Sensitivity, T/100);
//                UART_PutBuffer;
//        
//                UART_PutString("*************************************\r\n");
////
                    CyDelay(100);
                    UART_PutArray(OutArray, 14); //Send data to Uart (values in [mg])
                    q = q+0x04;
                }
                w=0;
                q =0;
                campioni = campioni+ 0x24; 
            }
            n=0;
            campioni = 0x00;
//            UART_PutString("***********GIROOOOo******************\r\n");
        }
    }
}

/* [] END OF FILE */
