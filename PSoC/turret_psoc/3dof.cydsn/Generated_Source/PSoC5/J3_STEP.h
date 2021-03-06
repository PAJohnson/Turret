/*******************************************************************************
* File Name: J3_STEP.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_J3_STEP_H) /* Pins J3_STEP_H */
#define CY_PINS_J3_STEP_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "J3_STEP_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 J3_STEP__PORT == 15 && ((J3_STEP__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    J3_STEP_Write(uint8 value);
void    J3_STEP_SetDriveMode(uint8 mode);
uint8   J3_STEP_ReadDataReg(void);
uint8   J3_STEP_Read(void);
void    J3_STEP_SetInterruptMode(uint16 position, uint16 mode);
uint8   J3_STEP_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the J3_STEP_SetDriveMode() function.
     *  @{
     */
        #define J3_STEP_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define J3_STEP_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define J3_STEP_DM_RES_UP          PIN_DM_RES_UP
        #define J3_STEP_DM_RES_DWN         PIN_DM_RES_DWN
        #define J3_STEP_DM_OD_LO           PIN_DM_OD_LO
        #define J3_STEP_DM_OD_HI           PIN_DM_OD_HI
        #define J3_STEP_DM_STRONG          PIN_DM_STRONG
        #define J3_STEP_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define J3_STEP_MASK               J3_STEP__MASK
#define J3_STEP_SHIFT              J3_STEP__SHIFT
#define J3_STEP_WIDTH              1u

/* Interrupt constants */
#if defined(J3_STEP__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in J3_STEP_SetInterruptMode() function.
     *  @{
     */
        #define J3_STEP_INTR_NONE      (uint16)(0x0000u)
        #define J3_STEP_INTR_RISING    (uint16)(0x0001u)
        #define J3_STEP_INTR_FALLING   (uint16)(0x0002u)
        #define J3_STEP_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define J3_STEP_INTR_MASK      (0x01u) 
#endif /* (J3_STEP__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define J3_STEP_PS                     (* (reg8 *) J3_STEP__PS)
/* Data Register */
#define J3_STEP_DR                     (* (reg8 *) J3_STEP__DR)
/* Port Number */
#define J3_STEP_PRT_NUM                (* (reg8 *) J3_STEP__PRT) 
/* Connect to Analog Globals */                                                  
#define J3_STEP_AG                     (* (reg8 *) J3_STEP__AG)                       
/* Analog MUX bux enable */
#define J3_STEP_AMUX                   (* (reg8 *) J3_STEP__AMUX) 
/* Bidirectional Enable */                                                        
#define J3_STEP_BIE                    (* (reg8 *) J3_STEP__BIE)
/* Bit-mask for Aliased Register Access */
#define J3_STEP_BIT_MASK               (* (reg8 *) J3_STEP__BIT_MASK)
/* Bypass Enable */
#define J3_STEP_BYP                    (* (reg8 *) J3_STEP__BYP)
/* Port wide control signals */                                                   
#define J3_STEP_CTL                    (* (reg8 *) J3_STEP__CTL)
/* Drive Modes */
#define J3_STEP_DM0                    (* (reg8 *) J3_STEP__DM0) 
#define J3_STEP_DM1                    (* (reg8 *) J3_STEP__DM1)
#define J3_STEP_DM2                    (* (reg8 *) J3_STEP__DM2) 
/* Input Buffer Disable Override */
#define J3_STEP_INP_DIS                (* (reg8 *) J3_STEP__INP_DIS)
/* LCD Common or Segment Drive */
#define J3_STEP_LCD_COM_SEG            (* (reg8 *) J3_STEP__LCD_COM_SEG)
/* Enable Segment LCD */
#define J3_STEP_LCD_EN                 (* (reg8 *) J3_STEP__LCD_EN)
/* Slew Rate Control */
#define J3_STEP_SLW                    (* (reg8 *) J3_STEP__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define J3_STEP_PRTDSI__CAPS_SEL       (* (reg8 *) J3_STEP__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define J3_STEP_PRTDSI__DBL_SYNC_IN    (* (reg8 *) J3_STEP__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define J3_STEP_PRTDSI__OE_SEL0        (* (reg8 *) J3_STEP__PRTDSI__OE_SEL0) 
#define J3_STEP_PRTDSI__OE_SEL1        (* (reg8 *) J3_STEP__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define J3_STEP_PRTDSI__OUT_SEL0       (* (reg8 *) J3_STEP__PRTDSI__OUT_SEL0) 
#define J3_STEP_PRTDSI__OUT_SEL1       (* (reg8 *) J3_STEP__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define J3_STEP_PRTDSI__SYNC_OUT       (* (reg8 *) J3_STEP__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(J3_STEP__SIO_CFG)
    #define J3_STEP_SIO_HYST_EN        (* (reg8 *) J3_STEP__SIO_HYST_EN)
    #define J3_STEP_SIO_REG_HIFREQ     (* (reg8 *) J3_STEP__SIO_REG_HIFREQ)
    #define J3_STEP_SIO_CFG            (* (reg8 *) J3_STEP__SIO_CFG)
    #define J3_STEP_SIO_DIFF           (* (reg8 *) J3_STEP__SIO_DIFF)
#endif /* (J3_STEP__SIO_CFG) */

/* Interrupt Registers */
#if defined(J3_STEP__INTSTAT)
    #define J3_STEP_INTSTAT            (* (reg8 *) J3_STEP__INTSTAT)
    #define J3_STEP_SNAP               (* (reg8 *) J3_STEP__SNAP)
    
	#define J3_STEP_0_INTTYPE_REG 		(* (reg8 *) J3_STEP__0__INTTYPE)
#endif /* (J3_STEP__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_J3_STEP_H */


/* [] END OF FILE */
