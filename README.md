
# PsoC_Final_Project_1_03
In this project, you are required to develop a program able to store accelerometer data in the external EEPROM, and stream them over UART for visualization. You can test your program by connecting the PSoC to your laptop, but if you want you can power it using the battery pack we have provided to you, so that you can freely move the accelerometer around (e.g., walking around).
The main objectives of the project are:
• use the internal FIFO of the LIS3DH to temporary store the accelerometer data
• read the data packets from the accelerometer and store them into the external EEPROM memory
• provide the end user with a menu via UART to configure some settings related to the program
• allow the end user to retrieve the data stored in the EEPROM so that they can be visualized on the Bridge Control Panel
Project requirements:
1. The LIS3DH must be configured to communicate via I2C or SPI (see Table 1)
2. At start-up time, the LIS3DH must be configured to temporary store accelerometer data in the internal FIFO and to trigger an interrupt on FIFO overrun/watermark level.
3. With your micro-controller, you need to retrieve the incoming data packets and store them in the external EEPROM memory. Each 30-bit packet, composed of X, Y, and Z axis data, must be stored in 32-bits, right-justified (thus, leaving the leftmost 2 bits empty).
4. The group must implement a technique to save, in a specific location of the EEPROM, the index at which new incoming data should be stored. For example, you may chose that, when the microcontroller is programmed, new data should be written starting from location 0x32 in the EEPROM memory data). After writing 10 packets, each one composed of 30 bits stored in 4 bytes, resulting in a total of 40 bytes, new data should be stored in location 0x5A.
5. A menu, shown to the user at start-up time through the UART interface or when the character ? is sent from a host machine, allows to configure some settings related to the program:
a. Start/stop data acquisition: when the character b is sent from the host machine, data acquisition from the accelerometer and storage on the EEPROM memory must start. When the key s is entered, data acquisition and storage must be stopped. This configuration (start/stop) must also be stored in a location of the EEPROM memory and retrieved at startup time, so that if you turn off the PSoC, the setting is maintained
b. Configuration of the accelerometer full scale: In order to enter this configuration mode, the user must enter the character f, and after entering this key, set the accelerometer full scale range. As for the start/stop acquisition, this configuration must be stored in EEPROM memory to berecovered at startup time. Furthermore, when changing the full-scale range, data in the EEPROM must be completely deleted. 
c. Configuration of the accelerometer sampling frequency. As above, this must be stored in EEPROM memory to be recovered at startup time. In addition, when changing the sampling frequency, data in the EEPROM must be completely deleted. In order to enter this configuration mode, the user must enter the character p set the sampling frequency.
6. When the device is connected to the laptop via UART, and the character v is sent from the host machine, the data from the EEPROM memory must be sent overt UART so that it can be visualized in the Bridge Control Panel. Data must be sent in integer format, keeping the first two decimals. Data should be sent in a continuous way until the character u is received from the micro-controller. For example, if 10 packets are stored in the EEPROM memory, these 10 packets should be sent continuously until the character u is received. When data is being streamed to the laptop, data acquisition from the accelerometer and data storage in the EEPROM must be stopped.
7. Since it is necessary to show a feedback to the user, the on-board LED and the external LED must be used to interact with the user. A blinking pattern with 1s period and 50% duty cycle on the on-board LED tells the user that data acquisition is ON, while a blinking pattern with a period of 250 ms with 50% duty cycle tells the user that data acquisition is ON, but EEPROM memory is full of data. When data is being streamed to the host machine, the external LED must be turned ON.
8. The on-board push button can be used to reset the system and to start writing on the EEPROM from the first location. If the push button is pressed for a time period longer than 5 seconds, the EEPROM counter should reset.
Variant1. in addition to the accelerometer data, store the temperature value read from the analog sensor (TMP36GT9Z) in the EEPROM memory, sampled with an appropriate sampling frequency and resolution. Through the UART menu the user can select to store the temperature data as either temperature values in °C, or temperature values in °F. In any case, data should be stored in integer format keeping two decimals. In this mode, the temperature data must be stored after every packet of accelerometer data, in two bytes. By entering the character t,the user can enter this configuration mode, and choose how to store the data.
