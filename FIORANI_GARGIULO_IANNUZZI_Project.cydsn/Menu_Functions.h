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
    
    #define STOP            0x00
    #define START           0x01
    
    #define CELSIUS         0x00
    #define FAHRENHEIT      0x01
    
    #define MAX_WAITTIME    1000

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
