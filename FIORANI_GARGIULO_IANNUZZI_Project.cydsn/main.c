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
#include "LIS3DHRegisters.h"
#include "Menu_Functions.h"
#include "project.h"

int TurnedON;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    TurnedON = 1;
    UART_Start();
    isr_MENU_StartEx(Custom_ISR_MENU);
    Custom_ISR_MENU();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
