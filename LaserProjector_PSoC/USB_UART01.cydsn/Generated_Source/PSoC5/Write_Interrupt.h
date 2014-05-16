/*******************************************************************************
* File Name: Write_Interrupt.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_Write_Interrupt_H)
#define CY_ISR_Write_Interrupt_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Write_Interrupt_Start(void);
void Write_Interrupt_StartEx(cyisraddress address);
void Write_Interrupt_Stop(void);

CY_ISR_PROTO(Write_Interrupt_Interrupt);

void Write_Interrupt_SetVector(cyisraddress address);
cyisraddress Write_Interrupt_GetVector(void);

void Write_Interrupt_SetPriority(uint8 priority);
uint8 Write_Interrupt_GetPriority(void);

void Write_Interrupt_Enable(void);
uint8 Write_Interrupt_GetState(void);
void Write_Interrupt_Disable(void);

void Write_Interrupt_SetPending(void);
void Write_Interrupt_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Write_Interrupt ISR. */
#define Write_Interrupt_INTC_VECTOR            ((reg32 *) Write_Interrupt__INTC_VECT)

/* Address of the Write_Interrupt ISR priority. */
#define Write_Interrupt_INTC_PRIOR             ((reg8 *) Write_Interrupt__INTC_PRIOR_REG)

/* Priority of the Write_Interrupt interrupt. */
#define Write_Interrupt_INTC_PRIOR_NUMBER      Write_Interrupt__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Write_Interrupt interrupt. */
#define Write_Interrupt_INTC_SET_EN            ((reg32 *) Write_Interrupt__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Write_Interrupt interrupt. */
#define Write_Interrupt_INTC_CLR_EN            ((reg32 *) Write_Interrupt__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Write_Interrupt interrupt state to pending. */
#define Write_Interrupt_INTC_SET_PD            ((reg32 *) Write_Interrupt__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Write_Interrupt interrupt. */
#define Write_Interrupt_INTC_CLR_PD            ((reg32 *) Write_Interrupt__INTC_CLR_PD_REG)


#endif /* CY_ISR_Write_Interrupt_H */


/* [] END OF FILE */
