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


#include    "project.h"
#include    "Menu_Functions.h"

uint8_t     reg_menu=0;
uint8_t     Sensitivity;
int         WaitTime=0;

CY_ISR (Custom_ISR_MENU)
{
    ch_received = UART_GetChar();
    
    if (ch_received == '?')
    {
        UART_PutString("*********    HELP: POSSIBLE SETTINGS    *********\r\r");
        UART_PutString("f. Full Scale\r\np. Sampling Freq\r\nt. Set Temperature Unit\r\nv. Print Data on BCP\r\nb. Start Registration\r\ns. Stop Registration\r\n\r");        
        UART_PutString("***********************************************\r\n");
    }
    else if (ch_received == 'f' || ch_received == 'F')
    {
        UART_PutString("*********    CHOOSE ACCELEROMETER FULL SCALE    *********\r\r1. +-2g\r\n2. +-4g\r\n3. +-8g\r\n4. +-16g\r\n5. Go back\r\n\r");
        UART_PutString("*********************************************************\r\n");
    
        while(!UART_GetRxBufferSize())
        {
            CyDelay(10);
            WaitTime++;
            if (WaitTime >= MAX_WAITTIME) break;
        }
        
        if (WaitTime >= MAX_WAITTIME)   ch_received = '5';
        else                            ch_received = UART_GetChar();
        
        WaitTime = 0;
        
        UART_ClearRxBuffer();
                        
        switch (ch_received)
            {
                case '1':
                        
                    reg_menu = ACC_readByte(LIS3DH_CTRL_REG4);
                    if (reg_menu != LIS3DH_2G_CTRL_REG4) 
                    {
                        reg_menu = LIS3DH_2G_CTRL_REG4;
                        ACC_writeByte(LIS3DH_CTRL_REG4,reg_menu);
                    }
                    
                    Sensitivity = 4;
                    
                    EEPROM_writeByte(EEPROM_FULLSCALE_CONFIG, reg_menu);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writeByte(EEPROM_SENSITIVITY_VALUE, Sensitivity);
                    EEPROM_waitForWriteComplete();
                    
                    UART_PutString("YOU CHOOSE +-2g\r\r");
                    
                    FIFO_Reset ();
                    
                    break;
                
                case '2':
                    
                    reg_menu = ACC_readByte(LIS3DH_CTRL_REG4);
                    if (reg_menu != LIS3DH_4G_CTRL_REG4) 
                    {
                        reg_menu = LIS3DH_4G_CTRL_REG4;
                        ACC_writeByte(LIS3DH_CTRL_REG4,reg_menu);
                    }
                    
                    Sensitivity = 8;
                    
                    EEPROM_writeByte(EEPROM_FULLSCALE_CONFIG, reg_menu);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writeByte(EEPROM_SENSITIVITY_VALUE, Sensitivity);
                    EEPROM_waitForWriteComplete();
                    
                    UART_PutString("YOU CHOOSE +-4g\r\r");
                    
                    FIFO_Reset ();
                    
                    break;
                
                case '3':
                    
                    reg_menu = ACC_readByte(LIS3DH_CTRL_REG4);
                    if (reg_menu != LIS3DH_8G_CTRL_REG4) 
                    {
                        reg_menu = LIS3DH_8G_CTRL_REG4;
                        ACC_writeByte(LIS3DH_CTRL_REG4,reg_menu);
                    }
                    
                    Sensitivity = 16;
                    
                    EEPROM_writeByte(EEPROM_FULLSCALE_CONFIG, reg_menu);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writeByte(EEPROM_SENSITIVITY_VALUE, Sensitivity);
                    EEPROM_waitForWriteComplete();
                    
                    UART_PutString("YOU CHOOSE +-8g\r\r");
                    
                    FIFO_Reset ();
                    
                    break;
                
                case '4':
                    
                    reg_menu = ACC_readByte(LIS3DH_CTRL_REG4);
                    if (reg_menu != LIS3DH_16G_CTRL_REG4) 
                    {
                        reg_menu = LIS3DH_16G_CTRL_REG4;
                        ACC_writeByte(LIS3DH_CTRL_REG4,reg_menu);
                    }
                    
                    Sensitivity = 48;
                    
                    EEPROM_writeByte(EEPROM_FULLSCALE_CONFIG, reg_menu);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writeByte(EEPROM_SENSITIVITY_VALUE, Sensitivity);
                    EEPROM_waitForWriteComplete();
                    
                    UART_PutString("YOU CHOOSE +-16g\r\r");
                    
                    FIFO_Reset ();
                    
                    break;
                
                case '5':
                    
                    UART_PutString("EXIT....\r\r");
                    
                    break;
                
                default:
                    UART_PutString("INVALID INPUT, EXIT....\r\r\r\r\r\r");
            }
    }
    else if (ch_received == 'p' || ch_received == 'P')
    {
        UART_PutString("*********    CHOOSE ACCELEROMETER SAMPLING FREQUENCY    *********\r\r1. 1 Hz\r\n2. 10 Hz\r\n3. 25 Hz\r\n4. 50 Hz\r\n5. Go back\r");
        UART_PutString("*****************************************************************\r\n");
        
        while(!UART_GetRxBufferSize())
        {
            CyDelay(10);
            WaitTime++;
            if (WaitTime >= MAX_WAITTIME) break;
        }
        
        if (WaitTime >= MAX_WAITTIME)   ch_received = '5';
        else                            ch_received = UART_GetChar();
        
        WaitTime = 0;
        
        UART_ClearRxBuffer();

        switch (ch_received)
        {
            case '1':
            
                reg_menu = ACC_readByte(LIS3DH_CTRL_REG1);
                if (reg_menu != LIS3DH_1HZ_CTRL_REG1) 
                {
                    reg_menu = LIS3DH_1HZ_CTRL_REG1;
                    ACC_writeByte(LIS3DH_CTRL_REG1,reg_menu);
                }
                
                EEPROM_writeByte(EEPROM_FREQ_CONFIG, reg_menu);
                EEPROM_waitForWriteComplete();
                
                Timer_WritePeriod(1000);
                
                UART_PutString("YOU CHOOSE 1 Hz\r\r");
                
                FIFO_Reset ();
                
                break;
                
            case '2':
                
                reg_menu = ACC_readByte(LIS3DH_CTRL_REG1);
                if (reg_menu != LIS3DH_10HZ_CTRL_REG1) 
                {
                    reg_menu = LIS3DH_10HZ_CTRL_REG1;
                    ACC_writeByte(LIS3DH_CTRL_REG1,reg_menu);
                }
                
                EEPROM_writeByte(EEPROM_FREQ_CONFIG, reg_menu);
                EEPROM_waitForWriteComplete();
                
                Timer_WritePeriod(100);
                
                UART_PutString("YOU CHOOSE 10 Hz\r\r");
                
                FIFO_Reset ();
                
                break;
                
            case '3':
                
                reg_menu = ACC_readByte(LIS3DH_CTRL_REG1);
                if (reg_menu != LIS3DH_25HZ_CTRL_REG1) 
                {
                    reg_menu = LIS3DH_25HZ_CTRL_REG1;
                    ACC_writeByte(LIS3DH_CTRL_REG1,reg_menu);
                }
                
                EEPROM_writeByte(EEPROM_FREQ_CONFIG, reg_menu);
                EEPROM_waitForWriteComplete();
                
                UART_PutString("YOU CHOOSE 25 Hz\r\r");
                
                Timer_WritePeriod(40);
                
                FIFO_Reset ();
                
                break;
                
            case '4':
                
                reg_menu = ACC_readByte(LIS3DH_CTRL_REG1);
                if (reg_menu != LIS3DH_50HZ_CTRL_REG1) 
                {
                    reg_menu = LIS3DH_50HZ_CTRL_REG1;
                    ACC_writeByte(LIS3DH_CTRL_REG1,reg_menu);
                }
                
                EEPROM_writeByte(EEPROM_FREQ_CONFIG, reg_menu);
                EEPROM_waitForWriteComplete();
                
                UART_PutString("YOU CHOOSE 50 Hz\r\r");
                
                Timer_WritePeriod(20);

                FIFO_Reset ();
                
                break;
                                            
                case '5':
                
                UART_PutString("EXIT....\r\r");
                
                break;
            
            default:
                UART_PutString("INVALID INPUT, EXIT....\r\r\r\r\r\r");
        }
    }
    else if (ch_received == 't' || ch_received == 'T')
    {
        UART_PutString("*********    CHOOSE TEMPERATURE UNIT    *********\r\r1. Celsius\r\n2. Fahrenheit\r\n3. Go Back\r");
        UART_PutString("*****************************************************************\r\n");
        
        while(!UART_GetRxBufferSize())
        {
            CyDelay(10);
            WaitTime++;
            if (WaitTime >= MAX_WAITTIME) break;
        }
        
        if (WaitTime >= MAX_WAITTIME)   ch_received = '3';
        else                            ch_received = UART_GetChar();
        
        WaitTime = 0;
        
        UART_ClearRxBuffer();

        switch (ch_received)
        {
            case '1':
                TempUnit = CELSIUS;
                
                EEPROM_writeByte(EEPROM_TEMP_CONFIG, TempUnit);
                EEPROM_waitForWriteComplete();
                
                UART_PutString("YOU CHOOSE CELSIUS UNIT\r\r");
                
                FIFO_Reset ();
                
                break;
            
            case '2':
                TempUnit = FAHRENHEIT;
            
                EEPROM_writeByte(EEPROM_TEMP_CONFIG, TempUnit);
                EEPROM_waitForWriteComplete();
                
                UART_PutString("YOU CHOOSE FAHRENHEIT UNIT\r\r");
                
                FIFO_Reset ();
                
                break;
            
            case '3':
            
                UART_PutString("EXIT....\r\r");
                
                break;
            
            default:
                UART_PutString("INVALID INPUT, EXIT....\r\r\r\r\r\r");
        }
    }
    else if (ch_received == 'b' || ch_received == 'B')
    {
        if (Registration_Active == 1)
        {
            UART_PutString ("*********    REGISTRATION ALREADY STARTED    *********\r");
            UART_PutString("*****************************************************************\r\n");
        }
        else if (Registration_Active == 0)
        {
            Registration_Active = 1;
            Led_Update(999,499);
            UART_PutString ("*********    REGISTRATION STARTED    *********\r");
            UART_PutString("*****************************************************************\r\n");
            EEPROM_writeByte(EEPROM_START_STOP_CONFIG, START);
            EEPROM_waitForWriteComplete();
            
            Timer_Start();
            ADC_DelSig_StartConvert();
//            
        }
        
        UART_ClearRxBuffer();
    }
    else if (ch_received == 's' || ch_received == 'S')
    {
        if (Registration_Active == 0)
        {
            UART_PutString ("*********    REGISTRATION ALREADY STOPPED    *********\r");
            UART_PutString("*****************************************************************\r\n");
        }
        else if (Registration_Active == 1)
        {
            Registration_Active = 0;
            Led_Update(999,999);
            UART_PutString ("*********    REGISTRATION STOPPED    *********\r");
            UART_PutString("*****************************************************************\r\n");
           
            EEPROM_writeByte(EEPROM_START_STOP_CONFIG, STOP);
            EEPROM_waitForWriteComplete();
            
            EEPROM_writePage(EEPROM_ADDRESS_INDEX, (uint8_t*)&address_index,2);
            EEPROM_waitForWriteComplete();
            
            Timer_Stop();
            ADC_DelSig_StopConvert();
            
        }
        
        UART_ClearRxBuffer();
    }
    else if (ch_received == 'v' || ch_received == 'V')
    {
        if (Registration_Active == 0)
        {
            UART_PutString ("*********    VISUALIZATION STARTED    *********\r");
            UART_PutString("*****************************************************************\r\n");
            ActiveVisualization = 1;
            Pin_ExternalLED_Write(1);
            Timer_Stop();
            ADC_DelSig_StopConvert();
        }
        else if ( Registration_Active == 1)
        {
            Registration_Active = 0;
            UART_PutString ("*********    VISUALIZATION STARTED    *********\r");
            UART_PutString("*****************************************************************\r\n");
            ActiveVisualization = 1;
            Pin_ExternalLED_Write(1);
            Timer_Stop();
            ADC_DelSig_StopConvert();
        }
        else if (ActiveVisualization == 1)
        {
            UART_PutString ("*********    VISUALIZATION ALREADY STARTED    *********\r");
            UART_PutString("*****************************************************************\r\n");
        }
        
        UART_ClearRxBuffer();
    }
    else if (ch_received == 'u' || ch_received == 'U')
    {
        if (ActiveVisualization == 1)
        {
            ActiveVisualization = 0;
            Pin_ExternalLED_Write(0);
            UART_PutString ("*********    VISUALIZATION STOPPED    *********\r");
            UART_PutString("*****************************************************************\r\n");
        }
        else if (ActiveVisualization == 0)
        {
            UART_PutString("INVALID INPUT....\r\r\r\r\r\r");
        }
        
        UART_ClearRxBuffer();
    }
}
                
            
/* [] END OF FILE */
