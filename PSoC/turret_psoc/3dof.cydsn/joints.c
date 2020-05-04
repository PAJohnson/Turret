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
    int i;
    for(i = 0; i < NUM_JOINTS; i++){
        joints[i].dir = 0;
        joints[i].duration = 0;
        joints[i].limit = 0;
        joints[i].pos = 0;
        joints[i].step = 0;
        joints[i].tuningWord = 0;
    }
}

/* [] END OF FILE */
