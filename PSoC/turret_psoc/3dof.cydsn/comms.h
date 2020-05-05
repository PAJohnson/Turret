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

#ifndef COMMS_H
#define COMMS_H

#include "cytypes.h"
#include "stdio.h"
#include "joints.h"

#define MSG_BUFF_SIZE 100
#define RX_SIZE 80
#define DATA_SIZE 16

//communication variables and types
#define HOME_JOINT 0x484A //home joint
#define GET_POSITION 0x4750 //return joint position
#define SET_VELOCITY 0x5653 // 'VC' set velocity, direction, duration
#define NOP 0

typedef struct message{
    uint16 command;
    uint8 joint; //joints start at 1
    uint8 data[DATA_SIZE];
} Message;

typedef struct{
    Message msgs[MSG_BUFF_SIZE];
    uint16 head;
    uint16 tail;
    uint16 size;
} Message_Buff;

typedef struct{
    uint8 joint;
    uint8 direction;
    uint32 tuningWord;
    uint32 duration;
} Move;

#define MOVE_QUEUE_SIZE 100

typedef struct{
    Move moves[MOVE_QUEUE_SIZE];
    uint16 head; //index of most recent move
    uint16 tail; //index of oldest move
    uint16 size; //moves in queue
} Move_Queue;

volatile Message_Buff msg_buff;
volatile Move_Queue move_queue;
volatile char rx_buffer[RX_SIZE];
volatile int rx_index;

void message_buff_init(Message_Buff volatile * msg_buff);
void message_buff_add(Message_Buff volatile * msg_buff, Message * msg);
void message_buff_execute(Message_Buff volatile * msg_buff);

void move_queue_init(Move_Queue volatile * move_queue);
int16 move_queue_add(Move_Queue volatile * move_queue, uint8 joint, uint8 direction, uint32 tuningWord, uint32 duration); //return 0 if successful, 1 otherwise
Move volatile * move_queue_get(Move_Queue volatile * move_queue); //gives pointer to oldest move, increments tail index

#endif
/* [] END OF FILE */
