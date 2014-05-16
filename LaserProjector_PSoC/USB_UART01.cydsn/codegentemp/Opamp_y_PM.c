/*******************************************************************************
* File Name: Opamp_y_PM.c
* Version 1.90
*
* Description:
*  This file provides the power management source code to the API for the 
*  OpAmp (Analog Buffer) component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Opamp_y.h"

static Opamp_y_BACKUP_STRUCT  Opamp_y_backup;


/*******************************************************************************  
* Function Name: Opamp_y_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration registers.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
*******************************************************************************/
void Opamp_y_SaveConfig(void) 
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: Opamp_y_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration registers.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Opamp_y_RestoreConfig(void) 
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: Opamp_y_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves its configuration. Should be called 
*  just prior to entering sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Opamp_y_backup: The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void Opamp_y_Sleep(void) 
{
    /* Save OpAmp enable state */
    if((Opamp_y_PM_ACT_CFG_REG & Opamp_y_ACT_PWR_EN) != 0u)
    {
        /* Component is enabled */
        Opamp_y_backup.enableState = 1u;
         /* Stops the component */
         Opamp_y_Stop();
    }
    else
    {
        /* Component is disabled */
        Opamp_y_backup.enableState = 0u;
    }
    /* Saves the configuration */
    Opamp_y_SaveConfig();
}


/*******************************************************************************  
* Function Name: Opamp_y_Wakeup
********************************************************************************
*
* Summary:
*  Enables block's operation and restores its configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Opamp_y_backup: The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void Opamp_y_Wakeup(void) 
{
    /* Restore the user configuration */
    Opamp_y_RestoreConfig();

    /* Enables the component operation */
    if(Opamp_y_backup.enableState == 1u)
    {
        Opamp_y_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
