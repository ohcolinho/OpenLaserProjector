/*******************************************************************************
* File Name: Push_Switch.c  
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
#include "Push_Switch.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 Push_Switch__PORT == 15 && ((Push_Switch__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: Push_Switch_Write
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
void Push_Switch_Write(uint8 value) 
{
    uint8 staticBits = (Push_Switch_DR & (uint8)(~Push_Switch_MASK));
    Push_Switch_DR = staticBits | ((uint8)(value << Push_Switch_SHIFT) & Push_Switch_MASK);
}


/*******************************************************************************
* Function Name: Push_Switch_SetDriveMode
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
void Push_Switch_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Push_Switch_0, mode);
}


/*******************************************************************************
* Function Name: Push_Switch_Read
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
*  Macro Push_Switch_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Push_Switch_Read(void) 
{
    return (Push_Switch_PS & Push_Switch_MASK) >> Push_Switch_SHIFT;
}


/*******************************************************************************
* Function Name: Push_Switch_ReadDataReg
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
uint8 Push_Switch_ReadDataReg(void) 
{
    return (Push_Switch_DR & Push_Switch_MASK) >> Push_Switch_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Push_Switch_INTSTAT) 

    /*******************************************************************************
    * Function Name: Push_Switch_ClearInterrupt
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
    uint8 Push_Switch_ClearInterrupt(void) 
    {
        return (Push_Switch_INTSTAT & Push_Switch_MASK) >> Push_Switch_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
