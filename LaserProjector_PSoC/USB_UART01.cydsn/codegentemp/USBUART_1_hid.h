/*******************************************************************************
* File Name: USBUART_1_hid.h
* Version 2.60
*
* Description:
*  Header File for the USFS component. Contains prototypes and constant values.
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_USBFS_USBUART_1_hid_H)
#define CY_USBFS_USBUART_1_hid_H

#include "cytypes.h"


/***************************************
* Prototypes of the USBUART_1_hid API.
***************************************/

uint8 USBUART_1_UpdateHIDTimer(uint8 interface) ;
uint8 USBUART_1_GetProtocol(uint8 interface) ;


/***************************************
*Renamed Functions for backward compatible
***************************************/

#define USBUART_1_bGetProtocol               USBUART_1_GetProtocol


/***************************************
*  Constants for USBUART_1_hid API.
***************************************/

#define USBUART_1_PROTOCOL_BOOT              (0x00u)
#define USBUART_1_PROTOCOL_REPORT            (0x01u)

/* Request Types (HID Chapter 7.2) */
#define USBUART_1_HID_GET_REPORT             (0x01u)
#define USBUART_1_HID_GET_IDLE               (0x02u)
#define USBUART_1_HID_GET_PROTOCOL           (0x03u)
#define USBUART_1_HID_SET_REPORT             (0x09u)
#define USBUART_1_HID_SET_IDLE               (0x0Au)
#define USBUART_1_HID_SET_PROTOCOL           (0x0Bu)

/* Descriptor Types (HID Chapter 7.1) */
#define USBUART_1_DESCR_HID_CLASS            (0x21u)
#define USBUART_1_DESCR_HID_REPORT           (0x22u)
#define USBUART_1_DESCR_HID_PHYSICAL         (0x23u)

/* Report Request Types (HID Chapter 7.2.1) */
#define USBUART_1_HID_GET_REPORT_INPUT       (0x01u)
#define USBUART_1_HID_GET_REPORT_OUTPUT      (0x02u)
#define USBUART_1_HID_GET_REPORT_FEATURE     (0x03u)

#endif /* End CY_USBFS_USBUART_1_hid_H */


/* [] END OF FILE */
