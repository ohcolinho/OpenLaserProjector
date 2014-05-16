/*******************************************************************************
* File Name: USBUART_1_audio.c
* Version 2.60
*
* Description:
*  USB AUDIO Class request handler.
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

#if defined(USBUART_1_ENABLE_AUDIO_CLASS)

#include "USBUART_1_audio.h"
#include "USBUART_1_pvt.h"
#if defined(USBUART_1_ENABLE_MIDI_STREAMING) 
    #include "USBUART_1_midi.h"
#endif /* End USBUART_1_ENABLE_MIDI_STREAMING*/


/***************************************
* Custom Declarations
***************************************/

/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


#if !defined(USER_SUPPLIED_AUDIO_HANDLER)


/***************************************
*    AUDIO Variables
***************************************/

#if defined(USBUART_1_ENABLE_AUDIO_STREAMING)
    volatile uint8 USBUART_1_currentSampleFrequency[USBUART_1_MAX_EP][USBUART_1_SAMPLE_FREQ_LEN];
    volatile uint8 USBUART_1_frequencyChanged;
    volatile uint8 USBUART_1_currentMute;
    volatile uint8 USBUART_1_currentVolume[USBUART_1_VOLUME_LEN];
    volatile uint8 USBUART_1_minimumVolume[USBUART_1_VOLUME_LEN] = {USBUART_1_VOL_MIN_LSB,
                                                                                  USBUART_1_VOL_MIN_MSB};
    volatile uint8 USBUART_1_maximumVolume[USBUART_1_VOLUME_LEN] = {USBUART_1_VOL_MAX_LSB,
                                                                                  USBUART_1_VOL_MAX_MSB};
    volatile uint8 USBUART_1_resolutionVolume[USBUART_1_VOLUME_LEN] = {USBUART_1_VOL_RES_LSB,
                                                                                     USBUART_1_VOL_RES_MSB};
#endif /* End USBUART_1_ENABLE_AUDIO_STREAMING */


/*******************************************************************************
* Function Name: USBUART_1_DispatchAUDIOClassRqst
********************************************************************************
*
* Summary:
*  This routine dispatches class requests
*
* Parameters:
*  None.
*
* Return:
*  requestHandled
*
* Global variables:
*   USBUART_1_currentSampleFrequency: Contains the current audio Sample
*       Frequency. It is set by the Host using SET_CUR request to the endpoint.
*   USBUART_1_frequencyChanged: This variable is used as a flag for the
*       user code, to be aware that Host has been sent request for changing
*       Sample Frequency. Sample frequency will be sent on the next OUT
*       transaction. It is contains endpoint address when set. The following
*       code is recommended for detecting new Sample Frequency in main code:
*       if((USBUART_1_frequencyChanged != 0) &&
*       (USBUART_1_transferState == USBUART_1_TRANS_STATE_IDLE))
*       {
*          USBUART_1_frequencyChanged = 0;
*       }
*       USBUART_1_transferState variable is checked to be sure that
*             transfer completes.
*   USBUART_1_currentMute: Contains mute configuration set by Host.
*   USBUART_1_currentVolume: Contains volume level set by Host.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_1_DispatchAUDIOClassRqst(void) 
{
    uint8 requestHandled = USBUART_1_FALSE;

    #if defined(USBUART_1_ENABLE_AUDIO_STREAMING)
        uint8 epNumber;
        epNumber = CY_GET_REG8(USBUART_1_wIndexLo) & USBUART_1_DIR_UNUSED;
    #endif /* End USBUART_1_ENABLE_AUDIO_STREAMING */

    if ((CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_DIR_MASK) == USBUART_1_RQST_DIR_D2H)
    {
        /* Control Read */
        if((CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_RCPT_MASK) == \
                                                                                    USBUART_1_RQST_RCPT_EP)
        {
            /* Endpoint */
            switch (CY_GET_REG8(USBUART_1_bRequest))
            {
                case USBUART_1_GET_CUR:
                #if defined(USBUART_1_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_SAMPLING_FREQ_CONTROL)
                    {
                         /* Endpoint Control Selector is Sampling Frequency */
                        USBUART_1_currentTD.wCount = USBUART_1_SAMPLE_FREQ_LEN;
                        USBUART_1_currentTD.pData  = USBUART_1_currentSampleFrequency[epNumber];
                        requestHandled   = USBUART_1_InitControlRead();
                    }
                #endif /* End USBUART_1_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_READ_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else if((CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_RCPT_MASK) == \
                                                                                    USBUART_1_RQST_RCPT_IFC)
        {
            /* Interface or Entity ID */
            switch (CY_GET_REG8(USBUART_1_bRequest))
            {
                case USBUART_1_GET_CUR:
                #if defined(USBUART_1_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_MUTE_CONTROL)
                    {
                        /* `#START MUTE_CONTROL_GET_REQUEST` Place multi-channel handler here */

                        /* `#END` */
                        
                         /* Entity ID Control Selector is MUTE */
                        USBUART_1_currentTD.wCount = 1u;
                        USBUART_1_currentTD.pData  = &USBUART_1_currentMute;
                        requestHandled   = USBUART_1_InitControlRead();
                    }
                    else if(CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_VOLUME_CONTROL)
                    {
                        /* `#START VOLUME_CONTROL_GET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                        /* Entity ID Control Selector is VOLUME, */
                        USBUART_1_currentTD.wCount = USBUART_1_VOLUME_LEN;
                        USBUART_1_currentTD.pData  = USBUART_1_currentVolume;
                        requestHandled   = USBUART_1_InitControlRead();
                    }
                    else
                    {
                        /* `#START OTHER_GET_CUR_REQUESTS` Place other request handler here */

                        /* `#END` */
                    }
                    break;
                case USBUART_1_GET_MIN:    /* GET_MIN */
                    if(CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_VOLUME_CONTROL)
                    {
                        /* Entity ID Control Selector is VOLUME, */
                        USBUART_1_currentTD.wCount = USBUART_1_VOLUME_LEN;
                        USBUART_1_currentTD.pData  = &USBUART_1_minimumVolume[0];
                        requestHandled   = USBUART_1_InitControlRead();
                    }
                    break;
                case USBUART_1_GET_MAX:    /* GET_MAX */
                    if(CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_VOLUME_CONTROL)
                    {
                        /* Entity ID Control Selector is VOLUME, */
                        USBUART_1_currentTD.wCount = USBUART_1_VOLUME_LEN;
                        USBUART_1_currentTD.pData  = &USBUART_1_maximumVolume[0];
                        requestHandled   = USBUART_1_InitControlRead();
                    }
                    break;
                case USBUART_1_GET_RES:    /* GET_RES */
                    if(CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_VOLUME_CONTROL)
                    {
                         /* Entity ID Control Selector is VOLUME, */
                        USBUART_1_currentTD.wCount = USBUART_1_VOLUME_LEN;
                        USBUART_1_currentTD.pData  = &USBUART_1_resolutionVolume[0];
                        requestHandled   = USBUART_1_InitControlRead();
                    }
                    break;
                /* The contents of the status message is reserved for future use.
                *  For the time being, a null packet should be returned in the data stage of the
                *  control transfer, and the received null packet should be ACKed.
                */
                case USBUART_1_GET_STAT:
                        USBUART_1_currentTD.wCount = 0u;
                        requestHandled   = USBUART_1_InitControlWrite();

                #endif /* End USBUART_1_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_WRITE_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else
        {   /* USBUART_1_RQST_RCPT_OTHER */
        }
    }
    else if ((CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_DIR_MASK) == \
                                                                                    USBUART_1_RQST_DIR_H2D)
    {
        /* Control Write */
        if((CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_RCPT_MASK) == \
                                                                                    USBUART_1_RQST_RCPT_EP)
        {
            /* Endpoint */
            switch (CY_GET_REG8(USBUART_1_bRequest))
            {
                case USBUART_1_SET_CUR:
                #if defined(USBUART_1_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_SAMPLING_FREQ_CONTROL)
                    {
                         /* Endpoint Control Selector is Sampling Frequency */
                        USBUART_1_currentTD.wCount = USBUART_1_SAMPLE_FREQ_LEN;
                        USBUART_1_currentTD.pData  = USBUART_1_currentSampleFrequency[epNumber];
                        requestHandled   = USBUART_1_InitControlWrite();
                        USBUART_1_frequencyChanged = epNumber;
                    }
                #endif /* End USBUART_1_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_SAMPLING_FREQ_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else if((CY_GET_REG8(USBUART_1_bmRequestType) & USBUART_1_RQST_RCPT_MASK) == \
                                                                                    USBUART_1_RQST_RCPT_IFC)
        {
            /* Interface or Entity ID */
            switch (CY_GET_REG8(USBUART_1_bRequest))
            {
                case USBUART_1_SET_CUR:
                #if defined(USBUART_1_ENABLE_AUDIO_STREAMING)
                    if(CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_MUTE_CONTROL)
                    {
                        /* `#START MUTE_SET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                        /* Entity ID Control Selector is MUTE */
                        USBUART_1_currentTD.wCount = 1u;
                        USBUART_1_currentTD.pData  = &USBUART_1_currentMute;
                        requestHandled   = USBUART_1_InitControlWrite();
                    }
                    else if(CY_GET_REG8(USBUART_1_wValueHi) == USBUART_1_VOLUME_CONTROL)
                    {
                        /* `#START VOLUME_CONTROL_SET_REQUEST` Place multi-channel handler here */

                        /* `#END` */

                        /* Entity ID Control Selector is VOLUME */
                        USBUART_1_currentTD.wCount = USBUART_1_VOLUME_LEN;
                        USBUART_1_currentTD.pData  = USBUART_1_currentVolume;
                        requestHandled   = USBUART_1_InitControlWrite();
                    }
                    else
                    {
                        /* `#START OTHER_SET_CUR_REQUESTS` Place other request handler here */

                        /* `#END` */
                    }
                #endif /* End USBUART_1_ENABLE_AUDIO_STREAMING */

                /* `#START AUDIO_CONTROL_SEL_REQUESTS` Place other request handler here */

                /* `#END` */
                    break;
                default:
                    break;
            }
        }
        else
        {   /* USBUART_1_RQST_RCPT_OTHER */
        }
    }
    else
    {   /* requestHandled is initialized as FALSE by default */
    }

    return(requestHandled);
}


#endif /* USER_SUPPLIED_AUDIO_HANDLER */


/*******************************************************************************
* Additional user functions supporting AUDIO Requests
********************************************************************************/

/* `#START AUDIO_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif  /* End USBUART_1_ENABLE_AUDIO_CLASS*/


/* [] END OF FILE */
