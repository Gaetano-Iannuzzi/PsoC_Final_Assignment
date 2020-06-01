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
    isr_ADC_StartEx(Custom_ISR_ADC);
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    /* Start the ISR that manage the help and the UART command from host machine*/
    isr_MENU_StartEx(Custom_ISR_MENU);
    isr_Button_StartEx(Custom_Button_ISR);
    
    /* Recover last accelerometer configuration from EEPROM */
    uint8_t ConfExist;
    
    ConfExist = EEPROM_readByte(EEPROM_FREQ_CONFIG);  //Reading Sampling Frequency
    ACC_writeByte(LIS3DH_CTRL_REG1, ConfExist);    //Writing the value of sampling frequency
    
    ConfExist = EEPROM_readByte(EEPROM_FULLSCALE_CONFIG); //Reading FSO
    ACC_writeByte(LIS3DH_CTRL_REG4, ConfExist);
    
    ConfExist = EEPROM_readByte(EEPROM_TEMP_CONFIG);    //Reading Temperature mode 
    if (ConfExist == CELSIUS)       {   TempUnit = CELSIUS;      }
    if (ConfExist == FAHRENHEIT)    {   TempUnit = FAHRENHEIT;   }
    
    ConfExist = EEPROM_readByte(EEPROM_START_STOP_CONFIG);   // Reading Start or Stop 
    if (ConfExist == START) Registration_Active = START;
    if (ConfExist == STOP)  Registration_Active = STOP;
    
    Sensitivity = EEPROM_readByte(EEPROM_SENSITIVITY_VALUE); // Sensitivity that depends on full scale range
    
    EEPROM_readPage(EEPROM_ADDRESS_INDEX, (uint8_t*)&samples, 2);  //Write the index of EEPROM of samples
    

    
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
    uint16_t i=0,j=0,n=0,w=0;
    uint8_t  indexT=0;
    uint8_t Packet[60];
    uint8_t Packet_Read[60];
    uint16_t m=0x00,q=0x00;
    int16_t X,Y,Z,T;
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    int32_t OutX32,OutY32,OutZ32; //int32 values of acceleration after the cast of the floating point
    float32 AccX,AccY,AccZ; //floating point values in m/s^2
    uint8_t OutArray[16]; // 14 bytes for the Output data + 1 byte for Header + 1 for the footer
    OutArray[0] = header;
    OutArray[15] = footer;
    uint16_t samples_read = 0x00;
    
    int PacketInEEPROM = 0;  //Number of Packets stored in the EEPROM
    
    UART_PutString("*********    HELP: POSSIBLE SETTINGS    *********\r\r");
    UART_PutString("f. Full Scale\r\np. Sampling Freq\r\nt. Set Temperature Unit\r\nv. Print Data on BCP\r\nb. Start Registration\r\ns. Stop Registration\r\n\r");        
    UART_PutString("***********************************************\r\n");
    
    for(;;)
    {
        //SAMPLING OF THE DATA
        if (Registration_Active == 1)  //If the 'b' is pressed, Start of sampling
        {
            if ((WTM_Full == 1) && (PacketInEEPROM <= MAX_PACKET_IN_EEPROM))
            {    
                ACC_readMultibytes(LIS3DH_OUT_X_L, &ReadData[0],60);
                i+=6; 
                for ( i=6; i<60; i+=6)
                 {
                    if( PacketReadyFlag==1)  //if we have 9 samples of temperature
                    {
                       // Creation of Packet for EEPROM writing 
                        Packet[j]   = (ReadData[i+1]>>2);
                        Packet[j+1] = ((ReadData[i+3]>>4)|(ReadData[i]>>2)|(ReadData[i+1]<<6));
                        Packet[j+2] = ((ReadData[i+5]>>6)|(ReadData[i+2]>>4)|(ReadData[i+3]<<4));
                        Packet[j+3] = ((ReadData[i+4]>>6)|(ReadData[i+5]<<2));
                        Packet[j+4] = value_temp[indexT] & 0xFF;
                        Packet[j+5] = value_temp[indexT] >> 8;
                   
                    // Writing of packets in EEPROM
                        EEPROM_writePage((FIRST_EEPROM_REG+m+samples),(uint8_t*) &Packet[j],6);
                        EEPROM_waitForWriteComplete();
                        
                        indexT++;  //index of temperature array
                      
                    }
                    
                     j=j+6;       // Index that manage which 4-byte packet write in EEPROM
                     m=m+0x06;    // Increment the addrress in which start the 4-byte packets
                }
            
                j=0;    // In order to start from Packet[0] in next FIFO reading
                indexT=0;  //In order to start from Valuetemp[0]
                FIFO_Enable();
                PacketReadyFlag=0;  
                p=0;
                samples += 0x36;  //Increment the address of EEPROM
                m= 0x00;
                PacketInEEPROM++;  //incremet the number of EEPROM packets
                WTM_Full = 0;   
                address_index = samples;

            }
            else if(PacketInEEPROM > MAX_PACKET_IN_EEPROM)  //iof the EEPROM is full
            {
                Led_Update(449,224);    //EEPROM_FULL change the blinking led
            }
        }
        
        //VISUALIZATION OF THE DATA
        if(ActiveVisualization == 1) 
        {
           
           for ( n = 0; n < 54*PacketInEEPROM ; n+=54)
            {
                for(w=0; w<54;w+=6)
                {
                   //Reading EEPROM packets
                    EEPROM_readPage(FIRST_EEPROM_REG+q+samples_read,(uint8_t*)&Packet_Read[w],6);
                   
                    X= (int16) (((Packet_Read[w+1]<<2)|(Packet_Read[w]<<10)))>>6;
                    Y = (int16) ((Packet_Read[w+2]<<4)|(Packet_Read[w+1]<<12))>>6;
                    Z = (int16) ((Packet_Read[w+3]<<6)|(Packet_Read[w+2]<<14))>>6;
                    T = (int16) (Packet_Read[w+4] | (Packet_Read[w+5]<<8));
                
                    AccX=  X*(int16)Sensitivity*9.806*0.001; // Multiply the value the sensitivity and 9.806*0.001 m/s^2
                    OutX32 =  AccX*100; //Cast the floating point value to an int32 
                                                       //without loosing information of 2 decimals using the multiplication by 1000
                    OutArray[1] = (uint8_t)(OutX32 & 0xFF);
                    OutArray[2] = (uint8_t)(OutX32 >>8);
                    OutArray[3] = (uint8_t)(OutX32 >>16);
                    OutArray[4] = (uint8_t)(OutX32 >>24);
                    

                    AccY = Y*(int16)Sensitivity*9.806*0.001; // Multiply the value for the sensitivity and 9.806*0.001 m/s^2
                    OutY32 = AccY*100; //Cast the floating point value to an int32 
                                              //without loosing information of 2 decimals using the multiplication by 1000  
                    OutArray[5] = (uint8_t)(OutY32 & 0xFF);
                    OutArray[6] = (uint8_t)(OutY32 >> 8);
                    OutArray[7] = (uint8_t)(OutY32 >>16);
                    OutArray[8] = (uint8_t)(OutY32 >>24);
                        
                    AccZ = Z*(int16)Sensitivity*9.806*0.001; // Multiply the value for the sensitivity  and 9.806*0.001 m/s^2
                    OutZ32 = AccZ*100;//Cast the floating point value to an int32 
                                           //without loosing information of 2 decimals using the multiplication by 1000  
                    OutArray[9] = (uint8_t)(OutZ32 & 0xFF);
                    OutArray[10] =(uint8_t)(OutZ32 >> 8);
                    OutArray[11] = (uint8_t)(OutZ32 >>16);
                    OutArray[12] = (uint8_t)(OutZ32 >>24);

                    OutArray[13] = (uint8_t)(T & 0xFF);
                    OutArray[14] = (uint8_t)(T >>8);

                    CyDelay(100);
                    UART_PutArray(OutArray, 16); //Send data to Uart of LIS3DH and Temperature Sensor
                    q = q+0x06;
                }
                w=0;
                q =0;
                samples_read = samples_read+ 0x36; 
            }
            n=0;
            samples_read = 0x00;
        }
    }
}

/* [] END OF FILE */
