/*******************************************************************************
* File Name: laser_state.c  
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
#include "laser_state.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 laser_state__PORT == 15 && ((laser_state__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: laser_state_Write
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
void laser_state_Write(uint8 value) 
{
    uint8 staticBits = (laser_state_DR & (uint8)(~laser_state_MASK));
    laser_state_DR = staticBits | ((uint8)(value << laser_state_SHIFT) & laser_state_MASK);
}


/*******************************************************************************
* Function Name: laser_state_SetDriveMode
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
void laser_state_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(laser_state_0, mode);
}


/*******************************************************************************
* Function Name: laser_state_Read
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
*  Macro laser_state_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 laser_state_Read(void) 
{
    return (laser_state_PS & laser_state_MASK) >> laser_state_SHIFT;
}


/*******************************************************************************
* Function Name: laser_state_ReadDataReg
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
uint8 laser_state_ReadDataReg(void) 
{
    return (laser_state_DR & laser_state_MASK) >> laser_state_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(laser_state_INTSTAT) 

    /*******************************************************************************
    * Function Name: laser_state_ClearInterrupt
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
    uint8 laser_state_ClearInterrupt(void) 
    {
        return (laser_state_INTSTAT & laser_state_MASK) >> laser_state_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
