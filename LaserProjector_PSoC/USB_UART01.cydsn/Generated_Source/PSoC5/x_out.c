/*******************************************************************************
* File Name: x_out.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "x_out.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 x_out__PORT == 15 && ((x_out__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: x_out_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void x_out_Write(uint8 value) 
{
    uint8 staticBits = (x_out_DR & (uint8)(~x_out_MASK));
    x_out_DR = staticBits | ((uint8)(value << x_out_SHIFT) & x_out_MASK);
}


/*******************************************************************************
* Function Name: x_out_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void x_out_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(x_out_0, mode);
}


/*******************************************************************************
* Function Name: x_out_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro x_out_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 x_out_Read(void) 
{
    return (x_out_PS & x_out_MASK) >> x_out_SHIFT;
}


/*******************************************************************************
* Function Name: x_out_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 x_out_ReadDataReg(void) 
{
    return (x_out_DR & x_out_MASK) >> x_out_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(x_out_INTSTAT) 

    /*******************************************************************************
    * Function Name: x_out_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 x_out_ClearInterrupt(void) 
    {
        return (x_out_INTSTAT & x_out_MASK) >> x_out_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
