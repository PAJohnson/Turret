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

#include "joints.h"

volatile int32 J1_home = J1_HOME;

void joint_init(Joint volatile * joints){
    //set joints values to 0 initially
    int i;
    for(i = 0; i < NUM_JOINTS; i++){
        joints[i].dir = 0;
        joints[i].duration = 0;
        joints[i].limit = 0;
        joints[i].pos = 0;
        joints[i].step = 0;
        joints[i].tuningWord = 0;
    }
    //initialize angle and angleOld.
    joints[1].angle = send_cmd(SPI_ANGLE|SPI_CMD_READ,0) & 0x3FFF;
    joints[2].angle = send_cmd(SPI_ANGLE|SPI_CMD_READ,1) & 0x3FFF;
    joints[1].angleOld = joints[1].angle;
    joints[2].angleOld = joints[2].angle;
}

/* [] END OF FILE */
