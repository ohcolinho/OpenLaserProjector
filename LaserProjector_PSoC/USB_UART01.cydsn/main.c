/*******************************************************************************
* File Name: main.c
*
* Description: This file programs the PSoC to receive data from USB and send control
* signals to laser galvonometers 
*   
*******************************************************************************/

#include <project.h>
#include <stdio.h>
#include <stdlib.h>

#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow the usage of floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
    asm (".global _printf_float");
#endif

#define BUFFER_LEN  50u         // Buffer to receive USBUART data from PC
#define N_COORD 5u              // Number of coordinates per data packet

uint8 buffer[BUFFER_LEN];       // Buffer to store data received from PC
uint8 count;                    // Store number of bytes received

uint8 pt_x[N_COORD];            // x coordinates from data packet
uint8 pt_y[N_COORD];            // y coordinates from data packet
uint8 ls[N_COORD];              // laser state from data packet

uint8 interp_x[2*N_COORD-1];    // Interpolated x points
uint8 interp_y[2*N_COORD-1];    // Interpolated y points
uint8 interp_ls[2*N_COORD-1];   // Interpolated ls points
uint8 isInterpolated;   

uint8 interp_index;
uint8 std_index;

uint8 interp_Mode;                 // Switch between modes

char8 *parity[] = { "None", "Odd", "Even", "Mark", "Space" };
char8 *stop[] = { "1", "1.5", "2" };

char8 lineStr[20];              // String to update LCD

// Define Interrupt Service Routines for:
// 1. Polling USB data from PC
// 2. Sending out control signals to mirror galvonometers
// 3. Switching between interpolated points and standard points

CY_ISR(poll_ISR)
{    
       // Disasserted interrupt signal
       Poll_Timer_ReadStatusRegister();
    
        // Check for input data from PC
        if(USBUART_1_DataIsReady() != 0u){             
        
        // Read received data and re-enable OUT endpoint
        count = USBUART_1_GetAll(buffer);           
        if(count != 0u){   
            
            // Unbundle contents of data packet
            uint8 i;
            for(i = 0; i < N_COORD; i++){
                pt_x[i] = buffer[3*i+0];
                pt_y[i] = buffer[3*i+1];
                ls[i] = buffer[3*i+2];
            }
            
            // New data is not yet interpolated
            isInterpolated = 0;
        }
    }
}

CY_ISR(fire_ISR)
{
    // Disarrert interrupt signal
    Fire_Timer_ReadStatusRegister();
    
    // Check Mode
    if(interp_Mode){
        // Ensure data is interpolated
        if(isInterpolated){
            
            // Ensure the index is not overrun
            if(interp_index < (2*N_COORD-1)){
                // Send out x,y,ls control signals
                VDAC8_x_SetValue(interp_x[interp_index]);
                VDAC8_y_SetValue(interp_y[interp_index]);
                laser_state_Write(interp_ls[interp_index]);
                interp_index++;
            }
        }        
    }
    
    else{
        // Send data over every other coordinate (i.e. original coordiante data)
        if(!(interp_index%2)){
            
           // Ensure the index is not overrun
            if(std_index < N_COORD){
            
                // Send out x,y,ls control signals
                VDAC8_x_SetValue(pt_x[std_index]);
                VDAC8_y_SetValue(pt_y[std_index]);
                laser_state_Write(ls[std_index]); 
            
                std_index++;
            }
        }
        
        interp_index++;
    }

}

CY_ISR(Mode_ISR)
{    
    // Switch mode
    interp_Mode = !interp_Mode;
    
    // Reset indicies
    interp_index = 0;
    std_index = 0;
    
    // Display current mode on LCD
    if(interp_Mode){
        sprintf(lineStr,"Interpolated    ");
        LCD_Position(1u, 0u);
        LCD_PrintString(lineStr);
    }
    else{
        sprintf(lineStr,"Standard        ");
        LCD_Position(1u, 0u);
        LCD_PrintString(lineStr);
    }

}


void main()
{
    /* Declare storage variables */
    uint8 state;
    
    /* Initialize variables */
    interp_index = 0;
    std_index = 0;
    
    /*Initialized boolean flags */
    isInterpolated = 0;
    interp_Mode = 1;
    
    /* Enable Global Interrupts */
    CyGlobalIntEnable;                        
    
    /* Start USBFS Operation with 3V operation */
    USBUART_1_Start(0u, USBUART_1_3V_OPERATION);

    /* Start ADCs and Opamps */
    VDAC8_x_Start();
    VDAC8_y_Start();
    Opamp_x_Start();
    Opamp_y_Start();
        
    /* Start Timers */
    Poll_Timer_Start();
    Fire_Timer_Start();
    
    /* Start LCD */
    LCD_Start();
    LCD_PrintString("USB-UART example");
    
    /* Wait for Device to enumerate */
    while(!USBUART_1_GetConfiguration());
    
    /* Start the Polling and Writing Interrupts*/
    Poll_Interrupt_StartEx(poll_ISR);
    Fire_Interrupt_StartEx(fire_ISR);
    Mode_Interrupt_StartEx(Mode_ISR);

    /* Enumeration is done, enable OUT endpoint for receive data from Host */
    USBUART_1_CDC_Init();

    /* Main Loop: */
    for(;;)
    {           
        // Interpolate points from new data packet
        if(!isInterpolated){
            // Initialize first entry in array
            interp_x[0] = pt_x[0];
            interp_y[0] = pt_y[0];
            interp_ls[0] = ls[0];
            
            uint8 i;
            for(i = 1; i < N_COORD; i++){
                // Interpolate between points for x coordinate
                interp_x[2*i-1] = 0.5*(pt_x[i-1]+pt_x[i]); 
                interp_x[2*i] = pt_x[i];
                
                // Interpolate between points for y coordinate
                interp_y[2*i-1] = 0.5*(pt_y[i-1]+pt_y[i]); 
                interp_y[2*i] = pt_y[i];
                
                // Interpolate laser state signal
                interp_ls[2*i-1] = ls[i-1];
                interp_ls[2*i] = ls[i];
            }
            
            // Update boolean flags
            isInterpolated = 1;
            
            // Reset firing indicies for either mode
            interp_index = 0;
            std_index = 0;
        }
        
        // Start screen on LCD for USB connection
        state = USBUART_1_IsLineChanged();              /* Check for Line settings changed */
        if(state != 0u)
        {  
            if(state & USBUART_1_LINE_CODING_CHANGED)   /* Show new settings */
            {
                sprintf(lineStr,"BR:%4ld,DB:%d",USBUART_1_GetDTERate(),(uint16)USBUART_1_GetDataBits());
                LCD_Position(0u, 0u);
                LCD_PrintString("                    ");
                LCD_Position(0u, 0u);
                LCD_PrintString(lineStr);
                sprintf(lineStr,"SB:%s,Parity:%s", stop[(uint16)USBUART_1_GetCharFormat()], \
                                                     parity[(uint16)USBUART_1_GetParityType()]);
                LCD_Position(1u, 0u);
                LCD_PrintString("                    ");
                LCD_Position(1u, 0u);
                LCD_PrintString(lineStr);
            }
            if(state & USBUART_1_LINE_CONTROL_CHANGED)  /* Show new settings */
            {   
                state = USBUART_1_GetLineControl();
                sprintf(lineStr,"DTR:%s,RTS:%s",  (state & USBUART_1_LINE_CONTROL_DTR) ? "ON" : "OFF", \
                                                    (state & USBUART_1_LINE_CONTROL_RTS) ? "ON" : "OFF");
                LCD_Position(1u, 0u);
                LCD_PrintString("                    ");
                LCD_Position(1u, 0u);
                LCD_PrintString(lineStr);
            
            }
        }
    }   
}