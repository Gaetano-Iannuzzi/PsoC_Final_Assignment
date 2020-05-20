

#include "Menu_Functions.h"
#include "project.h"

CY_ISR (Custom_ISR_MENU)
{
    ch_received = UART_GetChar(); // Leggo il dato ricevuto dall'UART.
    if ((ch_received == '?')||TurnedON==1)
    {
        
        LoopContinue = 1;

        while (LoopContinue == 1)
        {
            MenuActive = 1;
            while (MenuActive == 1)
                {
                    UART_PutString("*********    MENU: CHOOSE SETTINGS    *********\r\rf. Full Scale\r\np. Sampling Freq\r\nb. Start Registration\r\ns. Stop Registration\r\n\r");
                    UART_PutString("***********************************************\r\n");
        
        {
            UART_ClearRxBuffer();
            
            while (!UART_GetRxBufferSize());
            
            ch_received = UART_GetChar();
            
            
            
            if (ch_received == 'f')
            {
                    UART_PutString("*********    CHOOSE ACCELEROMETER FULL SCALE    *********\r\r1. +-2g\r\n2. +-4g\r\n3. +-8g\r\n4. +-16g\r\n\r");
                    UART_PutString("*********************************************************\r\n");
                
                    while(!UART_GetRxBufferSize());
                
                    ch_received = UART_GetChar();
                    
                    UART_ClearRxBuffer();
                    
                    switch (ch_received)
                    {
                        case '1':
                            UART_PutString("HAI SCELTO +-2g\r\r");
                            LoopContinue = 2;
                            break;
                        case '2':
                            UART_PutString("HAI SCELTO +-4g\r\r");
                            LoopContinue = 2;
                            break;
                        case '3':
                            UART_PutString("HAI SCELTO +-8g\r\r");
                            LoopContinue = 2;
                            break;
                        case '4':
                            UART_PutString("HAI SCELTO +-16g\r\r");
                            LoopContinue = 2;
                            break;
                    }
            }
            
            if (ch_received == 'p')
            {
                UART_PutString("*********    CHOOSE ACCELEROMETER SAMPLING FREQUENCY    *********\r\r1. 1 Hz\r\n2. 10 Hz\r\n3. 25 Hz\r\n4. 50 Hz\r\n\r");
                UART_PutString("*****************************************************************\r\n");
                
                while(!UART_GetRxBufferSize());
                
                ch_received = UART_GetChar();
                
                UART_ClearRxBuffer();

                switch (ch_received)
                {
                    case '1':
                        UART_PutString("HAI SCELTO 1 Hz\r\r");
                        LoopContinue = 2;
                        break;
                    case '2':
                        UART_PutString("HAI SCELTO 10 Hz\r\r");
                        LoopContinue = 2;
                        break;
                    case '3':
                        UART_PutString("HAI SCELTO 25 Hz\r\r");
                        LoopContinue = 2;
                        break;
                    case '4':
                        UART_PutString("HAI SCELTO 50 Hz\r\r");
                        LoopContinue = 2;
                        break;
                }
            }
            if ((ch_received == 'b') || (ch_received == 's'))
            {
                UART_PutString("*********    FINE ESEMPIO    *********\r");
                UART_PutString("*****************************************************************\r\r\r\n");
                LoopContinue = 0;
                MenuActive = 0;
                if (TurnedON == 1) TurnedON = 0;
            }
        }
    }
}
    }
}
                
            
/* [] END OF FILE */
