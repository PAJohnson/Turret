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
#include "comms.h"

volatile int rx_index = 0;

void message_buff_init(Message_Buff volatile * msg_buff){
    //set command to nop, joint to 0, data all to 0
    int i = 0;
    int j = 0;
    msg_buff->head = 0;
    msg_buff->tail = 0;
    msg_buff->size = 0;
    for(i = 0; i < MSG_BUFF_SIZE; i++){
        msg_buff->msgs[i].command = NOP;
        msg_buff->msgs[i].joint = 0;
        for(j = 0; j < DATA_SIZE; j++){
            msg_buff->msgs[i].data[j] = 0;
        }
    }
}

void message_buff_add(Message_Buff volatile * msg_buff, Message * msg){
    //figure this out later
    int i;
    if((msg->command == HOME_JOINT || msg->command == SET_VELOCITY || msg->command == GET_POSITION) \
        && (msg->joint == 1 || msg->joint == 2 || msg->joint == 3)){
        msg_buff->msgs[msg_buff->head].command = msg->command;
        msg_buff->msgs[msg_buff->head].joint = msg->joint;
        for(i = 0; i < DATA_SIZE; i++){
            msg_buff->msgs[msg_buff->head].data[i] = msg->data[i];
        }
        msg_buff->head = (msg_buff->head + 1) % MSG_BUFF_SIZE;
        msg_buff->size += 1;
    }
    return;
}

void message_buff_execute(Message_Buff volatile * msg_buff){
    //execute oldest message received
    uint8 joint;
    uint8 direction;
    uint32 tuningWord;
    uint32 duration;
    if(msg_buff->msgs[msg_buff->tail].command == SET_VELOCITY){
        //add message to move queue
        //move queue is global
        joint = msg_buff->msgs[msg_buff->tail].joint;
        direction = msg_buff->msgs[msg_buff->tail].data[0];
        tuningWord =    (msg_buff->msgs[msg_buff->tail].data[1]<<16) + \
                        (msg_buff->msgs[msg_buff->tail].data[2]<<8) + \
                        msg_buff->msgs[msg_buff->tail].data[3];
        duration =      (msg_buff->msgs[msg_buff->tail].data[4]<<24) + \
                        (msg_buff->msgs[msg_buff->tail].data[5]<<16) + \
                        (msg_buff->msgs[msg_buff->tail].data[6]<<8) + \
                        msg_buff->msgs[msg_buff->tail].data[7];
        move_queue_add(&move_queue,joint,direction,tuningWord,duration);
        msg_buff->tail = (msg_buff->tail + 1) % MSG_BUFF_SIZE;
        msg_buff->size -= 1;
    }
    if(msg_buff->msgs[msg_buff->tail].command == HOME_JOINT){
        joint = msg_buff->msgs[msg_buff->tail].joint;
        joint_home(joints,joint);
        msg_buff->tail = (msg_buff->tail + 1) % MSG_BUFF_SIZE;
        msg_buff->size -= 1;
    }
    if(msg_buff->msgs[msg_buff->tail].command == GET_POSITION){
        joint = msg_buff->msgs[msg_buff->tail].joint;
        sprintf(TransmitBuffer,"%ld\r\n",joint_getPosition(joint));
        UART_1_PutString(TransmitBuffer);
        msg_buff->tail = (msg_buff->tail + 1) % MSG_BUFF_SIZE;
        msg_buff->size -= 1;
    }
}

void move_queue_init(Move_Queue volatile * move_queue){
    int i;
    move_queue->head = 0;
    move_queue->tail = 0;
    move_queue->size = 0;
    for(i = 0; i < MOVE_QUEUE_SIZE; i++){
        move_queue->moves[i].direction = 0;
        move_queue->moves[i].duration = 0;
        move_queue->moves[i].joint = 0;
        move_queue->moves[i].tuningWord = 0;
    }
}
int16 move_queue_add(Move_Queue volatile * move_queue, uint8 joint, uint8 direction, uint32 tuningWord, uint32 duration){
    //return 0 if successful, 1 otherwise
    if(move_queue->size < MOVE_QUEUE_SIZE){
        move_queue->moves[move_queue->head].direction = direction;
        move_queue->moves[move_queue->head].duration = duration;
        move_queue->moves[move_queue->head].joint = joint;
        move_queue->moves[move_queue->head].tuningWord = tuningWord;
        move_queue->head = (move_queue->head + 1) % MOVE_QUEUE_SIZE;
        move_queue->size += 1;
        return 0;
    }
    else{
        return -1;
    }
}
Move volatile * move_queue_get(Move_Queue volatile * move_queue){
    //gives pointer to oldest move, increments tail index
    Move volatile * move;
    if(move_queue->size > 0){
        move_queue->size -= 1;
        move = &move_queue->moves[move_queue->tail];
        move_queue->tail = (move_queue->tail + 1) % MOVE_QUEUE_SIZE;
    }
    else{
        //move queue empty
        return 0;
    }
    return move;
}

/* [] END OF FILE */
