/*******************************************************************************
* File Name: laser_state.h  
* Version 1.90
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_laser_state_H) /* Pins laser_state_H */
#define CY_PINS_laser_state_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "laser_state_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 laser_state__PORT == 15 && ((laser_state__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    laser_state_Write(uint8 value) ;
void    laser_state_SetDriveMode(uint8 mode) ;
uint8   laser_state_ReadDataReg(void) ;
uint8   laser_state_Read(void) ;
uint8   laser_state_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define laser_state_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define laser_state_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define laser_state_DM_RES_UP          PIN_DM_RES_UP
#define laser_state_DM_RES_DWN         PIN_DM_RES_DWN
#define laser_state_DM_OD_LO           PIN_DM_OD_LO
#define laser_state_DM_OD_HI           PIN_DM_OD_HI
#define laser_state_DM_STRONG          PIN_DM_STRONG
#define laser_state_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define laser_state_MASK               laser_state__MASK
#define laser_state_SHIFT              laser_state__SHIFT
#define laser_state_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define laser_state_PS                     (* (reg8 *) laser_state__PS)
/* Data Register */
#define laser_state_DR                     (* (reg8 *) laser_state__DR)
/* Port Number */
#define laser_state_PRT_NUM                (* (reg8 *) laser_state__PRT) 
/* Connect to Analog Globals */                                                  
#define laser_state_AG                     (* (reg8 *) laser_state__AG)                       
/* Analog MUX bux enable */
#define laser_state_AMUX                   (* (reg8 *) laser_state__AMUX) 
/* Bidirectional Enable */                                                        
#define laser_state_BIE                    (* (reg8 *) laser_state__BIE)
/* Bit-mask for Aliased Register Access */
#define laser_state_BIT_MASK               (* (reg8 *) laser_state__BIT_MASK)
/* Bypass Enable */
#define laser_state_BYP                    (* (reg8 *) laser_state__BYP)
/* Port wide control signals */                                                   
#define laser_state_CTL                    (* (reg8 *) laser_state__CTL)
/* Drive Modes */
#define laser_state_DM0                    (* (reg8 *) laser_state__DM0) 
#define laser_state_DM1                    (* (reg8 *) laser_state__DM1)
#define laser_state_DM2                    (* (reg8 *) laser_state__DM2) 
/* Input Buffer Disable Override */
#define laser_state_INP_DIS                (* (reg8 *) laser_state__INP_DIS)
/* LCD Common or Segment Drive */
#define laser_state_LCD_COM_SEG            (* (reg8 *) laser_state__LCD_COM_SEG)
/* Enable Segment LCD */
#define laser_state_LCD_EN                 (* (reg8 *) laser_state__LCD_EN)
/* Slew Rate Control */
#define laser_state_SLW                    (* (reg8 *) laser_state__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define laser_state_PRTDSI__CAPS_SEL       (* (reg8 *) laser_state__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define laser_state_PRTDSI__DBL_SYNC_IN    (* (reg8 *) laser_state__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define laser_state_PRTDSI__OE_SEL0        (* (reg8 *) laser_state__PRTDSI__OE_SEL0) 
#define laser_state_PRTDSI__OE_SEL1        (* (reg8 *) laser_state__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define laser_state_PRTDSI__OUT_SEL0       (* (reg8 *) laser_state__PRTDSI__OUT_SEL0) 
#define laser_state_PRTDSI__OUT_SEL1       (* (reg8 *) laser_state__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define laser_state_PRTDSI__SYNC_OUT       (* (reg8 *) laser_state__PRTDSI__SYNC_OUT) 


#if defined(laser_state__INTSTAT)  /* Interrupt Registers */

    #define laser_state_INTSTAT                (* (reg8 *) laser_state__INTSTAT)
    #define laser_state_SNAP                   (* (reg8 *) laser_state__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_laser_state_H */


/* [] END OF FILE */
