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

#include "as5048.h"

uint8 spiCalcEvenParity(uint16 value){
    uint8 cnt = 0;
    uint8 i;
    for (i = 0; i < 16; i++)
    {
        if (value & 0x1)
        {
        cnt++;
        }
    value >>= 1;
    }
    return cnt & 0x1;
}

uint16 get_angle(int encoder){
    int16 angle = 0;
    CS_J2_Write(1);
    CS_J3_Write(1);
    if(encoder == 0){
        CS_J2_Write(0);
        SPIM_1_WriteTxData(read_angle_command);
        while(!(SPIM_1_ReadTxStatus() & SPIM_1_STS_SPI_DONE)){}
        CS_J2_Write(1);
        while(SPIM_1_GetRxBufferSize() == 0){}
        angle = SPIM_1_ReadRxData();
    }
    else if(encoder == 1){
        //CS_J3_Write(0);
        //SPIM_1_WriteTxData(read_angle_command);
        //while(SPIM_1_ReadTxStatus() != SPIM_1_STS_SPI_DONE){}
        //CS_J3_Write(1);
        //while(SPIM_1_GetRxBufferSize() == 0){}
        //angle = SPIM_1_ReadRxData() & 0x3FFF;
    }
    else{
        ;
    }
    return angle;
}

uint16 send_cmd(uint16 data, char enc){
    uint16 result;
    uint16 newData;
    newData = data|spiCalcEvenParity(data)<<15;
    //send data cmd, then send nop and rx from nop
    if(enc == 0){
        CS_J2_Write(0);   
    }
    else if(enc == 1){
        CS_J3_Write(0);   
    }
    SPIM_1_WriteTxData(newData);
    while(0 == (SPIM_1_ReadTxStatus() & SPIM_1_STS_SPI_DONE)){} // wait for bytes to send
    if(enc == 0){
        CS_J2_Write(1);   
    }
    else if(enc == 1){
        CS_J3_Write(1);   
    }
    while(0 == (SPIM_1_ReadRxStatus() & SPIM_1_STS_RX_FIFO_NOT_EMPTY)){};
    result = SPIM_1_ReadRxData(); //useful data not transferred here
    if(enc == 0){
        CS_J2_Write(0);   
    }
    else if(enc == 1){
        CS_J3_Write(0);   
    }
    SPIM_1_WriteTxData(SPI_NOP|SPI_CMD_READ|spiCalcEvenParity(SPI_NOP|SPI_CMD_READ)<<15); //send dummy data
    while(0 == (SPIM_1_ReadTxStatus() & SPIM_1_STS_SPI_DONE)){} //wait for word to transmit
    if(enc == 0){
        CS_J2_Write(1);   
    }
    else if(enc == 1){
        CS_J3_Write(1);   
    }
    while(0 == (SPIM_1_ReadRxStatus() & SPIM_1_STS_RX_FIFO_NOT_EMPTY)){};
    result = SPIM_1_ReadRxData();
    return result;
}

/* [] END OF FILE */
