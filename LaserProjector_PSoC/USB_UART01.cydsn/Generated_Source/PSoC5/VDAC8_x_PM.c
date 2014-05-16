/*******************************************************************************
* File Name: VDAC8_x_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VDAC8_x.h"

static VDAC8_x_backupStruct VDAC8_x_backup;


/*******************************************************************************
* Function Name: VDAC8_x_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void VDAC8_x_SaveConfig(void) 
{
    if (!((VDAC8_x_CR1 & VDAC8_x_SRC_MASK) == VDAC8_x_SRC_UDB))
    {
        VDAC8_x_backup.data_value = VDAC8_x_Data;
    }
}


/*******************************************************************************
* Function Name: VDAC8_x_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void VDAC8_x_RestoreConfig(void) 
{
    if (!((VDAC8_x_CR1 & VDAC8_x_SRC_MASK) == VDAC8_x_SRC_UDB))
    {
        if((VDAC8_x_Strobe & VDAC8_x_STRB_MASK) == VDAC8_x_STRB_EN)
        {
            VDAC8_x_Strobe &= (uint8)(~VDAC8_x_STRB_MASK);
            VDAC8_x_Data = VDAC8_x_backup.data_value;
            VDAC8_x_Strobe |= VDAC8_x_STRB_EN;
        }
        else
        {
            VDAC8_x_Data = VDAC8_x_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VDAC8_x_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  VDAC8_x_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VDAC8_x_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VDAC8_x_ACT_PWR_EN == (VDAC8_x_PWRMGR & VDAC8_x_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VDAC8_x_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VDAC8_x_backup.enableState = 0u;
    }
    
    VDAC8_x_Stop();
    VDAC8_x_SaveConfig();
}


/*******************************************************************************
* Function Name: VDAC8_x_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  VDAC8_x_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VDAC8_x_Wakeup(void) 
{
    VDAC8_x_RestoreConfig();
    
    if(VDAC8_x_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VDAC8_x_Enable();

        /* Restore the data register */
        VDAC8_x_SetValue(VDAC8_x_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
