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
#ifndef __MENU_FUNCTIONS_H
    
    #define __MENU_FUNCTIONS_H
    
    #include "InterruptRoutines.h"
    #include "25LC256.h"
    #include "LIS3DH.h"
    #include "OnBoardLed.h"
    #include "project.h"
    #include "cytypes.h"
    #include "stdio.h"
    #include "FIFO_Functions.h"
    
    #define EEPROM_FREQ_CONFIG          0x0000
    #define EEPROM_FULLSCALE_CONFIG     0x0001
    #define EEPROM_START_STOP_CONFIG    0x0002
    #define EEPROM_TEMP_CONFIG          0x0003
    #define EEPROM_SENSITIVITY_VALUE    0x0004  // EEPROM address containing initial sensitivity value

    #define EEPROM_CONFIG_EXIST         0x0009
    
    #define FIRST_EEPROM_REG            0x000A
    #define MAX_PACKET_IN_EEPROM        546
    #define PACKET_SIZE                 60
    
    #define STOP            0
    #define START           1
    
    #define CELSIUS         0
    #define FAHRENHEIT      1
    
    #define MAX_WAITTIME    1000

    int ActiveRegistration;
    int ActiveVisualization;
    
    char    ch_received;
    int     LoopContinue;
    int     MenuActive;
    int     Registration_Active;
    uint8_t Sensitivity;
    uint8_t TempUnit;
    
    CY_ISR_PROTO (Custom_ISR_MENU);
    
#endif
    
/* [] END OF FILE */
