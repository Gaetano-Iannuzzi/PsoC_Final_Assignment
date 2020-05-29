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
#include "Menu_Functions.h"

// Include required header files
#include "project.h"

// Variables declaration
int32 value_digit;
int32 value_mv;
uint8 SendBytesFlag=0;
uint16_t PushButtonCounter = 0;  // Initialitazion of the variable
int Reset_Flag = 0;
int giro=0;
uint8_t TempData[2];
char F;
int     value_temp = 0;
float   value_temp_float = 0;

int WTM_Full;

CY_ISR(Custom_ISR_ADC)
{
    // Read Timer status register to bring interrupt line low
    Timer_ReadStatusRegister();
    
    
    value_digit = ADC_DelSig_Read32();
        
    if (value_digit < 0)        value_digit = 0;
    if (value_digit > 65535)    value_digit = 65535;
    
    value_mv = ADC_DelSig_CountsTo_mVolts(value_digit);

    if      (TempUnit == CELSIUS)           
    {
    value_temp_float = (value_mv-500)/10;
    value_temp = (int16)(value_temp_float * 100);
    }
    else if (TempUnit == FAHRENHEIT)
    {
        value_temp_float = (value_mv-500)/10;
        value_temp_float = (value_temp_float*9/5)+32;
        value_temp = (int16)(value_temp_float * 100); 
    }
    
    PacketReadyFlag=1;
    }



CY_ISR(Custom_Button_ISR)
{  
    PushButtonCounter ++;  // increment of the variable
    if(PushButtonCounter > 499) FIFO_Reset();
}

CY_ISR(Custom_isr_ACC)
{
    WTM_Full=1;
}
/* [] END OF FILE */
