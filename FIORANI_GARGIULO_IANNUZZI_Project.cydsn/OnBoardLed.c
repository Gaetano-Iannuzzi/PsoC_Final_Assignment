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
 *@authors Simone Fiorani, Alessandra Gargiulo, Gaetano Iannuzzi
 *@date 2020
*/
#include "OnBoardLed.h"

void Led_Start(void)
{
    // Start PWM connected to Led Channel
    PWM_Led_Start();
}

void Led_Stop(void)
{
    // Stop PWM connected to Led Channel
    PWM_Led_Stop();
}

void Led_Update(uint16_t period, uint16_t compare)
{
    // Update the new state of blinking of the Led channel
    PWM_Led_WritePeriod(period);  // New period of the PWM
    PWM_Led_WriteCompare(compare);  // New compare value
    
}
/* [] END OF FILE */
