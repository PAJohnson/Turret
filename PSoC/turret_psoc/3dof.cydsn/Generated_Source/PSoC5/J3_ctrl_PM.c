/*******************************************************************************
* File Name: J3_ctrl_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "J3_ctrl.h"

/* Check for removal by optimization */
#if !defined(J3_ctrl_Sync_ctrl_reg__REMOVED)

static J3_ctrl_BACKUP_STRUCT  J3_ctrl_backup = {0u};

    
/*******************************************************************************
* Function Name: J3_ctrl_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void J3_ctrl_SaveConfig(void) 
{
    J3_ctrl_backup.controlState = J3_ctrl_Control;
}


/*******************************************************************************
* Function Name: J3_ctrl_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void J3_ctrl_RestoreConfig(void) 
{
     J3_ctrl_Control = J3_ctrl_backup.controlState;
}


/*******************************************************************************
* Function Name: J3_ctrl_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void J3_ctrl_Sleep(void) 
{
    J3_ctrl_SaveConfig();
}


/*******************************************************************************
* Function Name: J3_ctrl_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void J3_ctrl_Wakeup(void)  
{
    J3_ctrl_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
