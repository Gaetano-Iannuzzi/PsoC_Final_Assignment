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
    //#include "25LC256.h"
    #include "project.h"
    #include "cytypes.h"
    #include "stdio.h"
    

    extern int TurnedON;
    
    char ch_received;
    int LoopContinue;
    int MenuActive;
    
    CY_ISR_PROTO (Custom_ISR_MENU);
    
#endif
    
/* [] END OF FILE */
