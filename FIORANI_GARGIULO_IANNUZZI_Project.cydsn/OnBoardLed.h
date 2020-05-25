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
#ifndef _ONBOARDLED_H_
    #define _ONBOARDLED_H_
    #include "project.h"
    
//    const int Period_ACQUISITION_ON = 999;   // 1s.
//    const int Compare_ACQUISITION_ON= 499;  //0.5s.
//    const int Period_EEPROM_FULL = 249;   //0.25s.
//    const int Compare_EEPROM_FULL =124;  //0.125s.
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
