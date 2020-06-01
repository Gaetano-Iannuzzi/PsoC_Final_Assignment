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

#include "FIFO_Functions.h"
#include "Menu_Functions.h"

void FIFO_Enable ()    //FIFO enable function 
{
            /*Bypass Mode */                                    // Necessary to restart the FIFO accumulation
            fifo_reg = ACC_readByte(LIS3DH_FIFO_CTRL_REG);
            if( fifo_reg != LIS3DH_BYPASS_MODE_CTRL_REG) 
            {
                fifo_reg = LIS3DH_BYPASS_MODE_CTRL_REG;
                ACC_writeByte(LIS3DH_FIFO_CTRL_REG,fifo_reg);
            }
            
            /*FIFO */
            fifo_reg = ACC_readByte(LIS3DH_FIFO_CTRL_REG);
            if( fifo_reg != LIS3DH_WTM9_MODE_CTRL_REG5) 
            {
                fifo_reg = LIS3DH_WTM9_MODE_CTRL_REG5;
                ACC_writeByte(LIS3DH_FIFO_CTRL_REG,fifo_reg);
            }
}



void FIFO_Reset () //Reset the register index
{
    samples = 0x0000;
    j = 0;
    m = 0x00;
    
    if (ActiveVisualization == 1)   ActiveVisualization = 0;
    if (Registration_Active == 1)   Registration_Active = 0;
    
    EEPROM_writeByte(EEPROM_START_STOP_CONFIG, STOP);
    EEPROM_waitForWriteComplete();
    
    EEPROM_writePage(EEPROM_ADDRESS_INDEX, (uint8_t*)&samples,2);
    EEPROM_waitForWriteComplete();
    
    
    FIFO_Enable();
}
            
 /* [] END OF FILE */
