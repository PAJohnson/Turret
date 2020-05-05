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
        joints[i].posOffset = 0;
        joints[i].step = 0;
        joints[i].tuningWord = 0;
        joints[i].homed = 0;
    }
    //initialize angle and angleOld.
    joints[1].angle = send_cmd(SPI_ANGLE|SPI_CMD_READ,0) & 0x3FFF;
    joints[2].angle = send_cmd(SPI_ANGLE|SPI_CMD_READ,1) & 0x3FFF;
    joints[1].angleOld = joints[1].angle;
    joints[2].angleOld = joints[2].angle;
}
void joint_home(Joint volatile * joints, char joint){
    //different for joint 1 vs joints 2/3
    if(joint == 1){
        J1_EN_Write(0);
        if(joints[0].pos - J1_home > 0){
            move_queue_add(&move_queue, 1, J1_REV, vel_to_tuningWord(1000), 200000);
        }
        else if(joints[0].pos - J1_home < 0){
            move_queue_add(&move_queue, 1, J1_FWD, vel_to_tuningWord(1000), 200000);
        }
        //home joint 1 - no limit switch, just compare position with required value   
        while(joints[0].pos > J1_home + 100 || joints[0].pos < J1_home - 100){
            ;//wait
        }
        //joint homed
        joints[0].duration = 0; //kill current motion sequence
        joints[0].posOffset = joints[0].pos;
        J1_EN_Write(1);
        
    }
    else if(joint == 2){
        J2_EN_Write(0);
        J3_EN_Write(0);
        move_queue_add(&move_queue, 2, J2_FWD, vel_to_tuningWord(1000), 200000);
        move_queue_add(&move_queue, 3, J3_FWD, vel_to_tuningWord(1000), 200000);
        while(joints[1].limit != 1){
            ;//wait
        }
        joints[1].posOffset = joints[1].pos;
        joints[1].duration = 0;
        joints[2].duration = 0;
        J2_EN_Write(1);
        J3_EN_Write(1);
    }
    else if(joint == 3){
        J2_EN_Write(0);
        J3_EN_Write(0);
        move_queue_add(&move_queue, 3, J3_REV, vel_to_tuningWord(1000), 200000);
        while(joints[2].limit != 1){
            ;//wait
        }
        joints[2].posOffset = joints[2].pos;
        joints[2].duration = 0;
        J2_EN_Write(1);
        J3_EN_Write(1);
    }
    else{
        //do nothing   
    }
}

int32 joint_getPosition(char joint){
    return joints[joint-1].pos-joints[joint-1].posOffset;   
}

void set_velocity(int joint, int32 tuningWord, char direction){
    //set tuning word for each joint
    joints[joint-1].tuningWord = tuningWord;
    joints[joint-1].dir = direction;
}

uint32 vel_to_tuningWord(uint32 velocity){
    uint64 tw = ((uint64)velocity * 16777216) / 10000;
    return (uint32)tw;
}

/* [] END OF FILE */
