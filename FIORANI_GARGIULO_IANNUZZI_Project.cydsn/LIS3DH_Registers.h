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

//Device Registers
#define LIS3DH_STATUS_REG_AUX         0x07
#define LIS3DH_OUT_ADC1_L             0x08
#define LIS3DH_OUT_ADC1_H             0x09
#define LIS3DH_OUT_ADC2_L             0x0A
#define LIS3DH_OUT_ADC2_H             0x0B
#define LIS3DH_OUT_ADC3_L             0x0C
#define LIS3DH_OUT_ADC3_H             0x0D
#define LIS3DH_WHO_AM_I               0x0F
#define LIS3DH_CTRL_REG0              0x1E
#define LIS3DH_TEMP_CFG_REG           0x1F
#define LIS3DH_CTRL_REG1              0x20
#define LIS3DH_CTRL_REG2              0x21
#define LIS3DH_CTRL_REG3              0x22
#define LIS3DH_CTRL_REG4              0x23
#define LIS3DH_CTRL_REG5              0x24
#define LIS3DH_CTRL_REG6              0x25
#define LIS3DH_REFERENCE              0x26
#define LIS3DH_STATUS_REG             0x27
#define LIS3DH_OUT_X_L                0x28
#define LIS3DH_OUT_X_H                0x29
#define LIS3DH_OUT_Y_L                0x2A
#define LIS3DH_OUT_Y_H                0x2B
#define LIS3DH_OUT_Z_L                0x2C
#define LIS3DH_OUT_Z_H                0x2D
#define LIS3DH_FIFO_CTRL_REG          0x2E
#define LIS3DH_FIFO_SRC_REG           0x2F
#define LIS3DH_INT1_CFG               0x30
#define LIS3DH_INT1_SRC               0x31
#define LIS3DH_INT1_THS               0x32
#define LIS3DH_INT1_DURATION          0x33
#define LIS3DH_INT2_CFG               0x34
#define LIS3DH_INT2_SRC               0x35
#define LIS3DH_INT2_THS               0x36
#define LIS3DH_INT2_DURATION          0x37
#define LIS3DH_CLICK_CFG              0x38
#define LIS3DH_CLICK_SRC              0x39
#define LIS3DH_CLICK_THS              0x3A
#define LIS3DH_TIME_LIMIT             0x3B
#define LIS3DH_TIME_LATENCY           0x3C
#define LIS3DH_TIME_WINDOW            0x3D
#define LIS3DH_ACT_THS                0x3E
#define LIS3DH_ACT_DUR                0x3F

//Sampling frequency NORMAL MODE
#define LIS3DH_1HZ_CTRL_REG1          0x17
#define LIS3DH_10HZ_CTRL_REG1         0x27
#define LIS3DH_25HZ_CTRL_REG1         0x37
#define LIS3DH_50HZ_CTRL_REG1         0x47

//Full scale range
//BDU=1 (output registers not updated until MSB and LSB reading)
#define LIS3DH_2G_CTRL_REG4           0x80
#define LIS3DH_4G_CTRL_REG4           0x90    
#define LIS3DH_8G_CTRL_REG4           0xA0
#define LIS3DH_16G_CTRL_REG4          0xB0

//FIFO mode
#define LIS3DH_FIFO_ENABLE_CTRL_REG5  0x40
#define LIS3DH_OVRN_FIFO               
#define LIS3DH_BYPASS_MODE_CTRL_REG   0x00
#define LIS3DH_FIFO_MODE_CTRL_REG     0x40
#define LIS3DH_WTM10_MODE_CTRL_REG5   0x4A

//Enable Interrupt on INT1
#define LIS3DH_I1_OVERRUN_CTRL_REG3   0x02
#define LIS3DH_I1_WTM_CTRL_REG3       0x04
#define LIS3DH_I1_WTM_OVR_CTRL_REG3   0x06

// Command for R/W
#define ACC_SINGLE_READ     0b10000000
#define ACC_MULTI_READ      0b11000000
#define ACC_SINGLE_WRITE    0b00000000
#define ACC_MULTI_WRITE     0b01000000

#include "SPI_Interface.h"
#define SLAVE_CS_Write CS_1_Write
#define SLAVE_CS_Write_2 CS_2_Write

#define SPIM_2           SPIM_2

uint8_t     ACC_readByte(uint8_t addr);
void        ACC_writeByte(uint8_t addr, uint8_t dataByte);
void        ACC_readMultibytes(uint8_t addr, uint8_t* data, uint8_t nBytes);
void        ACC_writeMultibytes(uint8_t addr, uint8_t* data, uint8_t nBytes);



/* [] END OF FILE */
