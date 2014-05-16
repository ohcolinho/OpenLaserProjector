/*******************************************************************************
* File Name: y_out.h  
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

#if !defined(CY_PINS_y_out_H) /* Pins y_out_H */
#define CY_PINS_y_out_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "y_out_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 y_out__PORT == 15 && ((y_out__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    y_out_Write(uint8 value) ;
void    y_out_SetDriveMode(uint8 mode) ;
uint8   y_out_ReadDataReg(void) ;
uint8   y_out_Read(void) ;
uint8   y_out_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define y_out_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define y_out_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define y_out_DM_RES_UP          PIN_DM_RES_UP
#define y_out_DM_RES_DWN         PIN_DM_RES_DWN
#define y_out_DM_OD_LO           PIN_DM_OD_LO
#define y_out_DM_OD_HI           PIN_DM_OD_HI
#define y_out_DM_STRONG          PIN_DM_STRONG
#define y_out_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define y_out_MASK               y_out__MASK
#define y_out_SHIFT              y_out__SHIFT
#define y_out_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define y_out_PS                     (* (reg8 *) y_out__PS)
/* Data Register */
#define y_out_DR                     (* (reg8 *) y_out__DR)
/* Port Number */
#define y_out_PRT_NUM                (* (reg8 *) y_out__PRT) 
/* Connect to Analog Globals */                                                  
#define y_out_AG                     (* (reg8 *) y_out__AG)                       
/* Analog MUX bux enable */
#define y_out_AMUX                   (* (reg8 *) y_out__AMUX) 
/* Bidirectional Enable */                                                        
#define y_out_BIE                    (* (reg8 *) y_out__BIE)
/* Bit-mask for Aliased Register Access */
#define y_out_BIT_MASK               (* (reg8 *) y_out__BIT_MASK)
/* Bypass Enable */
#define y_out_BYP                    (* (reg8 *) y_out__BYP)
/* Port wide control signals */                                                   
#define y_out_CTL                    (* (reg8 *) y_out__CTL)
/* Drive Modes */
#define y_out_DM0                    (* (reg8 *) y_out__DM0) 
#define y_out_DM1                    (* (reg8 *) y_out__DM1)
#define y_out_DM2                    (* (reg8 *) y_out__DM2) 
/* Input Buffer Disable Override */
#define y_out_INP_DIS                (* (reg8 *) y_out__INP_DIS)
/* LCD Common or Segment Drive */
#define y_out_LCD_COM_SEG            (* (reg8 *) y_out__LCD_COM_SEG)
/* Enable Segment LCD */
#define y_out_LCD_EN                 (* (reg8 *) y_out__LCD_EN)
/* Slew Rate Control */
#define y_out_SLW                    (* (reg8 *) y_out__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define y_out_PRTDSI__CAPS_SEL       (* (reg8 *) y_out__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define y_out_PRTDSI__DBL_SYNC_IN    (* (reg8 *) y_out__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define y_out_PRTDSI__OE_SEL0        (* (reg8 *) y_out__PRTDSI__OE_SEL0) 
#define y_out_PRTDSI__OE_SEL1        (* (reg8 *) y_out__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define y_out_PRTDSI__OUT_SEL0       (* (reg8 *) y_out__PRTDSI__OUT_SEL0) 
#define y_out_PRTDSI__OUT_SEL1       (* (reg8 *) y_out__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define y_out_PRTDSI__SYNC_OUT       (* (reg8 *) y_out__PRTDSI__SYNC_OUT) 


#if defined(y_out__INTSTAT)  /* Interrupt Registers */

    #define y_out_INTSTAT                (* (reg8 *) y_out__INTSTAT)
    #define y_out_SNAP                   (* (reg8 *) y_out__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_y_out_H */


/* [] END OF FILE */
