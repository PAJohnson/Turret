/*******************************************************************************
* File Name: NCO_ISR.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_NCO_ISR_H)
#define CY_ISR_NCO_ISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void NCO_ISR_Start(void);
void NCO_ISR_StartEx(cyisraddress address);
void NCO_ISR_Stop(void);

CY_ISR_PROTO(NCO_ISR_Interrupt);

void NCO_ISR_SetVector(cyisraddress address);
cyisraddress NCO_ISR_GetVector(void);

void NCO_ISR_SetPriority(uint8 priority);
uint8 NCO_ISR_GetPriority(void);

void NCO_ISR_Enable(void);
uint8 NCO_ISR_GetState(void);
void NCO_ISR_Disable(void);

void NCO_ISR_SetPending(void);
void NCO_ISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the NCO_ISR ISR. */
#define NCO_ISR_INTC_VECTOR            ((reg32 *) NCO_ISR__INTC_VECT)

/* Address of the NCO_ISR ISR priority. */
#define NCO_ISR_INTC_PRIOR             ((reg8 *) NCO_ISR__INTC_PRIOR_REG)

/* Priority of the NCO_ISR interrupt. */
#define NCO_ISR_INTC_PRIOR_NUMBER      NCO_ISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable NCO_ISR interrupt. */
#define NCO_ISR_INTC_SET_EN            ((reg32 *) NCO_ISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the NCO_ISR interrupt. */
#define NCO_ISR_INTC_CLR_EN            ((reg32 *) NCO_ISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the NCO_ISR interrupt state to pending. */
#define NCO_ISR_INTC_SET_PD            ((reg32 *) NCO_ISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the NCO_ISR interrupt. */
#define NCO_ISR_INTC_CLR_PD            ((reg32 *) NCO_ISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_NCO_ISR_H */


/* [] END OF FILE */
