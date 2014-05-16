/*******************************************************************************
* File Name: Write_Timer.h
* Version 2.50
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_Timer_v2_30_Write_Timer_H)
#define CY_Timer_v2_30_Write_Timer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Write_Timer_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define Write_Timer_Resolution                 8u
#define Write_Timer_UsingFixedFunction         0u
#define Write_Timer_UsingHWCaptureCounter      0u
#define Write_Timer_SoftwareCaptureMode        0u
#define Write_Timer_SoftwareTriggerMode        0u
#define Write_Timer_UsingHWEnable              0u
#define Write_Timer_EnableTriggerMode          0u
#define Write_Timer_InterruptOnCaptureCount    0u
#define Write_Timer_RunModeUsed                0u
#define Write_Timer_ControlRegRemoved          0u


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!Write_Timer_UsingFixedFunction)
        #if (CY_UDB_V0)
            uint8 TimerUdb;                 /* Timer internal counter value */
            uint8 TimerPeriod;              /* Timer Period value       */
            uint8 InterruptMaskValue;       /* Timer Compare Value */
            #if (Write_Timer_UsingHWCaptureCounter)
                uint8 TimerCaptureCounter;  /* Timer Capture Counter Value */
            #endif /* variable declaration for backing up Capture Counter value*/
        #endif /* variables for non retention registers in CY_UDB_V0 */

        #if (CY_UDB_V1)
            uint8 TimerUdb;
            uint8 InterruptMaskValue;
            #if (Write_Timer_UsingHWCaptureCounter)
                uint8 TimerCaptureCounter;
            #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */
        #endif /* (CY_UDB_V1) */

        #if (!Write_Timer_ControlRegRemoved)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */
}Write_Timer_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    Write_Timer_Start(void) ;
void    Write_Timer_Stop(void) ;

void    Write_Timer_SetInterruptMode(uint8 interruptMode) ;
uint8   Write_Timer_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define Write_Timer_GetInterruptSource() Write_Timer_ReadStatusRegister()

#if(!Write_Timer_ControlRegRemoved)
    uint8   Write_Timer_ReadControlRegister(void) ;
    void    Write_Timer_WriteControlRegister(uint8 control) \
        ;
#endif /* (!Write_Timer_ControlRegRemoved) */

uint8  Write_Timer_ReadPeriod(void) ;
void    Write_Timer_WritePeriod(uint8 period) \
    ;
uint8  Write_Timer_ReadCounter(void) ;
void    Write_Timer_WriteCounter(uint8 counter) \
    ;
uint8  Write_Timer_ReadCapture(void) ;
void    Write_Timer_SoftwareCapture(void) ;


#if(!Write_Timer_UsingFixedFunction) /* UDB Prototypes */
    #if (Write_Timer_SoftwareCaptureMode)
        void    Write_Timer_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!Write_Timer_UsingFixedFunction) */

    #if (Write_Timer_SoftwareTriggerMode)
        void    Write_Timer_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (Write_Timer_SoftwareTriggerMode) */
    #if (Write_Timer_EnableTriggerMode)
        void    Write_Timer_EnableTrigger(void) ;
        void    Write_Timer_DisableTrigger(void) ;
    #endif /* (Write_Timer_EnableTriggerMode) */

    #if(Write_Timer_InterruptOnCaptureCount)
        #if(!Write_Timer_ControlRegRemoved)
            void    Write_Timer_SetInterruptCount(uint8 interruptCount) \
                ;
        #endif /* (!Write_Timer_ControlRegRemoved) */
    #endif /* (Write_Timer_InterruptOnCaptureCount) */

    #if (Write_Timer_UsingHWCaptureCounter)
        void    Write_Timer_SetCaptureCount(uint8 captureCount) \
            ;
        uint8   Write_Timer_ReadCaptureCount(void) ;
    #endif /* (Write_Timer_UsingHWCaptureCounter) */

    void Write_Timer_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void Write_Timer_Init(void)          ;
void Write_Timer_Enable(void)        ;
void Write_Timer_SaveConfig(void)    ;
void Write_Timer_RestoreConfig(void) ;
void Write_Timer_Sleep(void)         ;
void Write_Timer_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define Write_Timer__B_TIMER__CM_NONE 0
#define Write_Timer__B_TIMER__CM_RISINGEDGE 1
#define Write_Timer__B_TIMER__CM_FALLINGEDGE 2
#define Write_Timer__B_TIMER__CM_EITHEREDGE 3
#define Write_Timer__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define Write_Timer__B_TIMER__TM_NONE 0x00u
#define Write_Timer__B_TIMER__TM_RISINGEDGE 0x04u
#define Write_Timer__B_TIMER__TM_FALLINGEDGE 0x08u
#define Write_Timer__B_TIMER__TM_EITHEREDGE 0x0Cu
#define Write_Timer__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define Write_Timer_INIT_PERIOD             255u
#define Write_Timer_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << Write_Timer_CTRL_CAP_MODE_SHIFT))
#define Write_Timer_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << Write_Timer_CTRL_TRIG_MODE_SHIFT))
#if (Write_Timer_UsingFixedFunction)
    #define Write_Timer_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << Write_Timer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << Write_Timer_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define Write_Timer_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << Write_Timer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Write_Timer_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Write_Timer_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (Write_Timer_UsingFixedFunction) */
#define Write_Timer_INIT_CAPTURE_COUNT      (2u)
#define Write_Timer_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << Write_Timer_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (Write_Timer_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define Write_Timer_STATUS         (*(reg8 *) Write_Timer_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define Write_Timer_STATUS_MASK    (*(reg8 *) Write_Timer_TimerHW__SR0 )
    #define Write_Timer_CONTROL        (*(reg8 *) Write_Timer_TimerHW__CFG0)
    #define Write_Timer_CONTROL2       (*(reg8 *) Write_Timer_TimerHW__CFG1)
    #define Write_Timer_CONTROL2_PTR   ( (reg8 *) Write_Timer_TimerHW__CFG1)
    #define Write_Timer_RT1            (*(reg8 *) Write_Timer_TimerHW__RT1)
    #define Write_Timer_RT1_PTR        ( (reg8 *) Write_Timer_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define Write_Timer_CONTROL3       (*(reg8 *) Write_Timer_TimerHW__CFG2)
        #define Write_Timer_CONTROL3_PTR   ( (reg8 *) Write_Timer_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define Write_Timer_GLOBAL_ENABLE  (*(reg8 *) Write_Timer_TimerHW__PM_ACT_CFG)
    #define Write_Timer_GLOBAL_STBY_ENABLE  (*(reg8 *) Write_Timer_TimerHW__PM_STBY_CFG)

    #define Write_Timer_CAPTURE_LSB         (* (reg16 *) Write_Timer_TimerHW__CAP0 )
    #define Write_Timer_CAPTURE_LSB_PTR       ((reg16 *) Write_Timer_TimerHW__CAP0 )
    #define Write_Timer_PERIOD_LSB          (* (reg16 *) Write_Timer_TimerHW__PER0 )
    #define Write_Timer_PERIOD_LSB_PTR        ((reg16 *) Write_Timer_TimerHW__PER0 )
    #define Write_Timer_COUNTER_LSB         (* (reg16 *) Write_Timer_TimerHW__CNT_CMP0 )
    #define Write_Timer_COUNTER_LSB_PTR       ((reg16 *) Write_Timer_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define Write_Timer_BLOCK_EN_MASK                     Write_Timer_TimerHW__PM_ACT_MSK
    #define Write_Timer_BLOCK_STBY_EN_MASK                Write_Timer_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define Write_Timer_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define Write_Timer_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define Write_Timer_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define Write_Timer_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define Write_Timer_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define Write_Timer_CTRL_ENABLE                        ((uint8)((uint8)0x01u << Write_Timer_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Write_Timer_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define Write_Timer_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << Write_Timer_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define Write_Timer_CTRL_MODE_SHIFT                     0x01u
        #define Write_Timer_CTRL_MODE_MASK                     ((uint8)((uint8)0x07u << Write_Timer_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define Write_Timer_CTRL_RCOD_SHIFT        0x02u
        #define Write_Timer_CTRL_ENBL_SHIFT        0x00u
        #define Write_Timer_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define Write_Timer_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << Write_Timer_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define Write_Timer_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << Write_Timer_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define Write_Timer_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << Write_Timer_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define Write_Timer_CTRL_RCOD       ((uint8)((uint8)0x03u << Write_Timer_CTRL_RCOD_SHIFT))
        #define Write_Timer_CTRL_ENBL       ((uint8)((uint8)0x80u << Write_Timer_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define Write_Timer_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define Write_Timer_RT1_MASK                        ((uint8)((uint8)0x03u << Write_Timer_RT1_SHIFT))
    #define Write_Timer_SYNC                            ((uint8)((uint8)0x03u << Write_Timer_RT1_SHIFT))
    #define Write_Timer_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define Write_Timer_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << Write_Timer_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define Write_Timer_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << Write_Timer_SYNCDSI_SHIFT))

    #define Write_Timer_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << Write_Timer_CTRL_MODE_SHIFT))
    #define Write_Timer_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << Write_Timer_CTRL_MODE_SHIFT))
    #define Write_Timer_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << Write_Timer_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Write_Timer_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Write_Timer_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Write_Timer_STATUS_TC_INT_MASK_SHIFT        (Write_Timer_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Write_Timer_STATUS_CAPTURE_INT_MASK_SHIFT   (Write_Timer_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define Write_Timer_STATUS_TC                       ((uint8)((uint8)0x01u << Write_Timer_STATUS_TC_SHIFT))
    #define Write_Timer_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << Write_Timer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define Write_Timer_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << Write_Timer_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define Write_Timer_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << Write_Timer_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define Write_Timer_STATUS              (* (reg8 *) Write_Timer_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define Write_Timer_STATUS_MASK         (* (reg8 *) Write_Timer_TimerUDB_rstSts_stsreg__MASK_REG)
    #define Write_Timer_STATUS_AUX_CTRL     (* (reg8 *) Write_Timer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define Write_Timer_CONTROL             (* (reg8 *) Write_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(Write_Timer_Resolution <= 8u) /* 8-bit Timer */
        #define Write_Timer_CAPTURE_LSB         (* (reg8 *) Write_Timer_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Write_Timer_CAPTURE_LSB_PTR       ((reg8 *) Write_Timer_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Write_Timer_PERIOD_LSB          (* (reg8 *) Write_Timer_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Write_Timer_PERIOD_LSB_PTR        ((reg8 *) Write_Timer_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Write_Timer_COUNTER_LSB         (* (reg8 *) Write_Timer_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define Write_Timer_COUNTER_LSB_PTR       ((reg8 *) Write_Timer_TimerUDB_sT8_timerdp_u0__A0_REG )
    #elif(Write_Timer_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define Write_Timer_CAPTURE_LSB         (* (reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Write_Timer_CAPTURE_LSB_PTR       ((reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Write_Timer_PERIOD_LSB          (* (reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Write_Timer_PERIOD_LSB_PTR        ((reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Write_Timer_COUNTER_LSB         (* (reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define Write_Timer_COUNTER_LSB_PTR       ((reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define Write_Timer_CAPTURE_LSB         (* (reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define Write_Timer_CAPTURE_LSB_PTR       ((reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define Write_Timer_PERIOD_LSB          (* (reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define Write_Timer_PERIOD_LSB_PTR        ((reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define Write_Timer_COUNTER_LSB         (* (reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
            #define Write_Timer_COUNTER_LSB_PTR       ((reg16 *) Write_Timer_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(Write_Timer_Resolution <= 24u)/* 24-bit Timer */
        #define Write_Timer_CAPTURE_LSB         (* (reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Write_Timer_CAPTURE_LSB_PTR       ((reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define Write_Timer_PERIOD_LSB          (* (reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Write_Timer_PERIOD_LSB_PTR        ((reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define Write_Timer_COUNTER_LSB         (* (reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define Write_Timer_COUNTER_LSB_PTR       ((reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define Write_Timer_CAPTURE_LSB         (* (reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Write_Timer_CAPTURE_LSB_PTR       ((reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define Write_Timer_PERIOD_LSB          (* (reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Write_Timer_PERIOD_LSB_PTR        ((reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define Write_Timer_COUNTER_LSB         (* (reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define Write_Timer_COUNTER_LSB_PTR       ((reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define Write_Timer_CAPTURE_LSB         (* (reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define Write_Timer_CAPTURE_LSB_PTR       ((reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define Write_Timer_PERIOD_LSB          (* (reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define Write_Timer_PERIOD_LSB_PTR        ((reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define Write_Timer_COUNTER_LSB         (* (reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
            #define Write_Timer_COUNTER_LSB_PTR       ((reg32 *) Write_Timer_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #if (Write_Timer_UsingHWCaptureCounter)
        #define Write_Timer_CAP_COUNT              (*(reg8 *) Write_Timer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Write_Timer_CAP_COUNT_PTR          ( (reg8 *) Write_Timer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Write_Timer_CAPTURE_COUNT_CTRL     (*(reg8 *) Write_Timer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define Write_Timer_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) Write_Timer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (Write_Timer_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define Write_Timer_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define Write_Timer_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define Write_Timer_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define Write_Timer_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define Write_Timer_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define Write_Timer_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << Write_Timer_CTRL_INTCNT_SHIFT))
    #define Write_Timer_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << Write_Timer_CTRL_TRIG_MODE_SHIFT))
    #define Write_Timer_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << Write_Timer_CTRL_TRIG_EN_SHIFT))
    #define Write_Timer_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << Write_Timer_CTRL_CAP_MODE_SHIFT))
    #define Write_Timer_CTRL_ENABLE                    ((uint8)((uint8)0x01u << Write_Timer_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define Write_Timer_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define Write_Timer_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define Write_Timer_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define Write_Timer_STATUS_TC_INT_MASK_SHIFT       Write_Timer_STATUS_TC_SHIFT
    #define Write_Timer_STATUS_CAPTURE_INT_MASK_SHIFT  Write_Timer_STATUS_CAPTURE_SHIFT
    #define Write_Timer_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define Write_Timer_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define Write_Timer_STATUS_FIFOFULL_INT_MASK_SHIFT Write_Timer_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define Write_Timer_STATUS_TC                      ((uint8)((uint8)0x01u << Write_Timer_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define Write_Timer_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << Write_Timer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Write_Timer_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << Write_Timer_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Write_Timer_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << Write_Timer_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define Write_Timer_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << Write_Timer_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define Write_Timer_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << Write_Timer_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Write_Timer_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << Write_Timer_STATUS_FIFOFULL_SHIFT))

    #define Write_Timer_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define Write_Timer_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define Write_Timer_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define Write_Timer_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define Write_Timer_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define Write_Timer_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_Write_Timer_H */


/* [] END OF FILE */
