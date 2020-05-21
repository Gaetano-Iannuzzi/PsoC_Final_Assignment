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
#include "LIS3DH_Registers.h"
#include "SPI_Interface.h"
#include "25LC256.h"
#include "Menu_Functions.h"
#include "project.h"

int TurnedON;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    TurnedON = 1;
    UART_Start();
    ADC_DelSig_Start();    //Start the Delta Sigma ADC
    isr_ADC_StartEx(Custom_ISR_ADC);   //Start the ADC ISR
    isr_Button_StartEx(Custom_Button_ISR);   // Start of ISR
    isr_MENU_StartEx(Custom_ISR_MENU);
    Custom_ISR_MENU();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
   
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
