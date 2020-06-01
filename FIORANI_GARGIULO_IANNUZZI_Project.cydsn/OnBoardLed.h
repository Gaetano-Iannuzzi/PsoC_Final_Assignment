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
#ifndef _ONBOARDLED_H_
    #define _ONBOARDLED_H_
    #include "project.h"
    
    /*
    // definition of a period and compare variable
    */
    typedef uint16_t period;
    typedef uint16_t compare;
    
    
    /*
     // Start of the  Led Driver
    */
    void Led_Start(void);
    
    /*
    // Stop of the Led Driver
    */
    void Led_Stop(void);
    
    /*
    // Update of the new state of blinking
    */
    void Led_Update(uint16_t period, uint16_t compare);
    
#endif
/* [] END OF FILE */
