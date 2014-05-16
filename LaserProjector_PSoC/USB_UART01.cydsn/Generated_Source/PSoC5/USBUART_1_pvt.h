/*******************************************************************************
* File Name: .h
* Version 2.60
*
* Description:
*  This private file provides constants and parameter values for the
*  USBFS Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_USBFS_USBUART_1_pvt_H)
#define CY_USBFS_USBUART_1_pvt_H


/***************************************
*     Private Variables
***************************************/

/* Generated external references for descriptors*/
extern const uint8 CYCODE USBUART_1_DEVICE0_DESCR[18u];
extern const uint8 CYCODE USBUART_1_DEVICE0_CONFIGURATION0_DESCR[67u];
extern const T_USBUART_1_EP_SETTINGS_BLOCK CYCODE USBUART_1_DEVICE0_CONFIGURATION0_EP_SETTINGS_TABLE[3u];
extern const uint8 CYCODE USBUART_1_DEVICE0_CONFIGURATION0_INTERFACE_CLASS[2u];
extern const T_USBUART_1_LUT CYCODE USBUART_1_DEVICE0_CONFIGURATION0_TABLE[5u];
extern const T_USBUART_1_LUT CYCODE USBUART_1_DEVICE0_TABLE[2u];
extern const T_USBUART_1_LUT CYCODE USBUART_1_TABLE[1u];
extern const uint8 CYCODE USBUART_1_SN_STRING_DESCRIPTOR[2];
extern const uint8 CYCODE USBUART_1_STRING_DESCRIPTORS[159u];
extern const uint8 CYCODE USBUART_1_HIDREPORT_DESCRIPTOR1[54u];


extern const uint8 CYCODE USBUART_1_MSOS_DESCRIPTOR[USBUART_1_MSOS_DESCRIPTOR_LENGTH];
extern const uint8 CYCODE USBUART_1_MSOS_CONFIGURATION_DESCR[USBUART_1_MSOS_CONF_DESCR_LENGTH];
#if defined(USBUART_1_ENABLE_IDSN_STRING)
    extern uint8 USBUART_1_idSerialNumberStringDescriptor[USBUART_1_IDSN_DESCR_LENGTH];
#endif /* USBUART_1_ENABLE_IDSN_STRING */

extern volatile uint8 USBUART_1_interfaceNumber;
extern volatile uint8 USBUART_1_interfaceSetting[USBUART_1_MAX_INTERFACES_NUMBER];
extern volatile uint8 USBUART_1_interfaceSetting_last[USBUART_1_MAX_INTERFACES_NUMBER];
extern volatile uint8 USBUART_1_deviceAddress;
extern volatile uint8 USBUART_1_interfaceStatus[USBUART_1_MAX_INTERFACES_NUMBER];
extern const uint8 CYCODE *USBUART_1_interfaceClass;

extern volatile T_USBUART_1_EP_CTL_BLOCK USBUART_1_EP[USBUART_1_MAX_EP];
extern volatile T_USBUART_1_TD USBUART_1_currentTD;

#if(USBUART_1_EP_MM != USBUART_1__EP_MANUAL)
    extern uint8 USBUART_1_DmaChan[USBUART_1_MAX_EP];
    extern uint8 USBUART_1_DmaTd[USBUART_1_MAX_EP];
#endif /* End USBUART_1_EP_MM */

extern volatile uint8 USBUART_1_ep0Toggle;
extern volatile uint8 USBUART_1_lastPacketSize;
extern volatile uint8 USBUART_1_ep0Mode;
extern volatile uint8 USBUART_1_ep0Count;
extern volatile uint16 USBUART_1_transferByteCount;


/***************************************
*     Private Function Prototypes
***************************************/
void  USBUART_1_ReInitComponent(void) ;
void  USBUART_1_HandleSetup(void) ;
void  USBUART_1_HandleIN(void) ;
void  USBUART_1_HandleOUT(void) ;
void  USBUART_1_LoadEP0(void) ;
uint8 USBUART_1_InitControlRead(void) ;
uint8 USBUART_1_InitControlWrite(void) ;
void  USBUART_1_ControlReadDataStage(void) ;
void  USBUART_1_ControlReadStatusStage(void) ;
void  USBUART_1_ControlReadPrematureStatus(void)
                                                ;
uint8 USBUART_1_InitControlWrite(void) ;
uint8 USBUART_1_InitZeroLengthControlTransfer(void)
                                                ;
void  USBUART_1_ControlWriteDataStage(void) ;
void  USBUART_1_ControlWriteStatusStage(void) ;
void  USBUART_1_ControlWritePrematureStatus(void)
                                                ;
uint8 USBUART_1_InitNoDataControlTransfer(void) ;
void  USBUART_1_NoDataControlStatusStage(void) ;
void  USBUART_1_InitializeStatusBlock(void) ;
void  USBUART_1_UpdateStatusBlock(uint8 completionCode) ;
uint8 USBUART_1_DispatchClassRqst(void) ;

void USBUART_1_Config(uint8 clearAltSetting) ;
void USBUART_1_ConfigAltChanged(void) ;
void USBUART_1_ConfigReg(void) ;

const T_USBUART_1_LUT CYCODE *USBUART_1_GetConfigTablePtr(uint8 c)
                                                            ;
const T_USBUART_1_LUT CYCODE *USBUART_1_GetDeviceTablePtr(void)
                                                            ;
const uint8 CYCODE *USBUART_1_GetInterfaceClassTablePtr(void)
                                                    ;
uint8 USBUART_1_ClearEndpointHalt(void) ;
uint8 USBUART_1_SetEndpointHalt(void) ;
uint8 USBUART_1_ValidateAlternateSetting(void) ;

void USBUART_1_SaveConfig(void) ;
void USBUART_1_RestoreConfig(void) ;

#if defined(USBUART_1_ENABLE_IDSN_STRING)
    void USBUART_1_ReadDieID(uint8 descr[]) ;
#endif /* USBUART_1_ENABLE_IDSN_STRING */

#if defined(USBUART_1_ENABLE_HID_CLASS)
    uint8 USBUART_1_DispatchHIDClassRqst(void);
#endif /* End USBUART_1_ENABLE_HID_CLASS */
#if defined(USBUART_1_ENABLE_AUDIO_CLASS)
    uint8 USBUART_1_DispatchAUDIOClassRqst(void);
#endif /* End USBUART_1_ENABLE_HID_CLASS */
#if defined(USBUART_1_ENABLE_CDC_CLASS)
    uint8 USBUART_1_DispatchCDCClassRqst(void);
#endif /* End USBUART_1_ENABLE_CDC_CLASS */

CY_ISR_PROTO(USBUART_1_EP_0_ISR);
#if(USBUART_1_EP1_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_1_EP_1_ISR);
#endif /* End USBUART_1_EP1_ISR_REMOVE */
#if(USBUART_1_EP2_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_1_EP_2_ISR);
#endif /* End USBUART_1_EP2_ISR_REMOVE */
#if(USBUART_1_EP3_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_1_EP_3_ISR);
#endif /* End USBUART_1_EP3_ISR_REMOVE */
#if(USBUART_1_EP4_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_1_EP_4_ISR);
#endif /* End USBUART_1_EP4_ISR_REMOVE */
#if(USBUART_1_EP5_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_1_EP_5_ISR);
#endif /* End USBUART_1_EP5_ISR_REMOVE */
#if(USBUART_1_EP6_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_1_EP_6_ISR);
#endif /* End USBUART_1_EP6_ISR_REMOVE */
#if(USBUART_1_EP7_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_1_EP_7_ISR);
#endif /* End USBUART_1_EP7_ISR_REMOVE */
#if(USBUART_1_EP8_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_1_EP_8_ISR);
#endif /* End USBUART_1_EP8_ISR_REMOVE */
CY_ISR_PROTO(USBUART_1_BUS_RESET_ISR);
#if(USBUART_1_SOF_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_1_SOF_ISR);
#endif /* End USBUART_1_SOF_ISR_REMOVE */
#if(USBUART_1_EP_MM != USBUART_1__EP_MANUAL)
    CY_ISR_PROTO(USBUART_1_ARB_ISR);
#endif /* End USBUART_1_EP_MM */
#if(USBUART_1_DP_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBUART_1_DP_ISR);
#endif /* End USBUART_1_DP_ISR_REMOVE */


/***************************************
* Request Handlers
***************************************/

uint8 USBUART_1_HandleStandardRqst(void) ;
uint8 USBUART_1_DispatchClassRqst(void) ;
uint8 USBUART_1_HandleVendorRqst(void) ;


/***************************************
*    HID Internal references
***************************************/
#if defined(USBUART_1_ENABLE_HID_CLASS)
    void USBUART_1_FindReport(void) ;
    void USBUART_1_FindReportDescriptor(void) ;
    void USBUART_1_FindHidClassDecriptor(void) ;
#endif /* USBUART_1_ENABLE_HID_CLASS */


/***************************************
*    MIDI Internal references
***************************************/
#if defined(USBUART_1_ENABLE_MIDI_STREAMING)
    void USBUART_1_MIDI_IN_EP_Service(void) ;
#endif /* USBUART_1_ENABLE_MIDI_STREAMING */


#endif /* CY_USBFS_USBUART_1_pvt_H */


/* [] END OF FILE */
