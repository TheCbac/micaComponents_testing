/*******************************************************************************
* File Name: LEDS_Control_reg_PM.c
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

#include "LEDS_Control_reg.h"

/* Check for removal by optimization */
#if !defined(LEDS_Control_reg_Sync_ctrl_reg__REMOVED)

static LEDS_Control_reg_BACKUP_STRUCT  LEDS_Control_reg_backup = {0u};

    
/*******************************************************************************
* Function Name: LEDS_Control_reg_SaveConfig
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
void LEDS_Control_reg_SaveConfig(void) 
{
    LEDS_Control_reg_backup.controlState = LEDS_Control_reg_Control;
}


/*******************************************************************************
* Function Name: LEDS_Control_reg_RestoreConfig
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
void LEDS_Control_reg_RestoreConfig(void) 
{
     LEDS_Control_reg_Control = LEDS_Control_reg_backup.controlState;
}


/*******************************************************************************
* Function Name: LEDS_Control_reg_Sleep
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
void LEDS_Control_reg_Sleep(void) 
{
    LEDS_Control_reg_SaveConfig();
}


/*******************************************************************************
* Function Name: LEDS_Control_reg_Wakeup
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
void LEDS_Control_reg_Wakeup(void)  
{
    LEDS_Control_reg_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
