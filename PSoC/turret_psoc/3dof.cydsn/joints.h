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
} Joint;

//NCOs for joint speed control
volatile uint32 J1_tuningWord;
volatile uint32 J2_tuningWord;
volatile uint32 J3_tuningWord;
volatile uint32 J1_accumulator;
volatile uint32 J2_accumulator;
volatile uint32 J3_accumulator;
volatile char J1_step;
volatile char J2_step;
volatile char J3_step;
volatile char J1_dir;
volatile char J2_dir;
volatile char J3_dir;

volatile int32 J1_pos;
volatile int32 J2_pos;
volatile int32 J3_pos;
    
volatile uint32 J1_duration;
volatile uint32 J2_duration;
volatile uint32 J3_duration;

volatile int32 J1_home;

volatile char J1_limit;
volatile char J2_limit;
volatile char J3_limit;

volatile Joint joints[3]; //declare array of joints

void joint_init(Joint volatile * joints);
    
#endif

/* [] END OF FILE */
