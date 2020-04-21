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

#define BUFF_SIZE 80
#define J1_HOME 2680000
#define TRUE 1
#define FALSE 0

//serial communication brainstorming
//messages defined as <command><joint><data><delimiter>


//all correct - all move clockwise when facing back of J3 stepper motor
#define J1_FWD 0
#define J1_REV 1
#define J2_FWD 0
#define J2_REV 1
#define J3_FWD 0
#define J3_REV 1

char limit[2];

char j2_limit;
char j3_limit;

int32 j1_pos = 0;
int32 j2_pos = 0;
int32 j3_pos = 0;

int32 j1_home = J1_HOME;

char moveJ1 = FALSE;
char moveJ2 = FALSE;
char moveJ3 = FALSE;

char home = FALSE;

char J1_ctrl_val = 0;
char J2_ctrl_val = 0;
char J3_ctrl_val = 0;

//UART stuff
uint8 Ch;
char TransmitBuffer[BUFF_SIZE];

//NCOs for joint speed control
volatile uint32 J1_tuningWord = 0;
volatile uint32 J2_tuningWord = 0;
volatile uint32 J3_tuningWord = 0;
volatile uint32 J1_accumulator = 0;
volatile uint32 J2_accumulator = 0;
volatile uint32 J3_accumulator = 0;
volatile char J1_step = 0;
volatile char J2_step = 0;
volatile char J3_step = 0;
volatile char J1_dir = 0;
volatile char J2_dir = 0;
volatile char J3_dir = 0;


volatile Message_Buff msg_buff;
volatile Move_Queue move_queue;
volatile char rx_buffer[RX_SIZE];
volatile int rx_index = 0;

void home_joints(void);
void update_positions(void);
void set_velocity(int joint, int32 tuningWord, char direction);
uint32 vel_to_tuningWord(uint32 velocity);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    SPIM_1_Start();
    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert();
    NCO_ISR_Start();
    rx_isr_Start();
    int i = 0;
    
    j2_limit = J2_LIM_Read();
    j3_limit = J3_LIM_Read();
    if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT)){
        j1_pos = ADC_DelSig_1_CountsTo_uVolts(ADC_DelSig_1_GetResult32());   
    }
    
    message_buff_init(&msg_buff);
    move_queue_init(&move_queue);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    UART_1_PutString("COM port open");
    
    for(;;)
    {
        /* Place your application code here. */
        
        //grab and interpret UART commands
        if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT)){
            j1_pos = ADC_DelSig_1_CountsTo_uVolts(ADC_DelSig_1_GetResult32());   
        }
        limit[0] = J2_LIM_Read();
        limit[1] = J3_LIM_Read();
        message_buff_execute(&msg_buff);
    }
}

void home_joints(void){
    //home joints
    J1_EN_Write(0);
    J2_EN_Write(0);
    J3_EN_Write(0);
    update_positions();
    while(j1_pos > j1_home + 100 || j1_pos < j1_home - 100){
        //home J1
        update_positions();
        if(j1_pos - j1_home > 0){
            set_velocity(1,vel_to_tuningWord(1000),J1_REV);
        }
        if(j1_pos - j1_home < 0){
            set_velocity(1,vel_to_tuningWord(1000),J1_FWD);   
        }
    }
    set_velocity(1,0,J1_FWD);
    update_positions();
    while(j2_limit != 0){
        update_positions();
        J2_DIR_Write(J2_FWD);
        J3_DIR_Write(J3_FWD);
        set_velocity(2,vel_to_tuningWord(1000),J2_FWD);
        set_velocity(3,vel_to_tuningWord(1000),J3_FWD);
    }
    set_velocity(2,vel_to_tuningWord(0),J2_FWD);
    set_velocity(3,vel_to_tuningWord(0),J3_FWD);
    update_positions();
    while(j3_limit != 0){
        update_positions();
        set_velocity(3,vel_to_tuningWord(1000),J3_REV);
    }
    set_velocity(3,vel_to_tuningWord(0),J3_REV);
    J1_EN_Write(1);
    J2_EN_Write(1);
    J3_EN_Write(1);
    return;
}

void update_positions(void){
    //joint 1, pot for position and home
    if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT)){
        j1_pos = ADC_DelSig_1_CountsTo_uVolts(ADC_DelSig_1_GetResult32());   
    }
    //joint 2, SPI for position
    j2_limit = J2_LIM_Read();
    //joint 3, SPI for position
    j3_limit = J3_LIM_Read();
}

void set_velocity(int joint, int32 tuningWord, char direction){
    //set tuning word for each joint
    switch(joint){
        case 1:
            J1_tuningWord = tuningWord;
            J1_dir = direction;
            break;
        case 2:
            J2_tuningWord = tuningWord;
            J2_dir = direction;
            break;
        case 3:
            J3_tuningWord = tuningWord;
            J3_dir = direction;
            break;
        default:
            break;
    }
}

uint32 vel_to_tuningWord(uint32 velocity){
    uint64 tw = ((uint64)velocity * 16777216) / 10000;
    return (uint32)tw;
}
/* [] END OF FILE */
