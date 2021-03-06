/*******************************************************************************
* File Name: CS_J3.h  
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

#if !defined(CY_PINS_CS_J3_H) /* Pins CS_J3_H */
#define CY_PINS_CS_J3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CS_J3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CS_J3__PORT == 15 && ((CS_J3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    CS_J3_Write(uint8 value);
void    CS_J3_SetDriveMode(uint8 mode);
uint8   CS_J3_ReadDataReg(void);
uint8   CS_J3_Read(void);
void    CS_J3_SetInterruptMode(uint16 position, uint16 mode);
uint8   CS_J3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the CS_J3_SetDriveMode() function.
     *  @{
     */
        #define CS_J3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define CS_J3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define CS_J3_DM_RES_UP          PIN_DM_RES_UP
        #define CS_J3_DM_RES_DWN         PIN_DM_RES_DWN
        #define CS_J3_DM_OD_LO           PIN_DM_OD_LO
        #define CS_J3_DM_OD_HI           PIN_DM_OD_HI
        #define CS_J3_DM_STRONG          PIN_DM_STRONG
        #define CS_J3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define CS_J3_MASK               CS_J3__MASK
#define CS_J3_SHIFT              CS_J3__SHIFT
#define CS_J3_WIDTH              1u

/* Interrupt constants */
#if defined(CS_J3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in CS_J3_SetInterruptMode() function.
     *  @{
     */
        #define CS_J3_INTR_NONE      (uint16)(0x0000u)
        #define CS_J3_INTR_RISING    (uint16)(0x0001u)
        #define CS_J3_INTR_FALLING   (uint16)(0x0002u)
        #define CS_J3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define CS_J3_INTR_MASK      (0x01u) 
#endif /* (CS_J3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CS_J3_PS                     (* (reg8 *) CS_J3__PS)
/* Data Register */
#define CS_J3_DR                     (* (reg8 *) CS_J3__DR)
/* Port Number */
#define CS_J3_PRT_NUM                (* (reg8 *) CS_J3__PRT) 
/* Connect to Analog Globals */                                                  
#define CS_J3_AG                     (* (reg8 *) CS_J3__AG)                       
/* Analog MUX bux enable */
#define CS_J3_AMUX                   (* (reg8 *) CS_J3__AMUX) 
/* Bidirectional Enable */                                                        
#define CS_J3_BIE                    (* (reg8 *) CS_J3__BIE)
/* Bit-mask for Aliased Register Access */
#define CS_J3_BIT_MASK               (* (reg8 *) CS_J3__BIT_MASK)
/* Bypass Enable */
#define CS_J3_BYP                    (* (reg8 *) CS_J3__BYP)
/* Port wide control signals */                                                   
#define CS_J3_CTL                    (* (reg8 *) CS_J3__CTL)
/* Drive Modes */
#define CS_J3_DM0                    (* (reg8 *) CS_J3__DM0) 
#define CS_J3_DM1                    (* (reg8 *) CS_J3__DM1)
#define CS_J3_DM2                    (* (reg8 *) CS_J3__DM2) 
/* Input Buffer Disable Override */
#define CS_J3_INP_DIS                (* (reg8 *) CS_J3__INP_DIS)
/* LCD Common or Segment Drive */
#define CS_J3_LCD_COM_SEG            (* (reg8 *) CS_J3__LCD_COM_SEG)
/* Enable Segment LCD */
#define CS_J3_LCD_EN                 (* (reg8 *) CS_J3__LCD_EN)
/* Slew Rate Control */
#define CS_J3_SLW                    (* (reg8 *) CS_J3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CS_J3_PRTDSI__CAPS_SEL       (* (reg8 *) CS_J3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CS_J3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CS_J3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CS_J3_PRTDSI__OE_SEL0        (* (reg8 *) CS_J3__PRTDSI__OE_SEL0) 
#define CS_J3_PRTDSI__OE_SEL1        (* (reg8 *) CS_J3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CS_J3_PRTDSI__OUT_SEL0       (* (reg8 *) CS_J3__PRTDSI__OUT_SEL0) 
#define CS_J3_PRTDSI__OUT_SEL1       (* (reg8 *) CS_J3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CS_J3_PRTDSI__SYNC_OUT       (* (reg8 *) CS_J3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(CS_J3__SIO_CFG)
    #define CS_J3_SIO_HYST_EN        (* (reg8 *) CS_J3__SIO_HYST_EN)
    #define CS_J3_SIO_REG_HIFREQ     (* (reg8 *) CS_J3__SIO_REG_HIFREQ)
    #define CS_J3_SIO_CFG            (* (reg8 *) CS_J3__SIO_CFG)
    #define CS_J3_SIO_DIFF           (* (reg8 *) CS_J3__SIO_DIFF)
#endif /* (CS_J3__SIO_CFG) */

/* Interrupt Registers */
#if defined(CS_J3__INTSTAT)
    #define CS_J3_INTSTAT            (* (reg8 *) CS_J3__INTSTAT)
    #define CS_J3_SNAP               (* (reg8 *) CS_J3__SNAP)
    
	#define CS_J3_0_INTTYPE_REG 		(* (reg8 *) CS_J3__0__INTTYPE)
#endif /* (CS_J3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CS_J3_H */


/* [] END OF FILE */
