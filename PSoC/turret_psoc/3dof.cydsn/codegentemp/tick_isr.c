/*******************************************************************************
* File Name: tick_isr.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <tick_isr.h>
#include "cyapicallbacks.h"

#if !defined(tick_isr__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START tick_isr_intc` */
#include "project.h"
#include "joints.h"
extern volatile char tick;
char tick2 = 0;
/* `#END` */

#ifndef CYINT_IRQ_BASE
#define CYINT_IRQ_BASE      16
#endif /* CYINT_IRQ_BASE */
#ifndef CYINT_VECT_TABLE
#define CYINT_VECT_TABLE    ((cyisraddress **) CYREG_NVIC_VECT_OFFSET)
#endif /* CYINT_VECT_TABLE */

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: tick_isr_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it. This function disables the interrupt, 
*  sets the default interrupt vector, sets the priority from the value in the
*  Design Wide Resources Interrupt Editor, then enables the interrupt to the 
*  interrupt controller.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void tick_isr_Start(void)
{
    /* For all we know the interrupt is active. */
    tick_isr_Disable();

    /* Set the ISR to point to the tick_isr Interrupt. */
    tick_isr_SetVector(&tick_isr_Interrupt);

    /* Set the priority. */
    tick_isr_SetPriority((uint8)tick_isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    tick_isr_Enable();
}


/*******************************************************************************
* Function Name: tick_isr_StartEx
********************************************************************************
*
* Summary:
*  Sets up the interrupt and enables it. This function disables the interrupt,
*  sets the interrupt vector based on the address passed in, sets the priority 
*  from the value in the Design Wide Resources Interrupt Editor, then enables 
*  the interrupt to the interrupt controller.
*  
*  When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*  used to provide consistent definition across compilers:
*  
*  Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*   Function prototype example:
*   CY_ISR_PROTO(MyISR);
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void tick_isr_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    tick_isr_Disable();

    /* Set the ISR to point to the tick_isr Interrupt. */
    tick_isr_SetVector(address);

    /* Set the priority. */
    tick_isr_SetPriority((uint8)tick_isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    tick_isr_Enable();
}


/*******************************************************************************
* Function Name: tick_isr_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void tick_isr_Stop(void)
{
    /* Disable this interrupt. */
    tick_isr_Disable();

    /* Set the ISR to point to the passive one. */
    tick_isr_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: tick_isr_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for tick_isr.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(tick_isr_Interrupt)
{
    #ifdef tick_isr_INTERRUPT_INTERRUPT_CALLBACK
        tick_isr_Interrupt_InterruptCallback();
    #endif /* tick_isr_INTERRUPT_INTERRUPT_CALLBACK */ 

    /*  Place your Interrupt code here. */
    /* `#START tick_isr_Interrupt` */
   
    //update position values, limit switch values
    //joint 1, position and limit are special (analog pot)
    //want some lowpass filtering on this
    if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT)){
        joints[0].pos = (joints[0].pos + 15*ADC_DelSig_1_CountsTo_uVolts(ADC_DelSig_1_GetResult32()))/16;   
    }
    if(joints[0].pos < J1_HOME + 10 && joints[0].pos > J1_HOME - 10){
        joints[0].limit = 1;
    }
    else{
        joints[0].limit = 0;   
    }
    //joint 2, limit is switch and position is SPI encoder
    //need wrapping for position
    joints[1].limit = !(J2_LIM_Read());
    joints[1].angle = send_cmd(SPI_ANGLE|SPI_CMD_READ,0) & 0x3FFF;
    if(joints[1].angle - joints[1].angleOld > 10000){
        //angle decreasing, crossed over 0
        joints[1].pos = joints[1].pos + (joints[1].angle - (joints[1].angleOld + 16384));
    }
    else if(joints[1].angle - joints[1].angleOld < -10000){
        joints[1].pos = joints[1].pos + ((joints[1].angle - joints[1].angleOld) + 16384) % 16384;
    }
    else{
        joints[1].pos = joints[1].pos + joints[1].angle - joints[1].angleOld;   
    }
    joints[1].angleOld = joints[1].angle;
    
    
    
    //joint 3, limit is switch and position is SPI encoder
    joints[2].limit = !(J3_LIM_Read());
    joints[2].angle = send_cmd(SPI_ANGLE|SPI_CMD_READ,1) & 0x3FFF;
    if(joints[2].angle - joints[2].angleOld > 10000){
        //angle decreasing, crossed over 0
        joints[2].pos = joints[2].pos + (joints[2].angle - (joints[2].angleOld + 16384));
    }
    else if(joints[2].angle - joints[2].angleOld < -10000){
        joints[2].pos = joints[2].pos + ((joints[2].angle - joints[2].angleOld) + 16384) % 16384;
    }
    else{
        joints[2].pos = joints[2].pos + joints[2].angle - joints[2].angleOld;   
    }
    joints[2].angleOld = joints[2].angle;
    
    //timing for other things
    tick2 += 1;
    if(tick2 >= 10){
        tick = 1;
        tick2 = 0;
    }
    /* `#END` */
}


/*******************************************************************************
* Function Name: tick_isr_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling tick_isr_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use tick_isr_StartEx instead.
* 
*   When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*   used to provide consistent definition across compilers:
*
*   Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*
*   Function prototype example:
*     CY_ISR_PROTO(MyISR);
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void tick_isr_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)tick_isr__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: tick_isr_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress tick_isr_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)tick_isr__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: tick_isr_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. 
*
*   Note calling tick_isr_Start or tick_isr_StartEx will 
*   override any effect this API would have had. This API should only be called
*   after tick_isr_Start or tick_isr_StartEx has been called. 
*   To set the initial priority for the component, use the Design-Wide Resources
*   Interrupt Editor.
*
*   Note This API has no effect on Non-maskable interrupt NMI).
*
* Parameters:
*   priority: Priority of the interrupt, 0 being the highest priority
*             PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*             PSoC 4: Priority is from 0 to 3.
*
* Return:
*   None
*
*******************************************************************************/
void tick_isr_SetPriority(uint8 priority)
{
    *tick_isr_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: tick_isr_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt, 0 being the highest priority
*    PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*    PSoC 4: Priority is from 0 to 3.
*
*******************************************************************************/
uint8 tick_isr_GetPriority(void)
{
    uint8 priority;


    priority = *tick_isr_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: tick_isr_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt to the interrupt controller. Do not call this function
*   unless ISR_Start() has been called or the functionality of the ISR_Start() 
*   function, which sets the vector and the priority, has been called.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void tick_isr_Enable(void)
{
    /* Enable the general interrupt. */
    *tick_isr_INTC_SET_EN = tick_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: tick_isr_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 tick_isr_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*tick_isr_INTC_SET_EN & (uint32)tick_isr__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: tick_isr_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt in the interrupt controller.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void tick_isr_Disable(void)
{
    /* Disable the general interrupt. */
    *tick_isr_INTC_CLR_EN = tick_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: tick_isr_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
* Side Effects:
*   If interrupts are enabled and the interrupt is set up properly, the ISR is
*   entered (depending on the priority of this interrupt and other pending 
*   interrupts).
*
*******************************************************************************/
void tick_isr_SetPending(void)
{
    *tick_isr_INTC_SET_PD = tick_isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: tick_isr_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt in the interrupt controller.
*
*   Note Some interrupt sources are clear-on-read and require the block 
*   interrupt/status register to be read/cleared with the appropriate block API 
*   (GPIO, UART, and so on). Otherwise the ISR will continue to remain in 
*   pending state even though the interrupt itself is cleared using this API.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void tick_isr_ClearPending(void)
{
    *tick_isr_INTC_CLR_PD = tick_isr__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
