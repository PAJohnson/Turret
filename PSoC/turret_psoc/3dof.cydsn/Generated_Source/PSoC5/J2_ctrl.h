/*******************************************************************************
* File Name: J2_ctrl.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_J2_ctrl_H) /* CY_CONTROL_REG_J2_ctrl_H */
#define CY_CONTROL_REG_J2_ctrl_H

#include "cyfitter.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} J2_ctrl_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    J2_ctrl_Write(uint8 control) ;
uint8   J2_ctrl_Read(void) ;

void J2_ctrl_SaveConfig(void) ;
void J2_ctrl_RestoreConfig(void) ;
void J2_ctrl_Sleep(void) ; 
void J2_ctrl_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define J2_ctrl_Control        (* (reg8 *) J2_ctrl_Sync_ctrl_reg__CONTROL_REG )
#define J2_ctrl_Control_PTR    (  (reg8 *) J2_ctrl_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_J2_ctrl_H */


/* [] END OF FILE */
