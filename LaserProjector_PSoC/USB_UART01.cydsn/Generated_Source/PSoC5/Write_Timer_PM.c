/*******************************************************************************
* File Name: Write_Timer_PM.c
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

#include "Write_Timer.h"
static Write_Timer_backupStruct Write_Timer_backup;


/*******************************************************************************
* Function Name: Write_Timer_SaveConfig
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
*  Write_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Write_Timer_SaveConfig(void) 
{
    #if (!Write_Timer_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            Write_Timer_backup.TimerUdb = Write_Timer_ReadCounter();
            Write_Timer_backup.TimerPeriod = Write_Timer_ReadPeriod();
            Write_Timer_backup.InterruptMaskValue = Write_Timer_STATUS_MASK;
            #if (Write_Timer_UsingHWCaptureCounter)
                Write_Timer_backup.TimerCaptureCounter = Write_Timer_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Backup the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Write_Timer_backup.TimerUdb = Write_Timer_ReadCounter();
            Write_Timer_backup.InterruptMaskValue = Write_Timer_STATUS_MASK;
            #if (Write_Timer_UsingHWCaptureCounter)
                Write_Timer_backup.TimerCaptureCounter = Write_Timer_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Write_Timer_ControlRegRemoved)
            Write_Timer_backup.TimerControlRegister = Write_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Write_Timer_RestoreConfig
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
*  Write_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Write_Timer_RestoreConfig(void) 
{   
    #if (!Write_Timer_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            /* Interrupt State Backup for Critical Region*/
            uint8 Write_Timer_interruptState;

            Write_Timer_WriteCounter(Write_Timer_backup.TimerUdb);
            Write_Timer_WritePeriod(Write_Timer_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Write_Timer_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Write_Timer_STATUS_AUX_CTRL |= Write_Timer_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(Write_Timer_interruptState);
            Write_Timer_STATUS_MASK =Write_Timer_backup.InterruptMaskValue;
            #if (Write_Timer_UsingHWCaptureCounter)
                Write_Timer_SetCaptureCount(Write_Timer_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Restore the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Write_Timer_WriteCounter(Write_Timer_backup.TimerUdb);
            Write_Timer_STATUS_MASK =Write_Timer_backup.InterruptMaskValue;
            #if (Write_Timer_UsingHWCaptureCounter)
                Write_Timer_SetCaptureCount(Write_Timer_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Write_Timer_ControlRegRemoved)
            Write_Timer_WriteControlRegister(Write_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Write_Timer_Sleep
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
*  Write_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Write_Timer_Sleep(void) 
{
    #if(!Write_Timer_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Write_Timer_CTRL_ENABLE == (Write_Timer_CONTROL & Write_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Write_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Write_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Write_Timer_Stop();
    Write_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Write_Timer_Wakeup
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
*  Write_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Write_Timer_Wakeup(void) 
{
    Write_Timer_RestoreConfig();
    #if(!Write_Timer_ControlRegRemoved)
        if(Write_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Write_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
