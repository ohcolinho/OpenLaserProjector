/*******************************************************************************
* File Name: Fire_Timer_PM.c
* Version 2.50
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Fire_Timer.h"
static Fire_Timer_backupStruct Fire_Timer_backup;


/*******************************************************************************
* Function Name: Fire_Timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Fire_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Fire_Timer_SaveConfig(void) 
{
    #if (!Fire_Timer_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            Fire_Timer_backup.TimerUdb = Fire_Timer_ReadCounter();
            Fire_Timer_backup.TimerPeriod = Fire_Timer_ReadPeriod();
            Fire_Timer_backup.InterruptMaskValue = Fire_Timer_STATUS_MASK;
            #if (Fire_Timer_UsingHWCaptureCounter)
                Fire_Timer_backup.TimerCaptureCounter = Fire_Timer_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Backup the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Fire_Timer_backup.TimerUdb = Fire_Timer_ReadCounter();
            Fire_Timer_backup.InterruptMaskValue = Fire_Timer_STATUS_MASK;
            #if (Fire_Timer_UsingHWCaptureCounter)
                Fire_Timer_backup.TimerCaptureCounter = Fire_Timer_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Fire_Timer_ControlRegRemoved)
            Fire_Timer_backup.TimerControlRegister = Fire_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Fire_Timer_RestoreConfig
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
* Global variables:
*  Fire_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Fire_Timer_RestoreConfig(void) 
{   
    #if (!Fire_Timer_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            /* Interrupt State Backup for Critical Region*/
            uint8 Fire_Timer_interruptState;

            Fire_Timer_WriteCounter(Fire_Timer_backup.TimerUdb);
            Fire_Timer_WritePeriod(Fire_Timer_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Fire_Timer_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Fire_Timer_STATUS_AUX_CTRL |= Fire_Timer_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(Fire_Timer_interruptState);
            Fire_Timer_STATUS_MASK =Fire_Timer_backup.InterruptMaskValue;
            #if (Fire_Timer_UsingHWCaptureCounter)
                Fire_Timer_SetCaptureCount(Fire_Timer_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Restore the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Fire_Timer_WriteCounter(Fire_Timer_backup.TimerUdb);
            Fire_Timer_STATUS_MASK =Fire_Timer_backup.InterruptMaskValue;
            #if (Fire_Timer_UsingHWCaptureCounter)
                Fire_Timer_SetCaptureCount(Fire_Timer_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Fire_Timer_ControlRegRemoved)
            Fire_Timer_WriteControlRegister(Fire_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Fire_Timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Fire_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Fire_Timer_Sleep(void) 
{
    #if(!Fire_Timer_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Fire_Timer_CTRL_ENABLE == (Fire_Timer_CONTROL & Fire_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Fire_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Fire_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Fire_Timer_Stop();
    Fire_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Fire_Timer_Wakeup
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
*  Fire_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Fire_Timer_Wakeup(void) 
{
    Fire_Timer_RestoreConfig();
    #if(!Fire_Timer_ControlRegRemoved)
        if(Fire_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Fire_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
