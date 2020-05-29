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
#ifndef __FIFO_FUNCTIONS_H
    
    #define __FIFO_FUNCTIONS_H
    
    #include "InterruptRoutines.h"
    #include "25LC256.h"
    #include "LIS3DH.h"
    #include "OnBoardLed.h"
    #include "project.h"
    #include "cytypes.h"
    #include "stdio.h"
    
    uint8_t     fifo_reg;
    uint8_t     ctrl_reg5;
    uint16_t    samples;
    uint16_t    m;
    int16_t     j;

    void FIFO_Enable();
    void FIFO_Reset ();
    
#endif

/* [] END OF FILE */
