/*******************************************************************************
* File Name: Push_Switch.h  
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

#if !defined(CY_PINS_Push_Switch_H) /* Pins Push_Switch_H */
#define CY_PINS_Push_Switch_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Push_Switch_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Push_Switch__PORT == 15 && ((Push_Switch__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Push_Switch_Write(uint8 value) ;
void    Push_Switch_SetDriveMode(uint8 mode) ;
uint8   Push_Switch_ReadDataReg(void) ;
uint8   Push_Switch_Read(void) ;
uint8   Push_Switch_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Push_Switch_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Push_Switch_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Push_Switch_DM_RES_UP          PIN_DM_RES_UP
#define Push_Switch_DM_RES_DWN         PIN_DM_RES_DWN
#define Push_Switch_DM_OD_LO           PIN_DM_OD_LO
#define Push_Switch_DM_OD_HI           PIN_DM_OD_HI
#define Push_Switch_DM_STRONG          PIN_DM_STRONG
#define Push_Switch_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Push_Switch_MASK               Push_Switch__MASK
#define Push_Switch_SHIFT              Push_Switch__SHIFT
#define Push_Switch_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Push_Switch_PS                     (* (reg8 *) Push_Switch__PS)
/* Data Register */
#define Push_Switch_DR                     (* (reg8 *) Push_Switch__DR)
/* Port Number */
#define Push_Switch_PRT_NUM                (* (reg8 *) Push_Switch__PRT) 
/* Connect to Analog Globals */                                                  
#define Push_Switch_AG                     (* (reg8 *) Push_Switch__AG)                       
/* Analog MUX bux enable */
#define Push_Switch_AMUX                   (* (reg8 *) Push_Switch__AMUX) 
/* Bidirectional Enable */                                                        
#define Push_Switch_BIE                    (* (reg8 *) Push_Switch__BIE)
/* Bit-mask for Aliased Register Access */
#define Push_Switch_BIT_MASK               (* (reg8 *) Push_Switch__BIT_MASK)
/* Bypass Enable */
#define Push_Switch_BYP                    (* (reg8 *) Push_Switch__BYP)
/* Port wide control signals */                                                   
#define Push_Switch_CTL                    (* (reg8 *) Push_Switch__CTL)
/* Drive Modes */
#define Push_Switch_DM0                    (* (reg8 *) Push_Switch__DM0) 
#define Push_Switch_DM1                    (* (reg8 *) Push_Switch__DM1)
#define Push_Switch_DM2                    (* (reg8 *) Push_Switch__DM2) 
/* Input Buffer Disable Override */
#define Push_Switch_INP_DIS                (* (reg8 *) Push_Switch__INP_DIS)
/* LCD Common or Segment Drive */
#define Push_Switch_LCD_COM_SEG            (* (reg8 *) Push_Switch__LCD_COM_SEG)
/* Enable Segment LCD */
#define Push_Switch_LCD_EN                 (* (reg8 *) Push_Switch__LCD_EN)
/* Slew Rate Control */
#define Push_Switch_SLW                    (* (reg8 *) Push_Switch__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Push_Switch_PRTDSI__CAPS_SEL       (* (reg8 *) Push_Switch__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Push_Switch_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Push_Switch__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Push_Switch_PRTDSI__OE_SEL0        (* (reg8 *) Push_Switch__PRTDSI__OE_SEL0) 
#define Push_Switch_PRTDSI__OE_SEL1        (* (reg8 *) Push_Switch__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Push_Switch_PRTDSI__OUT_SEL0       (* (reg8 *) Push_Switch__PRTDSI__OUT_SEL0) 
#define Push_Switch_PRTDSI__OUT_SEL1       (* (reg8 *) Push_Switch__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Push_Switch_PRTDSI__SYNC_OUT       (* (reg8 *) Push_Switch__PRTDSI__SYNC_OUT) 


#if defined(Push_Switch__INTSTAT)  /* Interrupt Registers */

    #define Push_Switch_INTSTAT                (* (reg8 *) Push_Switch__INTSTAT)
    #define Push_Switch_SNAP                   (* (reg8 *) Push_Switch__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Push_Switch_H */


/* [] END OF FILE */
