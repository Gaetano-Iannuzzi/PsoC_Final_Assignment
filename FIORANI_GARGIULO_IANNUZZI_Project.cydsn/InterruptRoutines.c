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
int giro=0;
uint8_t TempData[2];
int j=0,temperature;
char F;
int32 value_temp;
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
        TempData[j+4] = value_digit >> 8;
        TempData[j+5] = value_digit & 0xFF;
        if( temperature == F){
         value_temp = (value_temp*9/5)+32;   
        }
        PacketReadyFlag=1;
    }
}


CY_ISR(Custom_Button_ISR)
{  
     PushButtonCounter ++;  // increment of the variable
    if(PushButtonCounter > 499) Reset_Flag = 1;
}
CY_ISR(Custom_isr_1)
{
    giro=1;
}
/* [] END OF FILE */
