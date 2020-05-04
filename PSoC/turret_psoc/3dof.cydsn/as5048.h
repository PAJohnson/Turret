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

#ifndef AS5048_H
#define AS5048_H
#include "cytypes.h"
#include "SPIM_1.h"
#include "CS_J2.h"
#include "CS_J3.h"
    
//defines
#define SPI_CMD_READ 0x4000 /*!< flag indicating read attempt when using SPI interface */
    
#define SPI_NOP 0x0000 //no operation dummy command
#define SPI_REG_CLRERR 0x0001 /*!< clear error register when using SPI */
#define SPI_PRG_CTRL 0x0003 //programming control register
#define SPI_OTP_REG_HI 0x0016 //otp hi register
#define SPI_OTP_REG_LO 0x0017 //otp lo register
#define SPI_DIAG_AGC 0x3FFD //spi diagnostics and agc register
#define SPI_MAGNITUDE 0x3FFE //spi magnitude register
#define SPI_ANGLE 0x3FFF //spi angle register
    
uint16 read_angle_command;    

uint8 spiCalcEvenParity(uint16 value);
uint16 get_angle(int encoder);
uint16 send_cmd(uint16 data, char enc);

    
#endif

/* [] END OF FILE */
