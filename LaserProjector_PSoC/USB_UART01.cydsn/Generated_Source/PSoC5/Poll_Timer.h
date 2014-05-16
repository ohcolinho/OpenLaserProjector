/*******************************************************************************
* File Name: Poll_Timer.h
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

#if !defined(CY_Timer_v2_30_Poll_Timer_H)
#define CY_Timer_v2_30_Poll_Timer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Poll_Timer_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define Poll_Timer_Resolution                 24u
#define Poll_Timer_UsingFixedFunction         0u
#define Poll_Timer_UsingHWCaptureCounter      0u
#define Poll_Timer_SoftwareCaptureMode        0u
#define Poll_Timer_SoftwareTriggerMode        0u
#define Poll_Timer_UsingHWEnable              0u
#define Poll_Timer_EnableTriggerMode          0u
#define Poll_Timer_InterruptOnCaptureCount    0u
#define Poll_Timer_RunModeUsed                0u
#define Poll_Timer_ControlRegRemoved          0u


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!Poll_Timer_UsingFixedFunction)
        #if (CY_UDB_V0)
            uint32 TimerUdb;                 /* Timer internal counter value */
            uint32 TimerPeriod;              /* Timer Period value       */
            uint8 InterruptMaskValue;       /* Timer Compare Value */
            #if (Poll_Timer_UsingHWCaptureCounter)
                uint8 TimerCaptureCounter;  /* Timer Capture Counter Value */
            #endif /* variable declaration for backing up Capture Counter value*/
        #endif /* variables for non retention registers in CY_UDB_V0 */

        #if (CY_UDB_V1)
            uint32 TimerUdb;
            uint8 InterruptMaskValue;
            #if (Poll_Timer_UsingHWCaptureCounter)
                uint8 TimerCaptureCounter;
            #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */
        #endif /* (CY_UDB_V1) */

        #if (!Poll_Timer_ControlRegRemoved)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */
}Poll_Timer_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    Poll_Timer_Start(void) ;
void    Poll_Timer_Stop(void) ;

void    Poll_Timer_SetInterruptMode(uint8 interruptMode) ;
uint8   Poll_Timer_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define Poll_Timer_GetInterruptSource() Poll_Timer_ReadStatusRegister()

#if(!Poll_Timer_ControlRegRemoved)
    uint8   Poll_Timer_ReadControlRegister(void) ;
    void    Poll_Timer_WriteControlRegister(uint8 control) \
        ;
#endif /* (!Poll_Timer_ControlRegRemoved) */

uint32  Poll_Timer_ReadPeriod(void) ;
void    Poll_Timer_WritePeriod(uint32 period) \
    ;
uint32  Poll_Timer_ReadCounter(void) ;
void    Poll_Timer_WriteCounter(uint32 counter) \
    ;
uint32  Poll_Timer_ReadCapture(void) ;
void    Poll_Timer_SoftwareCapture(void) ;


#if(!Poll_Timer_UsingFixedFunction) /* UDB Prototypes */
    #if (Poll_Timer_SoftwareCaptureMode)
        void    Poll_Timer_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!Poll_Timer_UsingFixedFunction) */

    #if (Poll_Timer_SoftwareTriggerMode)
        void    Poll_Timer_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (Poll_Timer_SoftwareTriggerMode) */
    #if (Poll_Timer_EnableTriggerMode)
        void    Poll_Timer_EnableTrigger(void) ;
        void    Poll_Timer_DisableTrigger(void) ;
    #endif /* (Poll_Timer_EnableTriggerMode) */

    #if(Poll_Timer_InterruptOnCaptureCount)
        #if(!Poll_Timer_ControlRegRemoved)
            void    Poll_Timer_SetInterruptCount(uint8 interruptCount) \
                ;
        #endif /* (!Poll_Timer_ControlRegRemoved) */
    #endif /* (Poll_Timer_InterruptOnCaptureCount) */

    #if (Poll_Timer_UsingHWCaptureCounter)
        void    Poll_Timer_SetCaptureCount(uint8 captureCount) \
            ;
        uint8   Poll_Timer_ReadCaptureCount(void) ;
    #endif /* (Poll_Timer_UsingHWCaptureCounter) */

    void Poll_Timer_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void Poll_Timer_Init(void)          ;
void Poll_Timer_Enable(void)        ;
void Poll_Timer_SaveConfig(void)    ;
void Poll_Timer_RestoreConfig(void) ;
void Poll_Timer_Sleep(void)         ;
void Poll_Timer_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define Poll_Timer__B_TIMER__CM_NONE 0
#define Poll_Timer__B_TIMER__CM_RISINGEDGE 1
#define Poll_Timer__B_TIMER__CM_FALLINGEDGE 2
#define Poll_Timer__B_TIMER__CM_EITHEREDGE 3
#define Poll_Timer__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define Poll_Timer__B_TIMER__TM_NONE 0x00u
#define Poll_Timer__B_TIMER__TM_RISINGEDGE 0x04u
#define Poll_Timer__B_TIMER__TM_FALLINGEDGE 0x08u
#define Poll_Timer__B_TIMER__TM_EITHEREDGE 0x0Cu
#define Poll_Timer__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define Poll_Timer_INIT_PERIOD             12799u
#define Poll_Timer_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << Poll_Timer_CTRL_CAP_MODE_SHIFT))
#define Poll_Timer_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << Poll_Timer_CTRL_TRIG_MODE_SHIFT))
#if (Poll_Timer_UsingFixedFunction)
    #define Poll_Timer_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << Poll_Timer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << Poll_Timer_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define Poll_Timer_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << Poll_Timer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Poll_Timer_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Poll_Timer_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (Poll_Timer_UsingFixedFunction) */
#define Poll_Timer_INIT_CAPTURE_COUNT      (2u)
#define Poll_Timer_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << Poll_Timer_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (Poll_Timer_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define Poll_Timer_STATUS         (*(reg8 *) Poll_Timer_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define Poll_Timer_STATUS_MASK    (*(reg8 *) Poll_Timer_TimerHW__SR0 )
    #define Poll_Timer_CONTROL        (*(reg8 *) Poll_Timer_TimerHW__CFG0)
    #define Poll_Timer_CONTROL2       (*(reg8 *) Poll_Timer_TimerHW__CFG1)
    #define Poll_Timer_CONTROL2_PTR   ( (reg8 *) Poll_Timer_TimerHW__CFG1)
    #define Poll_Timer_RT1            (*(reg8 *) Poll_Timer_TimerHW__RT1)
    #define Poll_Timer_RT1_PTR        ( (reg8 *) Poll_Timer_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define Poll_Timer_CONTROL3       (*(reg8 *) Poll_Timer_TimerHW__CFG2)
        #define Poll_Timer_CONTROL3_PTR   ( (reg8 *) Poll_Timer_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define Poll_Timer_GLOBAL_ENABLE  (*(reg8 *) Poll_Timer_TimerHW__PM_ACT_CFG)
    #define Poll_Timer_GLOBAL_STBY_ENABLE  (*(reg8 *) Poll_Timer_TimerHW__PM_STBY_CFG)

    #define Poll_Timer_CAPTURE_LSB         (* (reg16 *) Poll_Timer_TimerHW__CAP0 )
    #define Poll_Timer_CAPTURE_LSB_PTR       ((reg16 *) Poll_Timer_TimerHW__CAP0 )
    #define Poll_Timer_PERIOD_LSB          (* (reg16 *) Poll_Timer_TimerHW__PER0 )
    #define Poll_Timer_PERIOD_LSB_PTR        ((reg16 *) Poll_Timer_TimerHW__PER0 )
    #define Poll_Timer_COUNTER_LSB         (* (reg16 *) Poll_Timer_TimerHW__CNT_CMP0 )
    #define Poll_Timer_COUNTER_LSB_PTR       ((reg16 *) Poll_Timer_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define Poll_Timer_BLOCK_EN_MASK                     Poll_Timer_TimerHW__PM_ACT_MSK
    #define Poll_Timer_BLOCK_STBY_EN_MASK                Poll_Timer_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define Poll_Timer_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define Poll_Timer_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define Poll_Timer_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define Poll_Timer_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define Poll_Timer_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define Poll_Timer_CTRL_ENABLE                        ((uint8)((uint8)0x01u << Poll_Timer_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Poll_Timer_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define Poll_Timer_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << Poll_Timer_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define Poll_Timer_CTRL_MODE_SHIFT                     0x01u
        #define Poll_Timer_CTRL_MODE_MASK                     ((uint8)((uint8)0x07u << Poll_Timer_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define Poll_Timer_CTRL_RCOD_SHIFT        0x02u
        #define Poll_Timer_CTRL_ENBL_SHIFT        0x00u
        #define Poll_Timer_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define Poll_Timer_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << Poll_Timer_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define Poll_Timer_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << Poll_Timer_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define Poll_Timer_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << Poll_Timer_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define Poll_Timer_CTRL_RCOD       ((uint8)((uint8)0x03u << Poll_Timer_CTRL_RCOD_SHIFT))
        #define Poll_Timer_CTRL_ENBL       ((uint8)((uint8)0x80u << Poll_Timer_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define Poll_Timer_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define Poll_Timer_RT1_MASK                        ((uint8)((uint8)0x03u << Poll_Timer_RT1_SHIFT))
    #define Poll_Timer_SYNC                            ((uint8)((uint8)0x03u << Poll_Timer_RT1_SHIFT))
    #define Poll_Timer_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define Poll_Timer_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << Poll_Timer_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define Poll_Timer_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << Poll_Timer_SYNCDSI_SHIFT))

    #define Poll_Timer_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << Poll_Timer_CTRL_MODE_SHIFT))
    #define Poll_Timer_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << Poll_Timer_CTRL_MODE_SHIFT))
    #define Poll_Timer_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << Poll_Timer_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Poll_Timer_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Poll_Timer_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Poll_Timer_STATUS_TC_INT_MASK_SHIFT        (Poll_Timer_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Poll_Timer_STATUS_CAPTURE_INT_MASK_SHIFT   (Poll_Timer_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define Poll_Timer_STATUS_TC                       ((uint8)((uint8)0x01u << Poll_Timer_STATUS_TC_SHIFT))
    #define Poll_Timer_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << Poll_Timer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define Poll_Timer_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << Poll_Timer_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define Poll_Timer_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << Poll_Timer_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define Poll_Timer_STATUS              (* (reg8 *) Poll_Timer_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define Poll_Timer_STATUS_MASK         (* (reg8 *) Poll_Timer_TimerUDB_rstSts_stsreg__MASK_REG)
    #define Poll_Timer_STATUS_AUX_CTRL     (* (reg8 *) Poll_Timer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define Poll_Timer_CONTROL             (* (reg8 *) Poll_Timer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(Poll_Timer_Resolution <= 8u) /* 8-bit Timer */
        #define Poll_Timer_CAPTURE_LSB         (* (reg8 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__F0_REG )
        #define Poll_Timer_CAPTURE_LSB_PTR       ((reg8 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__F0_REG )
        #define Poll_Timer_PERIOD_LSB          (* (reg8 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__D0_REG )
        #define Poll_Timer_PERIOD_LSB_PTR        ((reg8 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__D0_REG )
        #define Poll_Timer_COUNTER_LSB         (* (reg8 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__A0_REG )
        #define Poll_Timer_COUNTER_LSB_PTR       ((reg8 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__A0_REG )
    #elif(Poll_Timer_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define Poll_Timer_CAPTURE_LSB         (* (reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__F0_REG )
            #define Poll_Timer_CAPTURE_LSB_PTR       ((reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__F0_REG )
            #define Poll_Timer_PERIOD_LSB          (* (reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__D0_REG )
            #define Poll_Timer_PERIOD_LSB_PTR        ((reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__D0_REG )
            #define Poll_Timer_COUNTER_LSB         (* (reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__A0_REG )
            #define Poll_Timer_COUNTER_LSB_PTR       ((reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define Poll_Timer_CAPTURE_LSB         (* (reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__16BIT_F0_REG )
            #define Poll_Timer_CAPTURE_LSB_PTR       ((reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__16BIT_F0_REG )
            #define Poll_Timer_PERIOD_LSB          (* (reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__16BIT_D0_REG )
            #define Poll_Timer_PERIOD_LSB_PTR        ((reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__16BIT_D0_REG )
            #define Poll_Timer_COUNTER_LSB         (* (reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__16BIT_A0_REG )
            #define Poll_Timer_COUNTER_LSB_PTR       ((reg16 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(Poll_Timer_Resolution <= 24u)/* 24-bit Timer */
        #define Poll_Timer_CAPTURE_LSB         (* (reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__F0_REG )
        #define Poll_Timer_CAPTURE_LSB_PTR       ((reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__F0_REG )
        #define Poll_Timer_PERIOD_LSB          (* (reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__D0_REG )
        #define Poll_Timer_PERIOD_LSB_PTR        ((reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__D0_REG )
        #define Poll_Timer_COUNTER_LSB         (* (reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__A0_REG )
        #define Poll_Timer_COUNTER_LSB_PTR       ((reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define Poll_Timer_CAPTURE_LSB         (* (reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__F0_REG )
            #define Poll_Timer_CAPTURE_LSB_PTR       ((reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__F0_REG )
            #define Poll_Timer_PERIOD_LSB          (* (reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__D0_REG )
            #define Poll_Timer_PERIOD_LSB_PTR        ((reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__D0_REG )
            #define Poll_Timer_COUNTER_LSB         (* (reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__A0_REG )
            #define Poll_Timer_COUNTER_LSB_PTR       ((reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define Poll_Timer_CAPTURE_LSB         (* (reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__32BIT_F0_REG )
            #define Poll_Timer_CAPTURE_LSB_PTR       ((reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__32BIT_F0_REG )
            #define Poll_Timer_PERIOD_LSB          (* (reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__32BIT_D0_REG )
            #define Poll_Timer_PERIOD_LSB_PTR        ((reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__32BIT_D0_REG )
            #define Poll_Timer_COUNTER_LSB         (* (reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__32BIT_A0_REG )
            #define Poll_Timer_COUNTER_LSB_PTR       ((reg32 *) Poll_Timer_TimerUDB_sT24_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #if (Poll_Timer_UsingHWCaptureCounter)
        #define Poll_Timer_CAP_COUNT              (*(reg8 *) Poll_Timer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Poll_Timer_CAP_COUNT_PTR          ( (reg8 *) Poll_Timer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Poll_Timer_CAPTURE_COUNT_CTRL     (*(reg8 *) Poll_Timer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define Poll_Timer_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) Poll_Timer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (Poll_Timer_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define Poll_Timer_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define Poll_Timer_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define Poll_Timer_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define Poll_Timer_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define Poll_Timer_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define Poll_Timer_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << Poll_Timer_CTRL_INTCNT_SHIFT))
    #define Poll_Timer_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << Poll_Timer_CTRL_TRIG_MODE_SHIFT))
    #define Poll_Timer_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << Poll_Timer_CTRL_TRIG_EN_SHIFT))
    #define Poll_Timer_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << Poll_Timer_CTRL_CAP_MODE_SHIFT))
    #define Poll_Timer_CTRL_ENABLE                    ((uint8)((uint8)0x01u << Poll_Timer_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define Poll_Timer_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define Poll_Timer_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define Poll_Timer_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define Poll_Timer_STATUS_TC_INT_MASK_SHIFT       Poll_Timer_STATUS_TC_SHIFT
    #define Poll_Timer_STATUS_CAPTURE_INT_MASK_SHIFT  Poll_Timer_STATUS_CAPTURE_SHIFT
    #define Poll_Timer_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define Poll_Timer_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define Poll_Timer_STATUS_FIFOFULL_INT_MASK_SHIFT Poll_Timer_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define Poll_Timer_STATUS_TC                      ((uint8)((uint8)0x01u << Poll_Timer_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define Poll_Timer_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << Poll_Timer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Poll_Timer_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << Poll_Timer_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Poll_Timer_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << Poll_Timer_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define Poll_Timer_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << Poll_Timer_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define Poll_Timer_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << Poll_Timer_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Poll_Timer_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << Poll_Timer_STATUS_FIFOFULL_SHIFT))

    #define Poll_Timer_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define Poll_Timer_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define Poll_Timer_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define Poll_Timer_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define Poll_Timer_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define Poll_Timer_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_Poll_Timer_H */


/* [] END OF FILE */
