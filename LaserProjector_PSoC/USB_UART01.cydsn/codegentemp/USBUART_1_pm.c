/*******************************************************************************
* File Name: USBUART_1_pm.c
* Version 2.60
*
* Description:
*  This file provides Suspend/Resume APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "project.h"
#include "USBUART_1.h"
#include "USBUART_1_pvt.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START PM_CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
* Local data allocation
***************************************/

static USBUART_1_BACKUP_STRUCT  USBUART_1_backup;


#if(USBUART_1_DP_ISR_REMOVE == 0u)


    /*******************************************************************************
    * Function Name: USBUART_1_DP_Interrupt
    ********************************************************************************
    *
    * Summary:
    *  This Interrupt Service Routine handles DP pin changes for wake-up from
    *  the sleep mode.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USBUART_1_DP_ISR)
    {
        /* `#START DP_USER_CODE` Place your code here */

        /* `#END` */

        /* Clears active interrupt */
        CY_GET_REG8(USBUART_1_DP_INTSTAT_PTR);
    }

#endif /* (USBUART_1_DP_ISR_REMOVE == 0u) */


/*******************************************************************************
* Function Name: USBUART_1_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: USBUART_1_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_RestoreConfig(void) 
{
    if(USBUART_1_configuration != 0u)
    {
        USBUART_1_ConfigReg();
    }
}


/*******************************************************************************
* Function Name: USBUART_1_Suspend
********************************************************************************
*
* Summary:
*  This function disables the USBFS block and prepares for power donwn mode.
*
* Parameters:
*  None.
*
* Return:
*   None.
*
* Global variables:
*  USBUART_1_backup.enable:  modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_Suspend(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    if((CY_GET_REG8(USBUART_1_CR0_PTR) & USBUART_1_CR0_ENABLE) != 0u)
    {   /* USB block is enabled */
        USBUART_1_backup.enableState = 1u;

        #if(USBUART_1_EP_MM != USBUART_1__EP_MANUAL)
            USBUART_1_Stop_DMA(USBUART_1_MAX_EP);     /* Stop all DMAs */
        #endif   /* End USBUART_1_EP_MM != USBUART_1__EP_MANUAL */

        /* Ensure USB transmit enable is low (USB_USBIO_CR0.ten). - Manual Transmission - Disabled */
        USBUART_1_USBIO_CR0_REG &= (uint8)~USBUART_1_USBIO_CR0_TEN;
        CyDelayUs(0u);  /*~50ns delay */

        /* Disable the USBIO by asserting PM.USB_CR0.fsusbio_pd_n(Inverted) and pd_pullup_hv(Inverted) high. */
        USBUART_1_PM_USB_CR0_REG &=
                                (uint8)~(USBUART_1_PM_USB_CR0_PD_N | USBUART_1_PM_USB_CR0_PD_PULLUP_N);

        /* Disable the SIE */
        USBUART_1_CR0_REG &= (uint8)~USBUART_1_CR0_ENABLE;

        CyDelayUs(0u);  /*~50ns delay */
        /* Store mode and Disable VRegulator*/
        USBUART_1_backup.mode = USBUART_1_CR1_REG & USBUART_1_CR1_REG_ENABLE;
        USBUART_1_CR1_REG &= (uint8)~USBUART_1_CR1_REG_ENABLE;

        CyDelayUs(1u);  /* 0.5 us min delay */
        /* Disable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
        USBUART_1_PM_USB_CR0_REG &= (uint8)~USBUART_1_PM_USB_CR0_REF_EN;

        /* Switch DP and DM terminals to GPIO mode and disconnect 1.5k pullup*/
        USBUART_1_USBIO_CR1_REG |= USBUART_1_USBIO_CR1_IOMODE;

        /* Disable USB in ACT PM */
        USBUART_1_PM_ACT_CFG_REG &= (uint8)~USBUART_1_PM_ACT_EN_FSUSB;
        /* Disable USB block for Standby Power Mode */
        USBUART_1_PM_STBY_CFG_REG &= (uint8)~USBUART_1_PM_STBY_EN_FSUSB;
        CyDelayUs(1u); /* min  0.5us delay required */

    }
    else
    {
        USBUART_1_backup.enableState = 0u;
    }
    CyExitCriticalSection(enableInterrupts);

    /* Set the DP Interrupt for wake-up from sleep mode. */
    #if(USBUART_1_DP_ISR_REMOVE == 0u)
        (void) CyIntSetVector(USBUART_1_DP_INTC_VECT_NUM,   &USBUART_1_DP_ISR);
        CyIntSetPriority(USBUART_1_DP_INTC_VECT_NUM, USBUART_1_DP_INTC_PRIOR);
        CyIntClearPending(USBUART_1_DP_INTC_VECT_NUM);
        CyIntEnable(USBUART_1_DP_INTC_VECT_NUM);
    #endif /* (USBUART_1_DP_ISR_REMOVE == 0u) */

}


/*******************************************************************************
* Function Name: USBUART_1_Resume
********************************************************************************
*
* Summary:
*  This function enables the USBFS block after power down mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_1_backup - checked.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_Resume(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    if(USBUART_1_backup.enableState != 0u)
    {
        #if(USBUART_1_DP_ISR_REMOVE == 0u)
            CyIntDisable(USBUART_1_DP_INTC_VECT_NUM);
        #endif /* End USBUART_1_DP_ISR_REMOVE */

        /* Enable USB block */
        USBUART_1_PM_ACT_CFG_REG |= USBUART_1_PM_ACT_EN_FSUSB;
        /* Enable USB block for Standby Power Mode */
        USBUART_1_PM_STBY_CFG_REG |= USBUART_1_PM_STBY_EN_FSUSB;
        /* Enable core clock */
        USBUART_1_USB_CLK_EN_REG |= USBUART_1_USB_CLK_ENABLE;

        /* Enable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
        USBUART_1_PM_USB_CR0_REG |= USBUART_1_PM_USB_CR0_REF_EN;
        /* The reference will be available ~40us after power restored */
        CyDelayUs(40u);
        /* Return VRegulator*/
        USBUART_1_CR1_REG |= USBUART_1_backup.mode;
        CyDelayUs(0u);  /*~50ns delay */
        /* Enable USBIO */
        USBUART_1_PM_USB_CR0_REG |= USBUART_1_PM_USB_CR0_PD_N;
        CyDelayUs(2u);
        /* Set the USBIO pull-up enable */
        USBUART_1_PM_USB_CR0_REG |= USBUART_1_PM_USB_CR0_PD_PULLUP_N;

        /* Reinit Arbiter configuration for DMA transfers */
        #if(USBUART_1_EP_MM != USBUART_1__EP_MANUAL)
            /* usb arb interrupt enable */
            USBUART_1_ARB_INT_EN_REG = USBUART_1_ARB_INT_MASK;
            #if(USBUART_1_EP_MM == USBUART_1__EP_DMAMANUAL)
                USBUART_1_ARB_CFG_REG = USBUART_1_ARB_CFG_MANUAL_DMA;
            #endif   /* End USBUART_1_EP_MM == USBUART_1__EP_DMAMANUAL */
            #if(USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO)
                /*Set cfg cmplt this rises DMA request when the full configuration is done */
                USBUART_1_ARB_CFG_REG = USBUART_1_ARB_CFG_AUTO_DMA | USBUART_1_ARB_CFG_AUTO_MEM;
            #endif   /* End USBUART_1_EP_MM == USBUART_1__EP_DMAAUTO */
        #endif   /* End USBUART_1_EP_MM != USBUART_1__EP_MANUAL */

        /* STALL_IN_OUT */
        CY_SET_REG8(USBUART_1_EP0_CR_PTR, USBUART_1_MODE_STALL_IN_OUT);
        /* Enable the SIE with a last address */
        USBUART_1_CR0_REG |= USBUART_1_CR0_ENABLE;
        CyDelayCycles(1u);
        /* Finally, Enable d+ pullup and select iomode to USB mode*/
        CY_SET_REG8(USBUART_1_USBIO_CR1_PTR, USBUART_1_USBIO_CR1_USBPUEN);

        /* Restore USB register settings */
        USBUART_1_RestoreConfig();

    }
    CyExitCriticalSection(enableInterrupts);
}


/* [] END OF FILE */
