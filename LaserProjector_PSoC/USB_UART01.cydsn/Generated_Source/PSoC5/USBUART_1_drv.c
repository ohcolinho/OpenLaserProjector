/*******************************************************************************
* File Name: USBUART_1_drv.c
* Version 2.60
*
* Description:
*  Endpoint 0 Driver for the USBFS Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBUART_1.h"
#include "USBUART_1_pvt.h"


/***************************************
* Global data allocation
***************************************/

volatile T_USBUART_1_EP_CTL_BLOCK USBUART_1_EP[USBUART_1_MAX_EP];
volatile uint8 USBUART_1_configuration;
volatile uint8 USBUART_1_interfaceNumber;
volatile uint8 USBUART_1_configurationChanged;
volatile uint8 USBUART_1_deviceAddress;
volatile uint8 USBUART_1_deviceStatus;
volatile uint8 USBUART_1_interfaceSetting[USBUART_1_MAX_INTERFACES_NUMBER];
volatile uint8 USBUART_1_interfaceSetting_last[USBUART_1_MAX_INTERFACES_NUMBER];
volatile uint8 USBUART_1_interfaceStatus[USBUART_1_MAX_INTERFACES_NUMBER];
volatile uint8 USBUART_1_device;
const uint8 CYCODE *USBUART_1_interfaceClass;


/***************************************
* Local data allocation
***************************************/

volatile uint8 USBUART_1_ep0Toggle;
volatile uint8 USBUART_1_lastPacketSize;
volatile uint8 USBUART_1_transferState;
volatile T_USBUART_1_TD USBUART_1_currentTD;
volatile uint8 USBUART_1_ep0Mode;
volatile uint8 USBUART_1_ep0Count;
volatile uint16 USBUART_1_transferByteCount;


/*******************************************************************************
* Function Name: USBUART_1_ep_0_Interrupt
********************************************************************************
*
* Summary:
*  This Interrupt Service Routine handles Endpoint 0 (Control Pipe) traffic.
*  It dispatches setup requests and handles the data and status stages.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(USBUART_1_EP_0_ISR)
{
    uint8 bRegTemp;
    uint8 modifyReg;


    bRegTemp = CY_GET_REG8(USBUART_1_EP0_CR_PTR);
    if ((bRegTemp & USBUART_1_MODE_ACKD) != 0u)
    {
        modifyReg = 1u;
        if ((bRegTemp & USBUART_1_MODE_SETUP_RCVD) != 0u)
        {
            if((bRegTemp & USBUART_1_MODE_MASK) != USBUART_1_MODE_NAK_IN_OUT)
            {
                modifyReg = 0u;                                     /* When mode not NAK_IN_OUT => invalid setup */
            }
            else
            {
                USBUART_1_HandleSetup();
                if((USBUART_1_ep0Mode & USBUART_1_MODE_SETUP_RCVD) != 0u)
                {
                    modifyReg = 0u;                         /* if SETUP bit set -> exit without modifying the mode */
                }

            }
        }
        else if ((bRegTemp & USBUART_1_MODE_IN_RCVD) != 0u)
        {
            USBUART_1_HandleIN();
        }
        else if ((bRegTemp & USBUART_1_MODE_OUT_RCVD) != 0u)
        {
            USBUART_1_HandleOUT();
        }
        else
        {
            modifyReg = 0u;
        }
        if(modifyReg != 0u)
        {
            bRegTemp = CY_GET_REG8(USBUART_1_EP0_CR_PTR);    /* unlock registers */
            if((bRegTemp & USBUART_1_MODE_SETUP_RCVD) == 0u)  /* Check if SETUP bit is not set, otherwise exit */
            {
                /* Update the count register */
                bRegTemp = USBUART_1_ep0Toggle | USBUART_1_ep0Count;
                CY_SET_REG8(USBUART_1_EP0_CNT_PTR, bRegTemp);
                if(bRegTemp == CY_GET_REG8(USBUART_1_EP0_CNT_PTR))   /* continue if writing was successful */
                {
                    do
                    {
                        modifyReg = USBUART_1_ep0Mode;       /* Init temporary variable */
                        /* Unlock registers */
                        bRegTemp = CY_GET_REG8(USBUART_1_EP0_CR_PTR) & USBUART_1_MODE_SETUP_RCVD;
                        if(bRegTemp == 0u)                          /* Check if SETUP bit is not set */
                        {
                            /* Set the Mode Register  */
                            CY_SET_REG8(USBUART_1_EP0_CR_PTR, USBUART_1_ep0Mode);
                            /* Writing check */
                            modifyReg = CY_GET_REG8(USBUART_1_EP0_CR_PTR) & USBUART_1_MODE_MASK;
                        }
                    }while(modifyReg != USBUART_1_ep0Mode);  /* Repeat if writing was not successful */
                }
            }
        }
    }
}


/*******************************************************************************
* Function Name: USBUART_1_HandleSetup
********************************************************************************
*
* Summary:
*  This Routine dispatches requests for the four USB request types
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
void USBUART_1_HandleSetup(void) 
{
    uint8 requestHandled;

    requestHandled = CY_GET_REG8(USBUART_1_EP0_CR_PTR);      /* unlock registers */
    CY_SET_REG8(USBUART_1_EP0_CR_PTR, requestHandled);       /* clear setup bit */
    requestHandled = CY_GET_REG8(USBUART_1_EP0_CR_PTR);      /* reread register */
    if((requestHandled & USBUART_1_MODE_SETUP_RCVD) != 0u)
    {
        USBUART_1_ep0Mode = requestHandled;        /* if SETUP bit set -> exit without modifying the mode */
    }
    else
    {
        /* In case the previous transfer did not complete, close it out */
        USBUART_1_UpdateStatusBlock(USBUART_1_XFER_PREMATURE);

        switch (CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_TYPE_MASK)
        {
            case USBUART_1_RQST_TYPE_STD:
                requestHandled = USBUART_1_HandleStandardRqst();
                break;
            case USBUART_1_RQST_TYPE_CLS:
                requestHandled = USBUART_1_DispatchClassRqst();
                break;
            case USBUART_1_RQST_TYPE_VND:
                requestHandled = USBUART_1_HandleVendorRqst();
                break;
            default:
                requestHandled = USBUART_1_FALSE;
                break;
        }
        if (requestHandled == USBUART_1_FALSE)
        {
            USBUART_1_ep0Mode = USBUART_1_MODE_STALL_IN_OUT;
        }
    }
}


/*******************************************************************************
* Function Name: USBUART_1_HandleIN
********************************************************************************
*
* Summary:
*  This routine handles EP0 IN transfers.
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
void USBUART_1_HandleIN(void) 
{
    switch (USBUART_1_transferState)
    {
        case USBUART_1_TRANS_STATE_IDLE:
            break;
        case USBUART_1_TRANS_STATE_CONTROL_READ:
            USBUART_1_ControlReadDataStage();
            break;
        case USBUART_1_TRANS_STATE_CONTROL_WRITE:
            USBUART_1_ControlWriteStatusStage();
            break;
        case USBUART_1_TRANS_STATE_NO_DATA_CONTROL:
            USBUART_1_NoDataControlStatusStage();
            break;
        default:    /* there are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBUART_1_HandleOUT
********************************************************************************
*
* Summary:
*  This routine handles EP0 OUT transfers.
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
void USBUART_1_HandleOUT(void) 
{
    switch (USBUART_1_transferState)
    {
        case USBUART_1_TRANS_STATE_IDLE:
            break;
        case USBUART_1_TRANS_STATE_CONTROL_READ:
            USBUART_1_ControlReadStatusStage();
            break;
        case USBUART_1_TRANS_STATE_CONTROL_WRITE:
            USBUART_1_ControlWriteDataStage();
            break;
        case USBUART_1_TRANS_STATE_NO_DATA_CONTROL:
            /* Update the completion block */
            USBUART_1_UpdateStatusBlock(USBUART_1_XFER_ERROR);
            /* We expect no more data, so stall INs and OUTs */
            USBUART_1_ep0Mode = USBUART_1_MODE_STALL_IN_OUT;
            break;
        default:    /* There are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBUART_1_LoadEP0
********************************************************************************
*
* Summary:
*  This routine loads the EP0 data registers for OUT transfers.  It uses the
*  currentTD (previously initialized by the _InitControlWrite function and
*  updated for each OUT transfer, and the bLastPacketSize) to determine how
*  many uint8s to transfer on the current OUT.
*
*  If the number of uint8s remaining is zero and the last transfer was full,
*  we need to send a zero length packet.  Otherwise we send the minimum
*  of the control endpoint size (8) or remaining number of uint8s for the
*  transaction.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_1_transferByteCount - Update the transfer byte count from the
*     last transaction.
*  USBUART_1_ep0Count - counts the data loaded to the SIE memory in
*     current packet.
*  USBUART_1_lastPacketSize - remembers the USBFS_ep0Count value for the
*     next packet.
*  USBUART_1_transferByteCount - sum of the previous bytes transferred
*     on previous packets(sum of USBFS_lastPacketSize)
*  USBUART_1_ep0Toggle - inverted
*  USBUART_1_ep0Mode  - prepare for mode register content.
*  USBUART_1_transferState - set to TRANS_STATE_CONTROL_READ
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_LoadEP0(void) 
{
    uint8 ep0Count = 0u;

    /* Update the transfer byte count from the last transaction */
    USBUART_1_transferByteCount += USBUART_1_lastPacketSize;
    /* Now load the next transaction */
    while ((USBUART_1_currentTD.count > 0u) && (ep0Count < 8u))
    {
        CY_SET_REG8((reg8 *)(USBUART_1_EP0_DR0_IND + ep0Count), *USBUART_1_currentTD.pData);
        USBUART_1_currentTD.pData = &USBUART_1_currentTD.pData[1u];
        ep0Count++;
        USBUART_1_currentTD.count--;
    }
    /* Support zero-length packet*/
    if( (USBUART_1_lastPacketSize == 8u) || (ep0Count > 0u) )
    {
        /* Update the data toggle */
        USBUART_1_ep0Toggle ^= USBUART_1_EP0_CNT_DATA_TOGGLE;
        /* Set the Mode Register  */
        USBUART_1_ep0Mode = USBUART_1_MODE_ACK_IN_STATUS_OUT;
        /* Update the state (or stay the same) */
        USBUART_1_transferState = USBUART_1_TRANS_STATE_CONTROL_READ;
    }
    else
    {
        /* Expect Status Stage Out */
        USBUART_1_ep0Mode = USBUART_1_MODE_STATUS_OUT_ONLY;
        /* Update the state (or stay the same) */
        USBUART_1_transferState = USBUART_1_TRANS_STATE_CONTROL_READ;
    }

    /* Save the packet size for next time */
    USBUART_1_lastPacketSize = ep0Count;
    USBUART_1_ep0Count = ep0Count;
}


/*******************************************************************************
* Function Name: USBUART_1_InitControlRead
********************************************************************************
*
* Summary:
*  Initialize a control read transaction, usable to send data to the host.
*  The following global variables should be initialized before this function
*  called. To send zero length packet use InitZeroLengthControlTransfer
*  function.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USBUART_1_currentTD.count - counts of data to be sent.
*  USBUART_1_currentTD.pData - data pointer.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_1_InitControlRead(void) 
{
    uint16 xferCount;
    if(USBUART_1_currentTD.count == 0u)
    {
        (void) USBUART_1_InitZeroLengthControlTransfer();
    }
    else
    {
        /* Set up the state machine */
        USBUART_1_transferState = USBUART_1_TRANS_STATE_CONTROL_READ;
        /* Set the toggle, it gets updated in LoadEP */
        USBUART_1_ep0Toggle = 0u;
        /* Initialize the Status Block */
        USBUART_1_InitializeStatusBlock();
        xferCount = (((uint16)CY_GET_REG8(USBUART_1_lengthHi) << 8u) | (CY_GET_REG8(USBUART_1_lengthLo)));

        if (USBUART_1_currentTD.count > xferCount)
        {
            USBUART_1_currentTD.count = xferCount;
        }
        USBUART_1_LoadEP0();
    }

    return(USBUART_1_TRUE);
}


/*******************************************************************************
* Function Name: USBUART_1_InitZeroLengthControlTransfer
********************************************************************************
*
* Summary:
*  Initialize a zero length data IN transfer.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USBUART_1_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBUART_1_ep0Mode  - prepare for mode register content.
*  USBUART_1_transferState - set to TRANS_STATE_CONTROL_READ
*  USBUART_1_ep0Count - cleared, means the zero-length packet.
*  USBUART_1_lastPacketSize - cleared.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_1_InitZeroLengthControlTransfer(void)
                                                
{
    /* Update the state */
    USBUART_1_transferState = USBUART_1_TRANS_STATE_CONTROL_READ;
    /* Set the data toggle */
    USBUART_1_ep0Toggle = USBUART_1_EP0_CNT_DATA_TOGGLE;
    /* Set the Mode Register  */
    USBUART_1_ep0Mode = USBUART_1_MODE_ACK_IN_STATUS_OUT;
    /* Save the packet size for next time */
    USBUART_1_lastPacketSize = 0u;
    USBUART_1_ep0Count = 0u;

    return(USBUART_1_TRUE);
}


/*******************************************************************************
* Function Name: USBUART_1_ControlReadDataStage
********************************************************************************
*
* Summary:
*  Handle the Data Stage of a control read transfer.
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
void USBUART_1_ControlReadDataStage(void) 

{
    USBUART_1_LoadEP0();
}


/*******************************************************************************
* Function Name: USBUART_1_ControlReadStatusStage
********************************************************************************
*
* Summary:
*  Handle the Status Stage of a control read transfer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_1_USBFS_transferByteCount - updated with last packet size.
*  USBUART_1_transferState - set to TRANS_STATE_IDLE.
*  USBUART_1_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_ControlReadStatusStage(void) 
{
    /* Update the transfer byte count */
    USBUART_1_transferByteCount += USBUART_1_lastPacketSize;
    /* Go Idle */
    USBUART_1_transferState = USBUART_1_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBUART_1_UpdateStatusBlock(USBUART_1_XFER_STATUS_ACK);
    /* We expect no more data, so stall INs and OUTs */
    USBUART_1_ep0Mode =  USBUART_1_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBUART_1_InitControlWrite
********************************************************************************
*
* Summary:
*  Initialize a control write transaction
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USBUART_1_USBFS_transferState - set to TRANS_STATE_CONTROL_WRITE
*  USBUART_1_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBUART_1_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_1_InitControlWrite(void) 
{
    uint16 xferCount;

    /* Set up the state machine */
    USBUART_1_transferState = USBUART_1_TRANS_STATE_CONTROL_WRITE;
    /* This might not be necessary */
    USBUART_1_ep0Toggle = USBUART_1_EP0_CNT_DATA_TOGGLE;
    /* Initialize the Status Block */
    USBUART_1_InitializeStatusBlock();

    xferCount = (((uint16)CY_GET_REG8(USBUART_1_lengthHi) << 8u) | (CY_GET_REG8(USBUART_1_lengthLo)));

    if (USBUART_1_currentTD.count > xferCount)
    {
        USBUART_1_currentTD.count = xferCount;
    }

    /* Expect Data or Status Stage */
    USBUART_1_ep0Mode = USBUART_1_MODE_ACK_OUT_STATUS_IN;

    return(USBUART_1_TRUE);
}


/*******************************************************************************
* Function Name: USBUART_1_ControlWriteDataStage
********************************************************************************
*
* Summary:
*  Handle the Data Stage of a control write transfer
*       1. Get the data (We assume the destination was validated previously)
*       2. Update the count and data toggle
*       3. Update the mode register for the next transaction
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_1_transferByteCount - Update the transfer byte count from the
*    last transaction.
*  USBUART_1_ep0Count - counts the data loaded from the SIE memory
*    in current packet.
*  USBUART_1_transferByteCount - sum of the previous bytes transferred
*    on previous packets(sum of USBFS_lastPacketSize)
*  USBUART_1_ep0Toggle - inverted
*  USBUART_1_ep0Mode  - set to MODE_ACK_OUT_STATUS_IN.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_ControlWriteDataStage(void) 
{
    uint8 ep0Count;
    uint8 regIndex = 0u;

    ep0Count = (CY_GET_REG8(USBUART_1_EP0_CNT_PTR) & USBUART_1_EPX_CNT0_MASK) -
               USBUART_1_EPX_CNTX_CRC_COUNT;

    USBUART_1_transferByteCount += ep0Count;

    while ((USBUART_1_currentTD.count > 0u) && (ep0Count > 0u))
    {
        *USBUART_1_currentTD.pData = CY_GET_REG8((reg8 *)(USBUART_1_EP0_DR0_IND + regIndex));
        USBUART_1_currentTD.pData = &USBUART_1_currentTD.pData[1u];
        regIndex++;
        ep0Count--;
        USBUART_1_currentTD.count--;
    }
    USBUART_1_ep0Count = ep0Count;
    /* Update the data toggle */
    USBUART_1_ep0Toggle ^= USBUART_1_EP0_CNT_DATA_TOGGLE;
    /* Expect Data or Status Stage */
    USBUART_1_ep0Mode = USBUART_1_MODE_ACK_OUT_STATUS_IN;
}


/*******************************************************************************
* Function Name: USBUART_1_ControlWriteStatusStage
********************************************************************************
*
* Summary:
*  Handle the Status Stage of a control write transfer
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_1_transferState - set to TRANS_STATE_IDLE.
*  USBUART_1_USBFS_ep0Mode  - set to MODE_STALL_IN_OUT.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_ControlWriteStatusStage(void) 
{
    /* Go Idle */
    USBUART_1_transferState = USBUART_1_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBUART_1_UpdateStatusBlock(USBUART_1_XFER_STATUS_ACK);
    /* We expect no more data, so stall INs and OUTs */
    USBUART_1_ep0Mode = USBUART_1_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBUART_1_InitNoDataControlTransfer
********************************************************************************
*
* Summary:
*  Initialize a no data control transfer
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  USBUART_1_transferState - set to TRANS_STATE_NO_DATA_CONTROL.
*  USBUART_1_ep0Mode  - set to MODE_STATUS_IN_ONLY.
*  USBUART_1_ep0Count - cleared.
*  USBUART_1_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_1_InitNoDataControlTransfer(void) 
{
    USBUART_1_transferState = USBUART_1_TRANS_STATE_NO_DATA_CONTROL;
    USBUART_1_ep0Mode = USBUART_1_MODE_STATUS_IN_ONLY;
    USBUART_1_ep0Toggle = USBUART_1_EP0_CNT_DATA_TOGGLE;
    USBUART_1_ep0Count = 0u;

    return(USBUART_1_TRUE);
}


/*******************************************************************************
* Function Name: USBUART_1_NoDataControlStatusStage
********************************************************************************
* Summary:
*  Handle the Status Stage of a no data control transfer.
*
*  SET_ADDRESS is special, since we need to receive the status stage with
*  the old address.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_1_transferState - set to TRANS_STATE_IDLE.
*  USBUART_1_ep0Mode  - set to MODE_STALL_IN_OUT.
*  USBUART_1_ep0Toggle - set to EP0_CNT_DATA_TOGGLE
*  USBUART_1_deviceAddress - used to set new address and cleared
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_NoDataControlStatusStage(void) 
{
    /* Change the USB address register if we got a SET_ADDRESS. */
    if (USBUART_1_deviceAddress != 0u)
    {
        CY_SET_REG8(USBUART_1_CR0_PTR, USBUART_1_deviceAddress | USBUART_1_CR0_ENABLE);
        USBUART_1_deviceAddress = 0u;
    }
    /* Go Idle */
    USBUART_1_transferState = USBUART_1_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBUART_1_UpdateStatusBlock(USBUART_1_XFER_STATUS_ACK);
     /* We expect no more data, so stall INs and OUTs */
    USBUART_1_ep0Mode = USBUART_1_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBUART_1_UpdateStatusBlock
********************************************************************************
*
* Summary:
*  Update the Completion Status Block for a Request.  The block is updated
*  with the completion code the USBUART_1_transferByteCount.  The
*  StatusBlock Pointer is set to NULL.
*
* Parameters:
*  completionCode - status.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_1_currentTD.pStatusBlock->status - updated by the
*    completionCode parameter.
*  USBUART_1_currentTD.pStatusBlock->length - updated.
*  USBUART_1_currentTD.pStatusBlock - cleared.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_UpdateStatusBlock(uint8 completionCode) 
{
    if (USBUART_1_currentTD.pStatusBlock != NULL)
    {
        USBUART_1_currentTD.pStatusBlock->status = completionCode;
        USBUART_1_currentTD.pStatusBlock->length = USBUART_1_transferByteCount;
        USBUART_1_currentTD.pStatusBlock = NULL;
    }
}


/*******************************************************************************
* Function Name: USBUART_1_InitializeStatusBlock
********************************************************************************
*
* Summary:
*  Initialize the Completion Status Block for a Request.  The completion
*  code is set to USB_XFER_IDLE.
*
*  Also, initializes USBUART_1_transferByteCount.  Save some space,
*  this is the only consumer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBUART_1_currentTD.pStatusBlock->status - set to XFER_IDLE.
*  USBUART_1_currentTD.pStatusBlock->length - cleared.
*  USBUART_1_transferByteCount - cleared.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_1_InitializeStatusBlock(void) 
{
    USBUART_1_transferByteCount = 0u;
    if(USBUART_1_currentTD.pStatusBlock != NULL)
    {
        USBUART_1_currentTD.pStatusBlock->status = USBUART_1_XFER_IDLE;
        USBUART_1_currentTD.pStatusBlock->length = 0u;
    }
}


/* [] END OF FILE */
