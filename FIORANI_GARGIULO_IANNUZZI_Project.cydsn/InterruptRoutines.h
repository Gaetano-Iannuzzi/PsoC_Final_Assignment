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
#ifndef __INTERRUPT_ROUTINES_H
    
    #define __INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    
    CY_ISR_PROTO (Custom_ISR_ADC);
    CY_ISR_PROTO(Custom_Button_ISR);
    CY_ISR_PROTO(Custom_isr_ACC);
    
    
    volatile uint8 PacketReadyFlag,p;
    int temp;
    extern int16_t value_temp[8];
    
    extern int WTM_Full;
    
#endif

/* [] END OF FILE */
