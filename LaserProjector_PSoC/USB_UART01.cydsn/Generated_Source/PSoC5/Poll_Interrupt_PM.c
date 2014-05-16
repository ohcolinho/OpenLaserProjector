/*******************************************************************************
* File Name: Poll_Interrupt_PM.c
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

#include "Poll_Interrupt.h"
static Poll_Interrupt_backupStruct Poll_Interrupt_backup;


/*******************************************************************************
* Function Name: Poll_Interrupt_SaveConfig
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
*  Poll_Interrupt_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Poll_Interrupt_SaveConfig(void) 
{
    #if (!Poll_Interrupt_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            Poll_Interrupt_backup.TimerUdb = Poll_Interrupt_ReadCounter();
            Poll_Interrupt_backup.TimerPeriod = Poll_Interrupt_ReadPeriod();
            Poll_Interrupt_backup.InterruptMaskValue = Poll_Interrupt_STATUS_MASK;
            #if (Poll_Interrupt_UsingHWCaptureCounter)
                Poll_Interrupt_backup.TimerCaptureCounter = Poll_Interrupt_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Backup the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Poll_Interrupt_backup.TimerUdb = Poll_Interrupt_ReadCounter();
            Poll_Interrupt_backup.InterruptMaskValue = Poll_Interrupt_STATUS_MASK;
            #if (Poll_Interrupt_UsingHWCaptureCounter)
                Poll_Interrupt_backup.TimerCaptureCounter = Poll_Interrupt_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Poll_Interrupt_ControlRegRemoved)
            Poll_Interrupt_backup.TimerControlRegister = Poll_Interrupt_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Poll_Interrupt_RestoreConfig
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
*  Poll_Interrupt_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Poll_Interrupt_RestoreConfig(void) 
{   
    #if (!Poll_Interrupt_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            /* Interrupt State Backup for Critical Region*/
            uint8 Poll_Interrupt_interruptState;

            Poll_Interrupt_WriteCounter(Poll_Interrupt_backup.TimerUdb);
            Poll_Interrupt_WritePeriod(Poll_Interrupt_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Poll_Interrupt_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Poll_Interrupt_STATUS_AUX_CTRL |= Poll_Interrupt_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(Poll_Interrupt_interruptState);
            Poll_Interrupt_STATUS_MASK =Poll_Interrupt_backup.InterruptMaskValue;
            #if (Poll_Interrupt_UsingHWCaptureCounter)
                Poll_Interrupt_SetCaptureCount(Poll_Interrupt_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Restore the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Poll_Interrupt_WriteCounter(Poll_Interrupt_backup.TimerUdb);
            Poll_Interrupt_STATUS_MASK =Poll_Interrupt_backup.InterruptMaskValue;
            #if (Poll_Interrupt_UsingHWCaptureCounter)
                Poll_Interrupt_SetCaptureCount(Poll_Interrupt_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Poll_Interrupt_ControlRegRemoved)
            Poll_Interrupt_WriteControlRegister(Poll_Interrupt_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Poll_Interrupt_Sleep
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
*  Poll_Interrupt_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Poll_Interrupt_Sleep(void) 
{
    #if(!Poll_Interrupt_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Poll_Interrupt_CTRL_ENABLE == (Poll_Interrupt_CONTROL & Poll_Interrupt_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Poll_Interrupt_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Poll_Interrupt_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Poll_Interrupt_Stop();
    Poll_Interrupt_SaveConfig();
}


/*******************************************************************************
* Function Name: Poll_Interrupt_Wakeup
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
*  Poll_Interrupt_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Poll_Interrupt_Wakeup(void) 
{
    Poll_Interrupt_RestoreConfig();
    #if(!Poll_Interrupt_ControlRegRemoved)
        if(Poll_Interrupt_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Poll_Interrupt_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
