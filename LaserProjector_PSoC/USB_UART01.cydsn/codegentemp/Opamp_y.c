/*******************************************************************************
* File Name: Opamp_y.c
* Version 1.90
*
* Description:
*  This file provides the source code to the API for OpAmp (Analog Buffer) 
*  Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Opamp_y.h"

uint8 Opamp_y_initVar = 0u;


/*******************************************************************************   
* Function Name: Opamp_y_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  Opamp_y_Start().
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Opamp_y_Init(void) 
{
    Opamp_y_SetPower(Opamp_y_DEFAULT_POWER);
}


/*******************************************************************************   
* Function Name: Opamp_y_Enable
********************************************************************************
*
* Summary:
*  Enables the OpAmp block operation
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Opamp_y_Enable(void) 
{
    /* Enable negative charge pumps in ANIF */
    Opamp_y_PUMP_CR1_REG  |= (Opamp_y_PUMP_CR1_CLKSEL | Opamp_y_PUMP_CR1_FORCE);

    /* Enable power to buffer in active mode */
    Opamp_y_PM_ACT_CFG_REG |= Opamp_y_ACT_PWR_EN;

    /* Enable power to buffer in alternative active mode */
    Opamp_y_PM_STBY_CFG_REG |= Opamp_y_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name:   Opamp_y_Start
********************************************************************************
*
* Summary:
*  The start function initializes the Analog Buffer with the default values and 
*  sets the power to the given level. A power level of 0, is same as 
*  executing the stop function.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Opamp_y_initVar: Used to check the initial configuration, modified 
*  when this function is called for the first time.
*
*******************************************************************************/
void Opamp_y_Start(void) 
{
    if(Opamp_y_initVar == 0u)
    {
        Opamp_y_initVar = 1u;
        Opamp_y_Init();
    }

    Opamp_y_Enable();
}


/*******************************************************************************
* Function Name: Opamp_y_Stop
********************************************************************************
*
* Summary:
*  Powers down amplifier to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Opamp_y_Stop(void) 
{
    /* Disable power to buffer in active mode template */
    Opamp_y_PM_ACT_CFG_REG &= (uint8)(~Opamp_y_ACT_PWR_EN);

    /* Disable power to buffer in alternative active mode template */
    Opamp_y_PM_STBY_CFG_REG &= (uint8)(~Opamp_y_STBY_PWR_EN);
    
    /* Disable negative charge pumps for ANIF only if all ABuf is turned OFF */
    if(Opamp_y_PM_ACT_CFG_REG == 0u)
    {
        Opamp_y_PUMP_CR1_REG &= (uint8)(~(Opamp_y_PUMP_CR1_CLKSEL | Opamp_y_PUMP_CR1_FORCE));
    }
}


/*******************************************************************************
* Function Name: Opamp_y_SetPower
********************************************************************************
*
* Summary:
*  Sets power level of Analog buffer.
*
* Parameters: 
*  power: PSoC3: Sets power level between low (1) and high power (3).
*         PSoC5: Sets power level High (0)
*
* Return:
*  void
*
**********************************************************************************/
void Opamp_y_SetPower(uint8 power) 
{
    #if (CY_PSOC3 || CY_PSOC5LP)
        Opamp_y_CR_REG &= (uint8)(~Opamp_y_PWR_MASK);
        Opamp_y_CR_REG |= power & Opamp_y_PWR_MASK;      /* Set device power */
    #else
        CYASSERT(Opamp_y_HIGHPOWER == power);
    #endif /* CY_PSOC3 || CY_PSOC5LP */
}


/* [] END OF FILE */
