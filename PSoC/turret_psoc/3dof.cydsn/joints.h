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

#ifndef JOINTS_H
#define JOINTS_H

#include "cytypes.h"
#include "as5048.h"

//definition for joint variables
    


#define J1_HOME 2680000
//all correct - all move clockwise when facing back of J3 stepper motor
#define J1_FWD 0
#define J1_REV 1
#define J2_FWD 0
#define J2_REV 1
#define J3_FWD 0
#define J3_REV 1
    
#define NUM_JOINTS 3

typedef struct{
    uint32 tuningWord;
    char step;
    char dir;
    int32 pos;
    uint32 duration;
    char limit;
    uint32 accumulator;
    int16 angle;
    int16 angleOld;
} Joint;

volatile int32 J1_home;

volatile Joint joints[3]; //declare array of joints

void joint_init(Joint volatile * joints);
    
#endif

/* [] END OF FILE */
