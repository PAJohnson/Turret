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
#include "project.h"
#include "stdio.h"
#include "comms.h"
#include "joints.h"
#include "as5048.h"


#define TRUE 1
#define FALSE 0

//serial communication brainstorming
//messages defined as <command><joint><data><delimiter>



char moveJ1 = FALSE;
char moveJ2 = FALSE;
char moveJ3 = FALSE;

char home = FALSE;

char J1_ctrl_val = 0;
char J2_ctrl_val = 0;
char J3_ctrl_val = 0;

//UART stuff
uint8 Ch;
extern char TransmitBuffer[BUFF_SIZE];


extern volatile Message_Buff msg_buff;
extern volatile Move_Queue move_queue;
extern volatile char rx_buffer[RX_SIZE];
extern volatile int rx_index;
volatile char tick;

void home_joints(void);
void update_positions(void);


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    read_angle_command = SPI_CMD_READ | SPI_ANGLE;
    read_angle_command |= spiCalcEvenParity(read_angle_command)<<15;
    
    CS_J2_Write(1);
    CS_J3_Write(1);
    
    UART_1_Start();
    SPIM_1_Start();
    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert();
    NCO_ISR_Start();
    
    message_buff_init(&msg_buff);
    move_queue_init(&move_queue);
    joint_init(joints);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    UART_1_PutString("COM port open");
    send_cmd(SPI_REG_CLRERR|SPI_CMD_READ,0);
    send_cmd(SPI_REG_CLRERR|SPI_CMD_READ,1);
    tick_isr_Start();
    rx_isr_Start();
    for(;;)
    {
        /* Place your application code here. */
        
        //grab and interpret UART commands
        message_buff_execute(&msg_buff);
        if(tick){
            tick = 0;
            //sprintf(TransmitBuffer,"LIM1: %d, LIM2: %d, LIM3: %d\n\r",joints[0].limit,joints[1].limit,joints[2].limit);
            //UART_1_PutString(TransmitBuffer);
            //sprintf(TransmitBuffer,"J1 Pos: %ld, J2 Pos: %ld, J3 Pos: %ld\n\r",joints[0].pos-joints[0].posOffset,joints[1].pos-joints[1].posOffset,joints[2].pos-joints[2].posOffset);
            //UART_1_PutString(TransmitBuffer);
        }
    }
}

void home_joints(void){
    //home joints
    J1_EN_Write(0);
    J2_EN_Write(0);
    J3_EN_Write(0);
    while(joints[0].pos > J1_home + 100 || joints[0].pos < J1_home - 100){
        //home J1
        update_positions();
        if(joints[0].pos - J1_home > 0){
            set_velocity(1,vel_to_tuningWord(1000),J1_REV);
        }
        if(joints[0].pos - J1_home < 0){
            set_velocity(1,vel_to_tuningWord(1000),J1_FWD);   
        }
    }
    set_velocity(1,0,J1_FWD);
    update_positions();
    while(joints[1].limit != 0){
        update_positions();
        J2_DIR_Write(J2_FWD);
        J3_DIR_Write(J3_FWD);
        set_velocity(2,vel_to_tuningWord(1000),J2_FWD);
        set_velocity(3,vel_to_tuningWord(1000),J3_FWD);
    }
    set_velocity(2,vel_to_tuningWord(0),J2_FWD);
    set_velocity(3,vel_to_tuningWord(0),J3_FWD);
    update_positions();
    while(joints[2].limit != 0){
        update_positions();
        set_velocity(3,vel_to_tuningWord(1000),J3_REV);
    }
    set_velocity(3,vel_to_tuningWord(0),J3_REV);
    J1_EN_Write(1);
    J2_EN_Write(1);
    J3_EN_Write(1);
    return;
}


/* [] END OF FILE */
