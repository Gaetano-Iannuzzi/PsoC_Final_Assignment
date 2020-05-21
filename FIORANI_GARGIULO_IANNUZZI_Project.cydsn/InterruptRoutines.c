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
// Include header
#include "InterruptRoutines.h"

// Include required header files
#include "project.h"

// Variables declaration
int32 value_digit;
uint8 SendBytesFlag=0;
uint16_t PushButtonCounter = 0;  // Initialitazion of the variable
int Reset_Flag = 0;

CY_ISR(Custom_ISR_ADC)
{
    // Read Timer status register to bring interrupt line low
    Timer_ReadStatusRegister();
    
    // Send data based on UART command
    if(SendBytesFlag!=0)
    {
       
        value_digit = ADC_DelSig_Read32();
            
        if (value_digit < 0)        value_digit = 0;
        if (value_digit > 65535)    value_digit = 65535;
        
        // Write bytes in buffer
        DataBuffer[1] = value_digit >> 8;
        DataBuffer[2] = value_digit & 0xFF;
            
        PacketReadyFlag=1;
    }
}


CY_ISR(Custom_Button_ISR)
{  
     PushButtonCounter ++;  // increment of the variable
    if(PushButtonCounter > 499) Reset_Flag = 1;
}

/* [] END OF FILE */
