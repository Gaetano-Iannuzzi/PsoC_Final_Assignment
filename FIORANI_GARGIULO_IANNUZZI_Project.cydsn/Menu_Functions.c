

#include "Menu_Functions.h"
#include "project.h"
#include "LIS3DH.h"
uint8_t ctrl_reg10;

#define UART_PutBuffer1 UART_PutString(bufferUART)
char bufferUART[100];

CY_ISR (Custom_ISR_MENU)
{
//    if (TurnedON == 1)  ch_received = '?';
     ch_received = UART_GetChar(); // Leggo il dato ricevuto dall'UART.
    
    if (ch_received == '?')
    {
        LoopContinue = 1;
        UART_ClearRxBuffer();

        while (LoopContinue == 1)
        {
            MenuActive = 1;
            while (MenuActive == 1)
                {
                    UART_PutString("*********    MENU: CHOOSE SETTINGS    *********\r\rf. Full Scale\r\np. Sampling Freq\r\nb. Start Registration\r\ns. Stop Registration\r\n\r");
                    UART_PutString("***********************************************\r\n");
        
                    
                    while (!UART_GetRxBufferSize());
                    
                    ch_received = UART_GetChar();                 
                    
                    if (ch_received == 'f')
                    {
                        UART_PutString("*********    CHOOSE ACCELEROMETER FULL SCALE    *********\r\r1. +-2g\r\n2. +-4g\r\n3. +-8g\r\n4. +-16g\r\n5. Go back\r\n\r");
                        UART_PutString("*********************************************************\r\n");
                    
                        while(!UART_GetRxBufferSize());
                    
                        ch_received = UART_GetChar();
                        
                        UART_ClearRxBuffer();
                        
                        switch (ch_received)
                        {
                             case '1':
                                    UART_PutString("YOU CHOOSE +-2g\r\r");
                                   
                                        break;
                                case '2':
                                    UART_PutString("YOU CHOOSE +-4g\r\r");
                                    break;
                                case '3':
                                    UART_PutString("YOU CHOOSE +-8g\r\r");
                                    break;
                                case '4':
                                    UART_PutString("YOU CHOOSE +-16g\r\r");
                                    break;
                                case '5':
                                    UART_PutString("GOING BACK TO MENU\r\r");
                                    break;
                                default:
                                    UART_PutString("INVALID INPUT, GOING BACK TO MENU\r\r\r\r\r\r");
                        }
                    }
                
                    else if (ch_received == 'p')
                    {
                        UART_PutString("*********    CHOOSE ACCELEROMETER SAMPLING FREQUENCY    *********\r\r1. 1 Hz\r\n2. 10 Hz\r\n3. 25 Hz\r\n4. 50 Hz\r\n5. Go back\r");
                        UART_PutString("*****************************************************************\r\n");
                        
                        while(!UART_GetRxBufferSize());
                        
                        ch_received = UART_GetChar();
                        
                        UART_ClearRxBuffer();

                        switch (ch_received)
                        {
                            case '1':
                                UART_PutString("YOU CHOOSE 1 Hz\r\r");
                                break;
                            case '2':
                                UART_PutString("YOU CHOOSE 10 Hz\r\r");
                                    
                                /* 10Hz */
                                ctrl_reg10 = ACC_readByte(LIS3DH_CTRL_REG1);
                                if (ctrl_reg10 != LIS3DH_10HZ_CTRL_REG1) 
                                {
                                    ctrl_reg10 = LIS3DH_10HZ_CTRL_REG1;
                                    ACC_writeByte(LIS3DH_CTRL_REG1,ctrl_reg10);
                                }
                                
                                uint8_t letto = ACC_readByte(LIS3DH_CTRL_REG1);
                                sprintf(bufferUART,"YOU CHOOSE : %d \r\r",letto);
                                UART_PutBuffer1;
                                break;
                            case '3':
                                UART_PutString("YOU CHOOSE 25 Hz\r\r");
                                break;
                            case '4':
                                UART_PutString("YOU CHOOSE 50 Hz\r\r");
                                break;
                            case '5':
                                UART_PutString("GOING BACK TO MENU\r\r");
                                break;
                            default:
                                UART_PutString("INVALID INPUT, GOING BACK TO MENU\r\r\r\r\r\r");
                        }
                    }
                    else if ((ch_received == 'b') || (ch_received == 's'))
                    {
                        UART_PutString("*********    FINE ESEMPIO    *********\r");
                        UART_PutString("*****************************************************************\r\r\r\n");
                        LoopContinue = 0;
                        MenuActive = 0;
//                        if (TurnedON == 1) TurnedON = 0;
                    }
                    else
                    {
                        UART_PutString("*********    INVALID INPUT, GOING BACK TO MENU    *********\r");
                        UART_PutString("*****************************************************************\r\r\r\r\r\r\r\n");
                    }
   
                }
        }
    }
}
                
            
/* [] END OF FILE */
