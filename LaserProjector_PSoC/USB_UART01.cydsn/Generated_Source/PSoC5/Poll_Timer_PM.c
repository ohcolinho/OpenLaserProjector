/*******************************************************************************
* File Name: Poll_Timer_PM.c
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

#include "Poll_Timer.h"
static Poll_Timer_backupStruct Poll_Timer_backup;


/*******************************************************************************
* Function Name: Poll_Timer_SaveConfig
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
*  Poll_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Poll_Timer_SaveConfig(void) 
{
    #if (!Poll_Timer_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            Poll_Timer_backup.TimerUdb = Poll_Timer_ReadCounter();
            Poll_Timer_backup.TimerPeriod = Poll_Timer_ReadPeriod();
            Poll_Timer_backup.InterruptMaskValue = Poll_Timer_STATUS_MASK;
            #if (Poll_Timer_UsingHWCaptureCounter)
                Poll_Timer_backup.TimerCaptureCounter = Poll_Timer_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Backup the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Poll_Timer_backup.TimerUdb = Poll_Timer_ReadCounter();
            Poll_Timer_backup.InterruptMaskValue = Poll_Timer_STATUS_MASK;
            #if (Poll_Timer_UsingHWCaptureCounter)
                Poll_Timer_backup.TimerCaptureCounter = Poll_Timer_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Poll_Timer_ControlRegRemoved)
            Poll_Timer_backup.TimerControlRegister = Poll_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Poll_Timer_RestoreConfig
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
*  Poll_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Poll_Timer_RestoreConfig(void) 
{   
    #if (!Poll_Timer_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            /* Interrupt State Backup for Critical Region*/
            uint8 Poll_Timer_interruptState;

            Poll_Timer_WriteCounter(Poll_Timer_backup.TimerUdb);
            Poll_Timer_WritePeriod(Poll_Timer_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Poll_Timer_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Poll_Timer_STATUS_AUX_CTRL |= Poll_Timer_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(Poll_Timer_interruptState);
            Poll_Timer_STATUS_MASK =Poll_Timer_backup.InterruptMaskValue;
            #if (Poll_Timer_UsingHWCaptureCounter)
                Poll_Timer_SetCaptureCount(Poll_Timer_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Restore the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Poll_Timer_WriteCounter(Poll_Timer_backup.TimerUdb);
            Poll_Timer_STATUS_MASK =Poll_Timer_backup.InterruptMaskValue;
            #if (Poll_Timer_UsingHWCaptureCounter)
                Poll_Timer_SetCaptureCount(Poll_Timer_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Poll_Timer_ControlRegRemoved)
            Poll_Timer_WriteControlRegister(Poll_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Poll_Timer_Sleep
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
*  Poll_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Poll_Timer_Sleep(void) 
{
    #if(!Poll_Timer_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Poll_Timer_CTRL_ENABLE == (Poll_Timer_CONTROL & Poll_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Poll_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Poll_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Poll_Timer_Stop();
    Poll_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Poll_Timer_Wakeup
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
*  Poll_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Poll_Timer_Wakeup(void) 
{
    Poll_Timer_RestoreConfig();
    #if(!Poll_Timer_ControlRegRemoved)
        if(Poll_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Poll_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
