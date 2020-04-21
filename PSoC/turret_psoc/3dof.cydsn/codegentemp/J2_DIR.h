/*******************************************************************************
* File Name: J2_DIR.h  
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

#if !defined(CY_PINS_J2_DIR_H) /* Pins J2_DIR_H */
#define CY_PINS_J2_DIR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "J2_DIR_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 J2_DIR__PORT == 15 && ((J2_DIR__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    J2_DIR_Write(uint8 value);
void    J2_DIR_SetDriveMode(uint8 mode);
uint8   J2_DIR_ReadDataReg(void);
uint8   J2_DIR_Read(void);
void    J2_DIR_SetInterruptMode(uint16 position, uint16 mode);
uint8   J2_DIR_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the J2_DIR_SetDriveMode() function.
     *  @{
     */
        #define J2_DIR_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define J2_DIR_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define J2_DIR_DM_RES_UP          PIN_DM_RES_UP
        #define J2_DIR_DM_RES_DWN         PIN_DM_RES_DWN
        #define J2_DIR_DM_OD_LO           PIN_DM_OD_LO
        #define J2_DIR_DM_OD_HI           PIN_DM_OD_HI
        #define J2_DIR_DM_STRONG          PIN_DM_STRONG
        #define J2_DIR_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define J2_DIR_MASK               J2_DIR__MASK
#define J2_DIR_SHIFT              J2_DIR__SHIFT
#define J2_DIR_WIDTH              1u

/* Interrupt constants */
#if defined(J2_DIR__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in J2_DIR_SetInterruptMode() function.
     *  @{
     */
        #define J2_DIR_INTR_NONE      (uint16)(0x0000u)
        #define J2_DIR_INTR_RISING    (uint16)(0x0001u)
        #define J2_DIR_INTR_FALLING   (uint16)(0x0002u)
        #define J2_DIR_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define J2_DIR_INTR_MASK      (0x01u) 
#endif /* (J2_DIR__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define J2_DIR_PS                     (* (reg8 *) J2_DIR__PS)
/* Data Register */
#define J2_DIR_DR                     (* (reg8 *) J2_DIR__DR)
/* Port Number */
#define J2_DIR_PRT_NUM                (* (reg8 *) J2_DIR__PRT) 
/* Connect to Analog Globals */                                                  
#define J2_DIR_AG                     (* (reg8 *) J2_DIR__AG)                       
/* Analog MUX bux enable */
#define J2_DIR_AMUX                   (* (reg8 *) J2_DIR__AMUX) 
/* Bidirectional Enable */                                                        
#define J2_DIR_BIE                    (* (reg8 *) J2_DIR__BIE)
/* Bit-mask for Aliased Register Access */
#define J2_DIR_BIT_MASK               (* (reg8 *) J2_DIR__BIT_MASK)
/* Bypass Enable */
#define J2_DIR_BYP                    (* (reg8 *) J2_DIR__BYP)
/* Port wide control signals */                                                   
#define J2_DIR_CTL                    (* (reg8 *) J2_DIR__CTL)
/* Drive Modes */
#define J2_DIR_DM0                    (* (reg8 *) J2_DIR__DM0) 
#define J2_DIR_DM1                    (* (reg8 *) J2_DIR__DM1)
#define J2_DIR_DM2                    (* (reg8 *) J2_DIR__DM2) 
/* Input Buffer Disable Override */
#define J2_DIR_INP_DIS                (* (reg8 *) J2_DIR__INP_DIS)
/* LCD Common or Segment Drive */
#define J2_DIR_LCD_COM_SEG            (* (reg8 *) J2_DIR__LCD_COM_SEG)
/* Enable Segment LCD */
#define J2_DIR_LCD_EN                 (* (reg8 *) J2_DIR__LCD_EN)
/* Slew Rate Control */
#define J2_DIR_SLW                    (* (reg8 *) J2_DIR__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define J2_DIR_PRTDSI__CAPS_SEL       (* (reg8 *) J2_DIR__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define J2_DIR_PRTDSI__DBL_SYNC_IN    (* (reg8 *) J2_DIR__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define J2_DIR_PRTDSI__OE_SEL0        (* (reg8 *) J2_DIR__PRTDSI__OE_SEL0) 
#define J2_DIR_PRTDSI__OE_SEL1        (* (reg8 *) J2_DIR__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define J2_DIR_PRTDSI__OUT_SEL0       (* (reg8 *) J2_DIR__PRTDSI__OUT_SEL0) 
#define J2_DIR_PRTDSI__OUT_SEL1       (* (reg8 *) J2_DIR__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define J2_DIR_PRTDSI__SYNC_OUT       (* (reg8 *) J2_DIR__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(J2_DIR__SIO_CFG)
    #define J2_DIR_SIO_HYST_EN        (* (reg8 *) J2_DIR__SIO_HYST_EN)
    #define J2_DIR_SIO_REG_HIFREQ     (* (reg8 *) J2_DIR__SIO_REG_HIFREQ)
    #define J2_DIR_SIO_CFG            (* (reg8 *) J2_DIR__SIO_CFG)
    #define J2_DIR_SIO_DIFF           (* (reg8 *) J2_DIR__SIO_DIFF)
#endif /* (J2_DIR__SIO_CFG) */

/* Interrupt Registers */
#if defined(J2_DIR__INTSTAT)
    #define J2_DIR_INTSTAT            (* (reg8 *) J2_DIR__INTSTAT)
    #define J2_DIR_SNAP               (* (reg8 *) J2_DIR__SNAP)
    
	#define J2_DIR_0_INTTYPE_REG 		(* (reg8 *) J2_DIR__0__INTTYPE)
#endif /* (J2_DIR__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_J2_DIR_H */


/* [] END OF FILE */
