/*******************************************************************************
* File Name: Write_Interrupt_PM.c
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

#include "Write_Interrupt.h"
static Write_Interrupt_backupStruct Write_Interrupt_backup;


/*******************************************************************************
* Function Name: Write_Interrupt_SaveConfig
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
*  Write_Interrupt_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Write_Interrupt_SaveConfig(void) 
{
    #if (!Write_Interrupt_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            Write_Interrupt_backup.TimerUdb = Write_Interrupt_ReadCounter();
            Write_Interrupt_backup.TimerPeriod = Write_Interrupt_ReadPeriod();
            Write_Interrupt_backup.InterruptMaskValue = Write_Interrupt_STATUS_MASK;
            #if (Write_Interrupt_UsingHWCaptureCounter)
                Write_Interrupt_backup.TimerCaptureCounter = Write_Interrupt_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Backup the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Write_Interrupt_backup.TimerUdb = Write_Interrupt_ReadCounter();
            Write_Interrupt_backup.InterruptMaskValue = Write_Interrupt_STATUS_MASK;
            #if (Write_Interrupt_UsingHWCaptureCounter)
                Write_Interrupt_backup.TimerCaptureCounter = Write_Interrupt_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Write_Interrupt_ControlRegRemoved)
            Write_Interrupt_backup.TimerControlRegister = Write_Interrupt_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Write_Interrupt_RestoreConfig
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
*  Write_Interrupt_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Write_Interrupt_RestoreConfig(void) 
{   
    #if (!Write_Interrupt_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            /* Interrupt State Backup for Critical Region*/
            uint8 Write_Interrupt_interruptState;

            Write_Interrupt_WriteCounter(Write_Interrupt_backup.TimerUdb);
            Write_Interrupt_WritePeriod(Write_Interrupt_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Write_Interrupt_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Write_Interrupt_STATUS_AUX_CTRL |= Write_Interrupt_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(Write_Interrupt_interruptState);
            Write_Interrupt_STATUS_MASK =Write_Interrupt_backup.InterruptMaskValue;
            #if (Write_Interrupt_UsingHWCaptureCounter)
                Write_Interrupt_SetCaptureCount(Write_Interrupt_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Restore the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Write_Interrupt_WriteCounter(Write_Interrupt_backup.TimerUdb);
            Write_Interrupt_STATUS_MASK =Write_Interrupt_backup.InterruptMaskValue;
            #if (Write_Interrupt_UsingHWCaptureCounter)
                Write_Interrupt_SetCaptureCount(Write_Interrupt_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Write_Interrupt_ControlRegRemoved)
            Write_Interrupt_WriteControlRegister(Write_Interrupt_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Write_Interrupt_Sleep
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
*  Write_Interrupt_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Write_Interrupt_Sleep(void) 
{
    #if(!Write_Interrupt_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Write_Interrupt_CTRL_ENABLE == (Write_Interrupt_CONTROL & Write_Interrupt_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Write_Interrupt_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Write_Interrupt_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Write_Interrupt_Stop();
    Write_Interrupt_SaveConfig();
}


/*******************************************************************************
* Function Name: Write_Interrupt_Wakeup
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
*  Write_Interrupt_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Write_Interrupt_Wakeup(void) 
{
    Write_Interrupt_RestoreConfig();
    #if(!Write_Interrupt_ControlRegRemoved)
        if(Write_Interrupt_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Write_Interrupt_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
