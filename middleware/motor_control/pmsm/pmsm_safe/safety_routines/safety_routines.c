/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_routines.h"
#include "safety_error_handler.h"
#include "safety_flow_check.h"
#include "hardware_cfg.h"

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "iec60730b.h"
#include "iec60730b_core.h"

#include "freemaster_tsa_pmsm.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* ------------------------------- Watchdog --------------------------------- */
/* The expected back-up LPTMR counter reset value. */
#define FS_WDOG_CLK_EXPECTED   ((uint32_t)(((uint64_t)FS_CFG_WDOG_TOUT_VALUE *\
                                (uint64_t)CLOCK_MCGIRCLK_FREQ) / ((uint64_t)CLOCK_LPO_FREQ)))
/* Add watchdog timeout delay compensation. */
#define FS_WDOG_CLK_EXP_COMPENS (FS_WDOG_CLK_EXPECTED + (((uint64_t)CLOCK_MCGIRCLK_FREQ *\
                                (uint64_t)FS_CFG_WDOG_TOUT_RST_DLY) / 1000000ULL))
/* The measured back-up clock tolerance. */
#define FS_WDOG_CLK_TOLERANCE   (FS_WDOG_CLK_EXPECTED * FS_CFG_WDOG_TEST_TOLERANCE / 100UL)
/* Upper measured back-up clock limit. */
#define FS_WDOG_CLK_LIM_HI      ((uint32_t)(FS_WDOG_CLK_EXP_COMPENS + FS_WDOG_CLK_TOLERANCE))
/* Lower measured back-up clock limit. */
#define FS_WDOG_CLK_LIM_LO      ((uint32_t)(FS_WDOG_CLK_EXP_COMPENS - FS_WDOG_CLK_TOLERANCE))

/* ----------------------------- Clock Test --------------------------------- */
/* The expected back-up LPTMR counter reset value. */
#define FS_CLOCK_CLK_EXPECTED   (CLOCK_MCGIRCLK_FREQ / IRQ_SLOW_LOOP_FREQ)
/* The measured back-up clock tolerance. */
#define FS_CLOCK_CLK_TOLERANCE  (FS_CLOCK_CLK_EXPECTED * FS_CFG_CLOCK_TST_TOLERANCE / 100UL)
/* Upper measured back-up clock limit. */
#define FS_CLOCK_CLK_LIM_HI     (FS_CLOCK_CLK_EXPECTED + FS_CLOCK_CLK_TOLERANCE)
/* Lower measured back-up clock limit. */
#define FS_CLOCK_CLK_LIM_LO     (FS_CLOCK_CLK_EXPECTED - FS_CLOCK_CLK_TOLERANCE)

/* ------------------------- ISR Handling Test ------------------------------ */
/* The expected fast-to-slow ISR execution rate. */
#define FS_ISR_FL_PER_SL_EXPECTED   (IRQ_FAST_LOOP_FREQ / IRQ_SLOW_LOOP_FREQ)
/* The configured tolerance of ISR rate execution */
#define FS_ISR_FL_PER_SL_TOLERANCE  (FS_ISR_FL_PER_SL_EXPECTED * FS_CFG_ISR_FL_TO_SL_RATE_TOLERANCE / 100UL)
/* The ISR execution rate maximum. */
#define FS_ISR_FL_PER_SL_EXECS_MAX  (FS_ISR_FL_PER_SL_EXPECTED + FS_ISR_FL_PER_SL_TOLERANCE)
/* The ISR execution rate minimum. */
#define FS_ISR_FL_PER_SL_EXECS_MIN  (FS_ISR_FL_PER_SL_EXPECTED - FS_ISR_FL_PER_SL_TOLERANCE)


/* Function pointer for uninterruptible test function table construction. */
typedef void (*pfsfcn_void_t)(void);


/* Core-dependent IEC60730B Safety Library function assignment. */
#if (defined(__CORTEX_M) && (__CORTEX_M == 0U))
    #define FS_CMn_CPU_Primask()                (FS_CM0_CPU_Primask())
    #define FS_CMn_CPU_SPmain()                 (FS_CM0_CPU_SPmain())
    #define FS_CMn_CPU_Register()               (FS_CM0_CPU_Register())
    #define FS_CMn_CPU_NonStackedRegister()     (FS_CM0_CPU_NonStackedRegister())
    #define FS_CMn_CPU_Control()                (FS_CM0_CPU_Control())
    #define FS_CMn_CPU_SPprocess()              (FS_CM0_CPU_SPprocess())
    #define FS_CMn_PC_Test(a,b,c)               (FS_CM0_PC_Test((a),(b),(c)))
    #define FS_CMn_RAM_AfterReset(a,b,c,d,e)    (FS_CM0_RAM_AfterReset((a),(b),(c),(d),(e)))
    #define FS_CMn_RAM_Runtime(a,b,c,d,e,f)     (FS_CM0_RAM_Runtime((a),(b),(c),(d),(e),(f)))
    #define FS_CMn_STACK_Init(a,b,c,d)          (FS_CM0_STACK_Init((a),(b),(c),(d)))
    #define FS_CMn_STACK_Test(a,b,c,d)          (FS_CM0_STACK_Test((a),(b),(c),(d)))
    #define FS_CMn_FLASH_HW16(a,b,c,d)          (FS_CM0_FLASH_HW16((a),(b),(c),(d)))

    #define FS_CMn_RAM_SegmentMarchX            (FS_CM0_RAM_SegmentMarchX)
    #define FS_CMn_RAM_SegmentMarchC            (FS_CM0_RAM_SegmentMarchC)

    #define FS_CMn_CLK_Init(a)                  (FS_CLK_Init((a)))
    #define FS_CMn_WDOG_Setup(a)                (FS_WDOG_Setup_LPTMR((a), (uint8_t)FS_KINETIS_WDOG))
    #define FS_CMn_WDOG_Check(a,b,c,d,e)        (FS_WDOG_Check((a),(b),(c),(d),(e), 0U, (uint8_t)FS_WDOG_SRS_WIDE_8b))
    #define FS_CMn_CLK_LPTMR(a,b)               (FS_CLK_LPTMR((a),(b)))
    #define FS_CMn_CLK_Check(a,b,c)             (FS_CLK_Check((a),(b),(c)))
    #define FS_CMn_PC_Object                    (FS_PC_Object)
    #define FS_CMn_WDOG_MemInit(a)              (a)->resets = 0U;\
                                                (a)->RefTimerBase = (uint32_t)LPTMR0;\
                                                (a)->WdogBase = (uint32_t)WDOG;\
                                                (a)->pResetDetectRegister = (uint32_t)(&(RCM->SRS0));\
                                                (a)->ResetDetectMask  = (uint32_t)RCM_SRS0_WDOG_MASK


    typedef  fs_wdog_test_t                      fs_wdog_reset_test_t;

#elif (defined(__CORTEX_M) && (__CORTEX_M == 4U))
    #define FS_CMn_CPU_Primask()                (FS_CM4_CM7_CPU_Primask())
    #define FS_CMn_CPU_SPmain()                 (FS_CM4_CM7_CPU_SPmain())
    #define FS_CMn_CPU_Register()               (FS_CM4_CM7_CPU_Register())
    #define FS_CMn_CPU_NonStackedRegister()     (FS_CM4_CM7_CPU_NonStackedRegister())
    #define FS_CMn_CPU_Control()                (FS_CM4_CM7_CPU_Control())
    #define FS_CMn_CPU_SPprocess()              (FS_CM4_CM7_CPU_SPprocess())
    #define FS_CMn_PC_Test(a,b,c)               (FS_CM4_CM7_PC_Test((a),(b),(c)))
    #define FS_CMn_RAM_AfterReset(a,b,c,d,e)    (FS_CM4_CM7_RAM_AfterReset((a),(b),(c),(d),(e)))
    #define FS_CMn_RAM_Runtime(a,b,c,d,e,f)     (FS_CM4_CM7_RAM_Runtime((a),(b),(c),(d),(e),(f)))
    #define FS_CMn_STACK_Init(a,b,c,d)          (FS_CM4_CM7_STACK_Init((a),(b),(c),(d)))
    #define FS_CMn_STACK_Test(a,b,c,d)          (FS_CM4_CM7_STACK_Test((a),(b),(c),(d)))
    #define FS_CMn_FLASH_HW16(a,b,c,d)          (FS_CM4_CM7_FLASH_HW16((a),(b),(c),(d)))

    #define FS_CMn_RAM_SegmentMarchX            (FS_CM4_CM7_RAM_SegmentMarchX)
    #define FS_CMn_RAM_SegmentMarchC            (FS_CM4_CM7_RAM_SegmentMarchC)

    #define FS_CMn_CLK_Init(a)                  (FS_CLK_Init((a)))
    #define FS_CMn_WDOG_Setup(a)                (FS_WDOG_Setup_LPTMR((a), (uint8_t)FS_KINETIS_WDOG))
    #define FS_CMn_WDOG_Check(a,b,c,d,e)        (FS_WDOG_Check((a),(b),(c),(d),(e), 0U, (uint8_t)FS_WDOG_SRS_WIDE_8b))
    #define FS_CMn_CLK_LPTMR(a,b)               (FS_CLK_LPTMR((a),(b)))
    #define FS_CMn_CLK_Check(a,b,c)             (FS_CLK_Check((a),(b),(c)))
    #define FS_CMn_PC_Object                    (FS_PC_Object)
    #define FS_CMn_WDOG_MemInit(a)              (a)->resets = 0U;\
                                                (a)->RefTimerBase = (uint32_t)LPTMR0;\
                                                (a)->WdogBase = (uint32_t)WDOG;\
                                                (a)->pResetDetectRegister = (uint32_t)(&(RCM->SRS0));\
                                                (a)->ResetDetectMask  = (uint32_t)RCM_SRS0_WDOG_MASK

    typedef fs_wdog_test_t                      fs_wdog_reset_test_t;

#else
    #error "Unsupported core!"
#endif

/*  Safety tests structure. See c_fsFcnRuntimeTests table for more details. */
typedef enum _fs_noirq_test
{
    kFS_CpuIsrTest   = 0U,
    kFS_PcTest       = 1U,
    kFS_SafeRamTest  = 2U,
    kFS_StackRamTest = 3U,
    kFS_WdogRefresh  = 4U,
    kFS_NoIrqTstNum  = 5U
} fs_noirq_test_t;

/*! @brief The linker command file configuration table. */
typedef struct _lcf_setup
{
    uint32_t  ui32RAMStart;          /*< The address of the beginning of the RAM memory. */
    uint32_t  ui32RAMEnd;            /*< The address of the end of the RAM memory. */
    uint32_t  ui32FlashStart;        /*< The address of the beginning of the Flash memory. */
    uint32_t  ui32FlashEnd;          /*< The address of the end of the Flash memory. */

    uint32_t  ui32RAMSafeStart;      /*< Safety-related RAM start address. */
    uint32_t  ui32RAMSafeEnd;        /*< Safety-related RAM end address. */

    uint32_t  ui32FlashSafeStart;    /*< Safety-related FLASH start address. */
    uint32_t  ui32FlashSafeEnd;      /*< Safety-related FLASH end address. */
    volatile uint16_t *pui16FlashSafeCRC; /*< Safety-related FLASH CRC address. */

    uint32_t  ui32StackStart;        /*< Stack RAM start address. */
    uint32_t  ui32StackEnd;          /*< Stack RAM end address. */
    uint32_t  ui32StackTstBlckSize;  /*< Stack RAM test block size. */

    uint32_t  ui32RAMBackupStart;    /*< RAM test back-up address. */
    uint32_t  ui32RAMBackupSize;     /*< RAM test back-up size. */
    uint32_t  ui32RAMTstBlckSize;    /*< Safety RAM run-time test block size. */

    uint32_t  *pui32PCTestFlag;      /*< Program counter test flag. */
    uint32_t  *pui32SafetyErrorAddr; /*< Safety error code*/

    fs_wdog_reset_test_t *psWDGTstVarsAddr;  /*< Watchdog test backup address. */

} lcf_setup_t;


/*! @brief Safety tests global variable structure. */
typedef struct _fs_test_common
{
    /* The last error code value. */
    uint32_t   ui32LastErrCode;

    /* The result code of the last failed test. */
    uint32_t   ui32LastFailedTstRslt;

    struct  /*!< Safety Clock test structure. */
    {
        uint32_t ui32TestContext;     /* Internal context variable. */
        uint16_t ui16TestStartFlag;   /* The first ISR activation flag. */
    } sClock;

    struct  /*!< Safety RAM test */
    {
        uint32_t ui32TestBlockAddrStack; /* Backup RAM area address for RAM run-time test. */
        uint32_t ui32TestBlockAddrRam;   /* Backup RAM area address for Stack run-time test. */
    } sRAM;

    struct  /*!< Safety Flash test structure. */
    {
        /* The actual start address of investigated flash block for the CRC module. */
        uint32_t ui32TestBlockAddr;
        /* CRC seed at the test start, partial result in process, and CRC result
           once the test finishes.*/
        uint16_t ui16PartialCRC;
    } sFLASH;

    struct  /*!< Safety Watchdog test structure. */
    {
        /* Watchdog fast control loop refresh ratio. */
        uint16_t ui16RefreshRatio;
        /* Number of resets. */
        uint16_t ui16Resets;
    } sWDOG;

    struct  /*!< Interrupt handling test structure. */
    {
        uint32_t ui32CtrSlowISR;  /* Slow loop ISR execution counter. */
        uint32_t ui32CtrFastISR;  /* Fast loop ISR execution counter. */
    } sISR;

    /* Fast control loop safety test selector. */
    fs_noirq_test_t  eNoIrqTest;

} fs_test_common_t;

#if defined(DEBUG)
/*! @brief Safety tests debugging and development structure. */
typedef struct _fs_test_dbg
{
     struct  /*!< Safety RAM test */
     {
        uint16_t ui16ScanTime_Ram;    /* Number of ISR cycles to scan entire safety RAM. */
        uint16_t ui16CycleLast_Ram;   /* The last value of cycle counter. */
        uint16_t ui16ScanTime_Stack;  /* Number of ISR cycles to scan entire stack. */
        uint16_t ui16CycleLast_Stack; /* The last value of cycle counter. */
     } sRAM;

     struct  /*!< Safety Flash test */
     {
        uint16_t ui16ScanTime;    /* Number of ISR cycles to scan entire safety FLASH. */
        uint16_t ui16CycleLast;   /* The last value of cycle counter. */
     } sFLASH;

     uint16_t ui16CycleCtr;
} fs_test_dbg_t;
#endif /* defined(DEBUG) */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief   Uninterruptible test of CPU registers.
 *
 * @details This function calls the CPU test functions from IEC60730 library.
 *          The function must be called from an interrupt with the highest
 *          priority. The errors are handled as follows:
 *          -MSP: Stuck in the endless loop (WDOG reset is necessary):
 *          -PRIMASK: Safety error handler is entered:
 *
 * @param   void
 *
 * @return  none
 */
static void FS_fsCpuTest_FL(void);

/*!
 * @brief   Program counter run-time test.
 *
 * @details This function uses two addresses: the first is the address of
 *          the iec60730b_cm0_pc_object.o  object, which is defined in linker
 *          file, and the second address comes as function argument (must be RAM
 *          address). Both addresses must be defined by the developer and
 *          suitable to test all of the possible PC bits. This test can?t be
 *          interrupted. In case of incorrect test result, the safety error
 *          handler FS_ErrorHandling() is called.
 *
 * @param   void
 *
 * @return  none
 */
static void FS_fsPcTest_FL(void);

/*!
 * @brief   Run-time test of safety RAM.
 *
 * @details This function calls the RAM test function from IEC60730 library.
 *          Safety-related part of the RAM is tested in sequence. In case of
 *          incorrect RAM test result, the safety error handler
 *          FS_ErrorHandling() is called.
 *
 * @param   void
 *
 * @return  none
 */
static void FS_fsRamTestSaRam_FL(void);

/*!
 * @brief   Run-time test of Stack RAM.
 *
 * @details This function calls the RAM test function from IEC60730 library.
 *          Stack is tested in sequence. In case of incorrect RAM test result,
 *          the safety error handler FS_ErrorHandling() is called.
 *
 * @return  none
 */
static void FS_fsRamTestStack_FL(void);

/*!
 * @brief   Safety watchdog runtime refresh.
 *
 * @details This function is used for adjusting of the watchdog refresh when
 *          using in the fast interrupt routine.
 *
 * @note    Refreshes the watchdog.
 *
 * @return  none
 */
static void FS_fsWdogRefresh_FL(void);



/*******************************************************************************
 * Variables
 ******************************************************************************/


/* Compiler-dependent initialization. */
#if defined(__IAR_SYSTEMS_ICC__) /* IAR */

    #pragma section = ".safety_ram"

/* ------------ Import symbols from linker-command file. -------------------- */
    /* Get the address of the beginning and the end of the RAM memory. */
    extern uint32_t __RAM_start__;
    extern uint32_t __RAM_end__;

    /* Get the address of the beginning and the end of the Flash memory. */
    extern uint32_t __ROM_start__;
    extern uint32_t __ROM_end__;

    /* Get the address of the beginning and the end of the stack. */
    extern uint32_t m_fs_stack_tst_p2;
    extern uint32_t m_fs_stack_tst_p3;
    extern uint32_t __size_fs_stack_tst_p__;

    /* Back-up RAM address. */
    extern uint32_t m_fs_ram_tst_bckp;
    extern uint32_t __size_fs_ram_tst_bckp__;

    /* Watchdog test structure. */
    extern uint32_t m_fs_wdog_tst_bckp;

    /* Program counter register test flag. */
    extern uint32_t m_fs_pc_tst_flag;

    /* Safety error code. */
    extern uint32_t m_fs_error_code;

    /* The safety-related FLASH boundaries and CRC. */
    __root extern uint16_t __checksum;
    __root const uint32_t c_ui32fsSafetyFlashEnd   @ "m_safety_flash_end";

    /* Assemble the linker configuration structure. */
    const lcf_setup_t c_sfsLCFSetup =
    {
        .ui32RAMStart         = (uint32_t)&__RAM_start__,
        .ui32RAMEnd           = (uint32_t)&__RAM_end__,
        .ui32FlashStart       = (uint32_t)&__ROM_start__,
        .ui32FlashEnd         = (uint32_t)&__ROM_end__,
        .ui32RAMSafeStart     = (uint32_t)__section_begin(".safety_ram"),
        .ui32RAMSafeEnd       = (uint32_t)__section_end(".safety_ram"),
        .ui32FlashSafeStart   = (uint32_t)&__ROM_start__,
        .ui32FlashSafeEnd     = ((uint32_t)&c_ui32fsSafetyFlashEnd) + 4UL,
        .pui16FlashSafeCRC    = (volatile uint16_t*)(&__checksum),
        .ui32StackStart       = (uint32_t)&m_fs_stack_tst_p2,
        .ui32StackEnd         = (uint32_t)&m_fs_stack_tst_p3,
        .ui32StackTstBlckSize = (uint32_t)&__size_fs_stack_tst_p__,
        .ui32RAMBackupStart   = (uint32_t)&m_fs_ram_tst_bckp,
        .ui32RAMBackupSize    = (uint32_t)&__size_fs_ram_tst_bckp__,
        .ui32RAMTstBlckSize   = FS_CFG_RAM_TEST_BLOCK_SIZE,
        .pui32PCTestFlag      = (uint32_t*)&m_fs_pc_tst_flag,
        .pui32SafetyErrorAddr = (uint32_t*)&m_fs_error_code,
        .psWDGTstVarsAddr     = (fs_wdog_reset_test_t*)&m_fs_wdog_tst_bckp
    };

    /* The safety test variable structure. */
    static fs_test_common_t  g_sfsSafetyTst @ ".safety_ram";

    /* The software flow check variables. */
    fs_flow_check_t g_sfsFlowChkAR @ ".safety_ram";
    fs_flow_check_t g_sfsFlowChkBL @ ".safety_ram";
    fs_flow_check_t g_sfsFlowChkFL @ ".safety_ram";
    fs_flow_check_t g_sfsFlowChkSL @ ".safety_ram";

#elif (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* KEIL */

    #include "linker_config.h"

    /*! @brief CRC structure containing information for the offline CRC calculation. */
    typedef struct _fs_crc
    {
        uint16_t ui16Start;
        uint32_t ui32FlashStart __attribute__((packed));
        uint32_t ui32FlashEnd   __attribute__((packed));
        uint32_t ui32CRC        __attribute__((packed));
        uint16_t ui16End        __attribute__((packed));
    } fs_crc_t;

    /* The safety-related RAM border marker. */
    extern uint32_t Image$$ER_FS_RAM$$Limit;

    /* The safety-related FLASH end border marker. */
    extern uint32_t Load$$ER_FS_RAM$$Limit;

    /* The safety-related FLASH CRC value. */
    const fs_crc_t c_sfsCRC __attribute__((used, section(".flshcrc"))) =
    {
        .ui16Start      = 0xA55AU,
        .ui32FlashStart = (uint32_t)__ROM_start__,
        .ui32FlashEnd   = (uint32_t)&Load$$ER_FS_RAM$$Limit,
        .ui32CRC        = (uint32_t)FS_CFG_FLASH_TST_CRC,
        .ui16End        = 0x5AA5U
    };

    /* Assemble the linker configuration structure. */
    const lcf_setup_t __attribute__((used)) c_sfsLCFSetup =
    {
        .ui32RAMStart         = (uint32_t)__RAM_start__,
        .ui32RAMEnd           = (uint32_t)__RAM_end__,
        .ui32FlashStart       = (uint32_t)__ROM_start__,
        .ui32FlashEnd         = (uint32_t)__ROM_end__,
        .ui32RAMSafeStart     = (uint32_t)m_data_start,
        .ui32RAMSafeEnd       = (uint32_t)&Image$$ER_FS_RAM$$Limit,
        .ui32FlashSafeStart   = (uint32_t)__ROM_start__,
        .ui32FlashSafeEnd     = (uint32_t)&Load$$ER_FS_RAM$$Limit,
        .pui16FlashSafeCRC    = (volatile uint16_t*)&(c_sfsCRC.ui32CRC),
        .ui32StackStart       = (uint32_t)m_fs_stack_tst_p2,
        .ui32StackEnd         = (uint32_t)m_fs_stack_tst_p3,
        .ui32StackTstBlckSize = (uint32_t)__size_fs_stack_tst_p__,
        .ui32RAMBackupStart   = (uint32_t)m_fs_ram_tst_bckp,
        .ui32RAMBackupSize    = (uint32_t)__size_fs_ram_tst_bckp__,
        .ui32RAMTstBlckSize   = FS_CFG_RAM_TEST_BLOCK_SIZE,
        .pui32PCTestFlag      = (uint32_t*)m_fs_pc_tst_flag,
        .pui32SafetyErrorAddr = (uint32_t*)m_fs_error_code,
        .psWDGTstVarsAddr     = (fs_wdog_reset_test_t*)m_fs_wdog_tst_bckp

    };

    /* The safety test variables. */
    static fs_test_common_t g_sfsSafetyTst __attribute__((section(".safety_ram")));

    /* The software flow check variables. */
    fs_flow_check_t g_sfsFlowChkAR __attribute__((section(".safety_ram")));
    fs_flow_check_t g_sfsFlowChkBL __attribute__((section(".safety_ram")));
    fs_flow_check_t g_sfsFlowChkFL __attribute__((section(".safety_ram")));
    fs_flow_check_t g_sfsFlowChkSL __attribute__((section(".safety_ram")));

#elif defined(__GNUC__) /* MCUXpresso */

/* ------------ Import symbols from linker-command file. -------------------- */

    /*! @brief CRC structure containing information for the offline CRC calculation. */
    typedef struct _fs_crc
    {
        uint16_t ui16Start;
        uint32_t ui32FlashStart __attribute__((packed));
        uint32_t ui32FlashEnd   __attribute__((packed));
        uint32_t ui32CRC        __attribute__((packed));
        uint16_t ui16End        __attribute__((packed));
    } fs_crc_t;

    /* Get the address of the beginning and the end of the RAM memory. */
    extern uint32_t __RAM_start__;
    extern uint32_t __RAM_end__;

    /* Get the address of the beginning and the end of the Flash memory. */
    extern uint32_t __ROM_start__;
    extern uint32_t __ROM_end__;

    /* Get the address of the beginning and the end of the stack. */
    extern uint32_t m_fs_stack_tst_p2;
    extern uint32_t m_fs_stack_tst_p3;

    /* Stack test block size.  */
    extern uint32_t __size_fs_stack_tst_p__;

    /* Safety RAM. */
    extern uint32_t m_sec_fs_ram_start;
    extern uint32_t m_sec_fs_ram_end;

    /* Watchdog test structure. */
    extern uint32_t m_fs_wdog_tst_bckp;

    /* Program counter test flag. */
    extern uint32_t m_fs_pc_tst_flag;

    /* Safety error code. */
    extern uint32_t m_fs_error_code;

    /* RAM test backup address and size. */
    extern uint32_t m_fs_ram_tst_bckp;
    extern uint32_t __size_fs_ram_tst_bckp__;

    /* Safety-related FLASH boundaries. */
    extern uint32_t m_safety_flash_end;

    /* The safety-related FLASH CRC value. */
    const fs_crc_t c_sfsCRC __attribute__((used, section(".flshcrc"))) =
    {
        .ui16Start      = 0xA55AU,
        .ui32FlashStart = (uint32_t)&__ROM_start__,
        .ui32FlashEnd   = (uint32_t)&m_safety_flash_end,
        .ui32CRC        = (uint32_t)FS_CFG_FLASH_TST_CRC,
        .ui16End        = 0x5AA5U
    };
    /* Assemble the linker configuration structure. */
    const lcf_setup_t __attribute__((used)) c_sfsLCFSetup =
    {
        .ui32RAMStart         = (uint32_t)&__RAM_start__,
        .ui32RAMEnd           = (uint32_t)&__RAM_end__,
        .ui32FlashStart       = (uint32_t)&__ROM_start__,
        .ui32FlashEnd         = (uint32_t)&__ROM_end__,
        .ui32RAMSafeStart     = (uint32_t)&m_sec_fs_ram_start,
        .ui32RAMSafeEnd       = (uint32_t)&m_sec_fs_ram_end,
        .ui32FlashSafeStart   = (uint32_t)&__ROM_start__,
        .ui32FlashSafeEnd     = (uint32_t)&m_safety_flash_end,
        .pui16FlashSafeCRC    = (volatile uint16_t*)&(c_sfsCRC.ui32CRC),
        .ui32StackStart       = (uint32_t)&m_fs_stack_tst_p2,
        .ui32StackEnd         = (uint32_t)&m_fs_stack_tst_p3,
        .ui32StackTstBlckSize = (uint32_t)&__size_fs_stack_tst_p__,
        .ui32RAMBackupStart   = (uint32_t)&m_fs_ram_tst_bckp,
        .ui32RAMBackupSize    = (uint32_t)&__size_fs_ram_tst_bckp__,
        .ui32RAMTstBlckSize   = FS_CFG_RAM_TEST_BLOCK_SIZE,
        .pui32PCTestFlag      = (uint32_t*)&m_fs_pc_tst_flag,
        .pui32SafetyErrorAddr = (uint32_t*)&m_fs_error_code,
        .psWDGTstVarsAddr     = (fs_wdog_reset_test_t*)&m_fs_wdog_tst_bckp
    };

    /* The safety test variables. */
    static fs_test_common_t g_sfsSafetyTst __attribute__((section(".safety_ram")));

    /* The software flow check variables. */
    fs_flow_check_t g_sfsFlowChkAR __attribute__((section(".safety_ram")));
    fs_flow_check_t g_sfsFlowChkBL __attribute__((section(".safety_ram")));
    fs_flow_check_t g_sfsFlowChkFL __attribute__((section(".safety_ram")));
    fs_flow_check_t g_sfsFlowChkSL __attribute__((section(".safety_ram")));


#else
    #error "Unsupported compiler!"
#endif /* defined(__IAR_SYSTEMS_ICC__) */

#if defined(DEBUG)
    /* Place the safety debugging structure into non-safety portion of RAM. */
    static fs_test_dbg_t  g_sSafetyDbg;
#endif /* defined(DEBUG) */

/* Table of run-time test functions. */
const pfsfcn_void_t c_fsFcnRuntimeTests[kFS_NoIrqTstNum] =
{
    FS_fsCpuTest_FL,       /* Un-interruptible core register tests. */
    FS_fsPcTest_FL,        /* Program counter test. */
    FS_fsRamTestSaRam_FL,  /* Safety-related RAM test. */
    FS_fsRamTestStack_FL,  /* Stack RAM test. */
    FS_fsWdogRefresh_FL    /* Run-time watchdog refresh. */
};


/*!
 * @brief   The safety variables TSA table.
 *
 * @details See freemaster_tsa.h for more details about TSA table construction.
 *
 * @param   TSA table identificator.
 *
 * @return  FMSTR_TSA_ENTRY* - The TSA table entry data type.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
FMSTR_TSA_TABLE_BEGIN_FS(TSA_SAFETY)
    /* The last error code (not cleared by reset). */
    FMSTR_TSA_RO_VAR_FS(S01, g_sfsSafetyTst.ui32LastErrCode,   FMSTR_TSA_UINT32)
    FMSTR_TSA_RO_VAR_FS(S02, g_sfsSafetyTst.sWDOG.ui16Resets,  FMSTR_TSA_UINT16)
#if defined(DEBUG)
    FMSTR_TSA_RW_VAR_FS(S03, g_sSafetyDbg.sRAM.ui16ScanTime_Ram,   FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR_FS(S04, g_sSafetyDbg.sRAM.ui16ScanTime_Stack, FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR_FS(S05, g_sSafetyDbg.sFLASH.ui16ScanTime,     FMSTR_TSA_UINT16)
#endif /* defined(DEBUG) */
FMSTR_TSA_TABLE_END_FS(TSA_SAFETY)
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * Code - functions with local scope
 ******************************************************************************/
/*                         RUNTIME CPU REGISTER TEST                          */
/*!
 * CPU REGISTERS uninterruptible test.
 */
static void FS_fsCpuTest_FL(void)
{
    uint32_t ui32TstResult; /* Test result return code. */

    /* Test the Priority Mask (PRIMASK) register. */
    ui32TstResult = FS_CMn_CPU_Primask();
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_CPU_PRIMASK, ui32TstResult);
    }

    /* Test the Main Stack Pointer (MSP) register. */
    ui32TstResult = FS_CMn_CPU_SPmain();
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_CPU_MSP, ui32TstResult);
    }

    /* Continue to the next uninterruptible test. */
    g_sfsSafetyTst.eNoIrqTest = kFS_PcTest;

    /* The safety uninterruptible tests node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_FSTST_UNINTERR);
}

/*                           RUN-TIME RAM TESTS                               */
/*!
 * SAFETY RAM run-time test.
 */
static void FS_fsRamTestSaRam_FL(void)
{
    uint32_t ui32TstResult; /* Test result return code. */

#if defined(DEBUG)
    /* Store the last RAM block address. */
    uint32_t ui32Addr = g_sfsSafetyTst.sRAM.ui32TestBlockAddrRam;
#endif /* defined(DEBUG) */

    /* Perform the test of the single RAM segment. */
    ui32TstResult = FS_CMn_RAM_Runtime(c_sfsLCFSetup.ui32RAMSafeStart,
                                       c_sfsLCFSetup.ui32RAMSafeEnd,
                                      &g_sfsSafetyTst.sRAM.ui32TestBlockAddrRam,
                                       c_sfsLCFSetup.ui32RAMTstBlckSize,
                                       c_sfsLCFSetup.ui32RAMBackupStart,
                                       FS_CMn_RAM_SegmentMarchX);

    /* Check the RAM test results. */
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_RAM_SAFETY_TEST, ui32TstResult);
    }

#if defined(DEBUG)
    /* Check whether the complete safety RAM section was scanned. */
    if(ui32Addr > g_sfsSafetyTst.sRAM.ui32TestBlockAddrRam)
    {
        /* Calculate the number of fast-loop cycles it took to scan the whole
           safety RAM. */
        g_sSafetyDbg.sRAM.ui16ScanTime_Ram =
            g_sSafetyDbg.ui16CycleCtr - g_sSafetyDbg.sRAM.ui16CycleLast_Ram;

        /* Store the number of cycles. */
        g_sSafetyDbg.sRAM.ui16CycleLast_Ram = g_sSafetyDbg.ui16CycleCtr;
    }
#endif /* defined(DEBUG) */


    /* Continue to the next uninterruptible test. */
    g_sfsSafetyTst.eNoIrqTest = kFS_StackRamTest;

    /* The safety uninterruptible tests node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_FSTST_UNINTERR);
}


/*!
 * STACK RAM run-time test.
 */
static void FS_fsRamTestStack_FL(void)
{
    uint32_t ui32TstResult; /* Test result return code. */

#if defined(DEBUG)
    /* Store the last RAM block address. */
    uint32_t ui32Addr = g_sfsSafetyTst.sRAM.ui32TestBlockAddrStack;
#endif /* defined(DEBUG) */

    /* Perform the test of the single Stack RAM segment. */
    ui32TstResult = FS_CMn_RAM_Runtime(c_sfsLCFSetup.ui32StackStart,
                                       c_sfsLCFSetup.ui32StackEnd,
                                      &g_sfsSafetyTst.sRAM.ui32TestBlockAddrStack,
                                       c_sfsLCFSetup.ui32RAMTstBlckSize,
                                       c_sfsLCFSetup.ui32RAMBackupStart,
                                       FS_CMn_RAM_SegmentMarchX);

    /* Check the Stack RAM test results. */
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_RAM_STACK_TEST, ui32TstResult);
    }

#if defined(DEBUG)
    /* Check whether the complete safety RAM section was scanned. */
    if(ui32Addr > g_sfsSafetyTst.sRAM.ui32TestBlockAddrStack)
    {
        /* Calculate the number of fast-loop cycles it took to scan the whole
           stack. */
        g_sSafetyDbg.sRAM.ui16ScanTime_Stack =
            g_sSafetyDbg.ui16CycleCtr - g_sSafetyDbg.sRAM.ui16CycleLast_Stack;

        /* Store the number of cycles. */
        g_sSafetyDbg.sRAM.ui16CycleLast_Stack = g_sSafetyDbg.ui16CycleCtr;
    }
#endif /* defined(DEBUG) */

    /* Continue to the next uninterruptible test. */
    g_sfsSafetyTst.eNoIrqTest = kFS_WdogRefresh;

    /* The safety uninterruptible tests node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_FSTST_UNINTERR);
}

/*                      WATCHDOG RUN-TIME REFRESH                             */
/*!
 * WATCHDOG run-time refresh.
 */
static void FS_fsWdogRefresh_FL(void)
{
#if FS_CFG_ENABLE_WATCHDOG

    /* Check whether the required number of the fast control loops was
       reached. */
    if(g_sfsSafetyTst.sWDOG.ui16RefreshRatio >= FS_CFG_WDOG_REFRESH_RATIO)
    {
        /* Refresh the watchdog. */
        FS_WDOG_REFRESH();

        /* Reset the fast control loop counter. */
        g_sfsSafetyTst.sWDOG.ui16RefreshRatio = (uint16_t)kFS_NoIrqTstNum;
    }
    else
    {
        /* The WDOG refresh threshold not yet reached - increment the fast
           control loop counter. */
        g_sfsSafetyTst.sWDOG.ui16RefreshRatio += (uint16_t)kFS_NoIrqTstNum;
    }

 #endif /* FS_CFG_ENABLE_WATCHDOG */

    /* Continue to the next uninterruptible test. */
    g_sfsSafetyTst.eNoIrqTest = kFS_CpuIsrTest;

    /* The safety uninterruptible tests node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_FSTST_UNINTERR);
}

/*******************************************************************************
 * Code - functions with global scope
 ******************************************************************************/

/*!
 * Common safety initialization function and watchdog enable/disable.
 */
void FS_fsCommonInit_AR(void)
{
    uint8_t ui8ResetCondition;

    /* Refresh watchdog. */
    FS_WDOG_REFRESH();

    /* Check the cause of reset. */
    ui8ResetCondition  = (RCM->SRS0 & (RCM_SRS0_POR_MASK | RCM_SRS0_PIN_MASK));
    ui8ResetCondition |= (RCM->SRS1 &  RCM_SRS1_SW_MASK);

    /* Check whether power-on, pin or SW reset occured. */
    if(ui8ResetCondition != 0U)
    {
        /* Reset the error code. */
        *c_sfsLCFSetup.pui32SafetyErrorAddr = 0UL;

        /* Clear the WDOG backup memory. */
        FS_CMn_WDOG_MemInit(c_sfsLCFSetup.psWDGTstVarsAddr);
    }

    /* Get the last error code. */
    g_sfsSafetyTst.ui32LastErrCode = *c_sfsLCFSetup.pui32SafetyErrorAddr;
    g_sfsSafetyTst.sWDOG.ui16Resets = (uint16_t)c_sfsLCFSetup.psWDGTstVarsAddr->resets;

    /* Store the test RAM runtime block address. */
    g_sfsSafetyTst.sRAM.ui32TestBlockAddrStack = c_sfsLCFSetup.ui32StackStart;
    g_sfsSafetyTst.sRAM.ui32TestBlockAddrRam   = c_sfsLCFSetup.ui32RAMSafeStart;

    /* Flash test initialization. */
#if FS_CFG_ENABLE_TEST_FLASH

    /* Initialize CRC runtime calculation variable with the initial CRC seed. */
    g_sfsSafetyTst.sFLASH.ui16PartialCRC = 0U;

    /* Flash test block initial address and size. */
    g_sfsSafetyTst.sFLASH.ui32TestBlockAddr = c_sfsLCFSetup.ui32FlashSafeStart;

#endif /* FS_CFG_ENABLE_TEST_FLASH */


    /* Stack test initialization. */
    /* Initialize areas below and above stack, and prepare following stack
       underflow/overflow test. */
    FS_CMn_STACK_Init(FS_CFG_STACK_TEST_PATTERN,
                      c_sfsLCFSetup.ui32StackStart,
                      c_sfsLCFSetup.ui32StackEnd,
                      c_sfsLCFSetup.ui32StackTstBlckSize);

    /* Clock test initialization */
#if FS_CFG_ENABLE_TEST_CLOCK

    /* Disable the clock test for now. The test result will be processed
       after the second interrupt occurs. */
    g_sfsSafetyTst.sClock.ui16TestStartFlag = 0U;

    /* Initialize the clock test. */
    FS_CMn_CLK_Init((uint32_t*)&g_sfsSafetyTst.sClock.ui32TestContext);

#endif /* FS_CFG_ENABLE_TEST_CLOCK */

    /* Clear the safety ISR execution test counters. */
    g_sfsSafetyTst.sISR.ui32CtrSlowISR = 0UL;
    g_sfsSafetyTst.sISR.ui32CtrFastISR = 0UL;

    /* Common safety routine init node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_FSTEST_INIT);
}

/*!
 * Uninterruptible safety test selector function.
 */
void FS_fsUninterruptibleTests_FL(void)
{
    /* Execute the run-time test. */
    c_fsFcnRuntimeTests[g_sfsSafetyTst.eNoIrqTest]();

#if defined(DEBUG)
    /* Increment the cycle counter. */
    g_sSafetyDbg.ui16CycleCtr++;
#endif /* defined(DEBUG) */
}

/*!
 * Safety error handler.
 */
void FS_fsErrorHandling(uint32_t ui32ErrCode, uint32_t ui32TestResult)
{
#if defined(DEBUG) && FS_CFG_SAFETY_ERROR_ACTION
    uint64_t ui64ErrCodeCtr;  /* The USER_LED_2 blinking counter. */
    uint32_t ui32Timer;       /* The USER_LED_2 blinking timer. */
    uint32_t ui32WdogTimer;   /* Watchdog refresh timer. */
#endif /* defined(DEBUG) && FS_CFG_SAFETY_ERROR_ACTION */

    /* Store the error code */
    *c_sfsLCFSetup.pui32SafetyErrorAddr |= ui32ErrCode;
    g_sfsSafetyTst.ui32LastFailedTstRslt = ui32TestResult;

#if FS_CFG_SAFETY_ERROR_ACTION

    /* Disable interrupts. */
    __asm("CPSID i");

    /* Enable clocks only for GPIO pins controlling the PWM and DC-bus brake
       outputs. */
    SIM->SCGC4 = 0UL;
    SIM->SCGC5 = (SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK |
                  SIM_SCGC5_PORTE_MASK);
    SIM->SCGC6 = SIM_SCGC6_FTM0_MASK;
    SIM->SCGC7 = 0UL;

    /* Make sure that the inactive FTM output polarity is set to low. */
    FTM0->POL = 0U;

    /* Disable the PWMs. */
    FTM0->OUTMASK = (FTM_OUTMASK_CH0OM_MASK | FTM_OUTMASK_CH1OM_MASK |
                     FTM_OUTMASK_CH2OM_MASK | FTM_OUTMASK_CH3OM_MASK |
                     FTM_OUTMASK_CH4OM_MASK | FTM_OUTMASK_CH5OM_MASK);

    /* Configure PWM FTM0 output ports. */
    PIN_PWM0_PORT->PCR[PIN_PWM0_PIN] = PORT_PCR_MUX(PIN_PWM0_MUX);
    PIN_PWM1_PORT->PCR[PIN_PWM1_PIN] = PORT_PCR_MUX(PIN_PWM1_MUX);
    PIN_PWM2_PORT->PCR[PIN_PWM2_PIN] = PORT_PCR_MUX(PIN_PWM2_MUX);
    PIN_PWM3_PORT->PCR[PIN_PWM3_PIN] = PORT_PCR_MUX(PIN_PWM3_MUX);
    PIN_PWM4_PORT->PCR[PIN_PWM4_PIN] = PORT_PCR_MUX(PIN_PWM4_MUX);
    PIN_PWM5_PORT->PCR[PIN_PWM5_PIN] = PORT_PCR_MUX(PIN_PWM5_MUX);

    /* Disable DC-bus brake. */
    PIN_MCU_BRAKE_GPIO->PCOR = (1UL << PIN_MCU_BRAKE_PIN);

    /* Activate the USER_LED_2 for error signalization. */
    BOARD_USER_LED_2_ON();

#if defined(DEBUG)
    /* Initialize the timers for permanent safety error signalization. */
    ui64ErrCodeCtr = ui32ErrCode;
    ui64ErrCodeCtr *= ui64ErrCodeCtr;
    ui32Timer      = 0UL;
    ui32WdogTimer  = 0U;
#endif /* defined(DEBUG) */

    /* Lock the MCU in the endless loop. */
    while(TRUE)
    {
#if defined(DEBUG)
        /* Check whether the blinking timer run out. */
        if(0UL < ui32Timer)
        {
            /* Decrement the timer. */
            ui32Timer--;
        }
        else
        {
            /* Check whether the error code counter run out. */
            if(0ULL < ui64ErrCodeCtr)
            {
                /* Decrement the error code counter. */
                ui64ErrCodeCtr >>= 1U;

                /* Re-initialize the timer. */
                ui32Timer = CLOCK_SYSTEM_FREQ / 50U;
            }
            else
            {
                /* Reset the error code counter. */
                ui64ErrCodeCtr = ui32ErrCode;
                ui64ErrCodeCtr *= ui64ErrCodeCtr;

                /* Re-initialize the timer for a longer period. */
                ui32Timer = CLOCK_SYSTEM_FREQ / 10U;
            }

            /* Toggle the LED. */
            BOARD_USER_LED_2_TGL();
        }

        /* Check whether the watchdog timer run out. */
        if(0UL < ui32WdogTimer)
        {
            /* Decrement the watchdog refresh timer. */
            ui32WdogTimer--;
        }
        else
        {
            /* Re-initialize the watchdog refresh timer. */
            /*! @note This period is tied to the MCU clock. Depending on the
                      safety error, it might not operate properly. */
            ui32WdogTimer = CLOCK_SYSTEM_FREQ / 500UL;

            /* Refresh the watchdog. */
            FS_WDOG_REFRESH();
        }
#endif /* defined(DEBUG) */
    }

#endif /* FS_CFG_SAFETY_ERROR_ACTION */
}



/*                            WATCHDOG TEST                                   */
/*!
 * WATCHDOG after-reset test.
 */
void FS_fsWatchdogTest_AR(void)
{
#if FS_CFG_ENABLE_WATCHDOG

    /* Check what was the source of reset. */
    if(0U == (RCM->SRS0 & RCM_SRS0_WDOG_MASK))
    {
        /* Non-WDOG reset occurred. This should occur only after power-on reset.
           Initialize WDOG reset-capability test. */
        FS_CMn_WDOG_Setup(c_sfsLCFSetup.psWDGTstVarsAddr);
    }

    /* The latest reset was caused by watchdog reset - investigate the watchdog
       performance. */
    FS_CMn_WDOG_Check(FS_WDOG_CLK_LIM_HI,
                      FS_WDOG_CLK_LIM_LO,
                      FS_CFG_WDOG_RESETS_LIMIT,
                      FS_CFG_WDOG_ENDLESS_LOOP_EN,
                      c_sfsLCFSetup.psWDGTstVarsAddr);

    /* Clear watchdog refresh counter. */
    g_sfsSafetyTst.sWDOG.ui16RefreshRatio = (uint16_t)kFS_NoIrqTstNum;

#endif /* FS_CFG_ENABLE_WATCHDOG */

    /* Watchdog after-reset test node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_FSTEST_WDOG);
}



/*                            CLOCK TEST                                      */
/*!
 * CLOCK TEST status capture function, called from interrupt.
 */
void FS_fsClockTestIsr_SL(void)
{
#if FS_CFG_ENABLE_TEST_CLOCK

    /* Capture the status of the independent timer. */
    FS_CMn_CLK_LPTMR((fs_lptmr_t*)LPTMR0, &g_sfsSafetyTst.sClock.ui32TestContext);

    /* Raise the start flag to allow the clock test evaluation. */
    g_sfsSafetyTst.sClock.ui16TestStartFlag++;

#endif /* FS_CFG_ENABLE_TEST_CLOCK */

    /* Slow control loop safety clock test node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkSL, FS_FLCK_SL_FSTEST_CLOCK);
}

/*!
 * CLOCK TEST evaluation function.
 */
void FS_fsClockTestCheck_BL(void)
{
#if FS_CFG_ENABLE_TEST_CLOCK
    uint32_t ui32TstResult; /* Test result return code. */

    /* Check whether the first investigated ISR was executed. */
    if(1U < g_sfsSafetyTst.sClock.ui16TestStartFlag)
    {
        /* Perform the clock test. */
        ui32TstResult = FS_CMn_CLK_Check(g_sfsSafetyTst.sClock.ui32TestContext,
                                         FS_CLOCK_CLK_LIM_LO,
                                         FS_CLOCK_CLK_LIM_HI);

        /* Reset the valid clock measurement flag. */
        g_sfsSafetyTst.sClock.ui16TestStartFlag = 2U;

        /* Evaluate the clock test results. */
        if(((uint32_t)FS_FAIL_CLK) == ui32TstResult)
        {
            /* Call the error handling routine. */
            FS_fsErrorHandling(FS_ERROR_CLOCK_TEST, ui32TstResult);
        }
    }

#endif /* FS_CFG_ENABLE_TEST_CLOCK */

    /* Background safety clock test evaluation node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkBL, FS_FLCK_BL_FSTEST_CLOCK);
}



/*                              FLASH TEST                                    */
/*!
 * FLASH TEST after reset.
 */
void FS_fsFlashTest_AR(void)
{
#if FS_CFG_ENABLE_TEST_FLASH

    uint32_t ui32TstResult; /* Test result return code. */

    /* Refresh watchdog. */
    FS_WDOG_REFRESH();

    /* Calculate the CRC of the safety-related flash. */
    ui32TstResult = FS_CMn_FLASH_HW16(c_sfsLCFSetup.ui32FlashSafeStart,
                                      c_sfsLCFSetup.ui32FlashSafeEnd -
                                      c_sfsLCFSetup.ui32FlashSafeStart,
                                      CRC_BASE, 0U);


    /* Compare the flash CRC with the expected value. */
    if(*c_sfsLCFSetup.pui16FlashSafeCRC != (uint16_t)ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_FLASH_TEST, ui32TstResult);
    }

#endif /* FS_CFG_ENABLE_TEST_FLASH */

    /* FLASH after-reset test node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_FSTEST_FLASH);
}

/*!
 * FLASH TEST run-time check.
 */
void FS_fsFlashTest_BL(void)
{
#if FS_CFG_ENABLE_TEST_FLASH

    uint32_t ui32TestBlockEndAddr;

#if defined(DEBUG)
    uint16_t ui16CycleCtr;  /* Safety flash scan counter variable. */
#endif

    /* Calculate end address of the flash block */
    ui32TestBlockEndAddr = g_sfsSafetyTst.sFLASH.ui32TestBlockAddr +
                           FS_CFG_FLASH_TST_BLOCK_SIZE;

    /* Check whether the block-end-address overflows the safety flash memory. */
    if(ui32TestBlockEndAddr < c_sfsLCFSetup.ui32FlashSafeEnd)
    {
        /* Calculate the running CRC of single block of the safety-related
           flash. */
        g_sfsSafetyTst.sFLASH.ui16PartialCRC =
            FS_CMn_FLASH_HW16(g_sfsSafetyTst.sFLASH.ui32TestBlockAddr,
                              FS_CFG_FLASH_TST_BLOCK_SIZE,
                              CRC_BASE,
                              g_sfsSafetyTst.sFLASH.ui16PartialCRC);

        /* Move to the next flash memory block. */
        g_sfsSafetyTst.sFLASH.ui32TestBlockAddr = ui32TestBlockEndAddr;
    }
    else
    {
        /* Limit the block size. Use ui32TestBlockEndAddr to store its size. */
        ui32TestBlockEndAddr = c_sfsLCFSetup.ui32FlashSafeEnd -
                               g_sfsSafetyTst.sFLASH.ui32TestBlockAddr;

        /* Calculate the running CRC of single block of the safety-related
           flash. */
        g_sfsSafetyTst.sFLASH.ui16PartialCRC =
            FS_CMn_FLASH_HW16(g_sfsSafetyTst.sFLASH.ui32TestBlockAddr,
                              ui32TestBlockEndAddr,
                              CRC_BASE,
                              g_sfsSafetyTst.sFLASH.ui16PartialCRC);

        /* Check the final CRC. */
        if(*c_sfsLCFSetup.pui16FlashSafeCRC != g_sfsSafetyTst.sFLASH.ui16PartialCRC)
        {
            /* Execute the safety error handling routine. */
            FS_fsErrorHandling(FS_ERROR_FLASH_TEST, 0UL);
        }

        /* Reset the CRC value to the original seed value. */
        g_sfsSafetyTst.sFLASH.ui16PartialCRC = 0U;

        /* Reset the block address to the safety flash start. */
        g_sfsSafetyTst.sFLASH.ui32TestBlockAddr = c_sfsLCFSetup.ui32FlashSafeStart;

#if defined(DEBUG)
        /* Get the number of cycles it took to scan the whole safety FLASH */
        ui16CycleCtr = g_sSafetyDbg.ui16CycleCtr;

        /* Calculate the number of fast-loop cycles it took to scan the whole
           safety FLASH. */
        g_sSafetyDbg.sFLASH.ui16ScanTime =
            ui16CycleCtr - g_sSafetyDbg.sFLASH.ui16CycleLast;

        /* Store the number of cycles. */
        g_sSafetyDbg.sFLASH.ui16CycleLast = ui16CycleCtr;
#endif /* defined(DEBUG) */
    }

#endif /* FS_CFG_ENABLE_TEST_FLASH */

    /* Background safety FLASH runtime test node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkBL, FS_FLCK_BL_FSTEST_FLASH);
}



/*                               RAM TEST                                     */
/*!
 * RAM TEST after reset.
 */
void FS_fsRamTest_AR(void)
{
    uint32_t ui32TstResult; /* Test result return code. */

    /* Refresh watchdog. */
    FS_WDOG_REFRESH();

    /* Perform the test of the entire safety RAM. */
    ui32TstResult = FS_CMn_RAM_AfterReset(c_sfsLCFSetup.ui32RAMSafeStart,
                                          c_sfsLCFSetup.ui32RAMSafeEnd,
                                          c_sfsLCFSetup.ui32RAMBackupSize,
                                          c_sfsLCFSetup.ui32RAMBackupStart,
                                          FS_CMn_RAM_SegmentMarchC);

    /* Check the RAM test results. */
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_RAM_SAFETY_TEST, ui32TstResult);
    }

    /* Refresh watchdog. */
    FS_WDOG_REFRESH();

    /* Perform the test of the entire Stack. */
    ui32TstResult = FS_CMn_RAM_AfterReset(c_sfsLCFSetup.ui32StackStart,
                                          c_sfsLCFSetup.ui32StackEnd,
                                          c_sfsLCFSetup.ui32RAMBackupSize,
                                          c_sfsLCFSetup.ui32RAMBackupStart,
                                          FS_CMn_RAM_SegmentMarchC);

    /* Check the Stack test results. */
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_RAM_STACK_TEST, ui32TstResult);
    }

    /* RAM after-reset test node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_FSTEST_RAM);
}


/*                           PROGRAM COUNTER TEST                             */
/*!
 * ROGRAM COUNTER after-reset test.
 */
void FS_fsPcTest_AR(void)
{
#if FS_CFG_ENABLE_TEST_PC

    uint32_t ui32TstResult; /* Test result return code. */

    /* Perform the program counter test. */
    ui32TstResult = FS_CMn_PC_Test(FS_CFG_PC_TEST_PATTERN,
                                   FS_CMn_PC_Object,
                                   c_sfsLCFSetup.pui32PCTestFlag);

    /* Check the PC test results. */
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_PC_TEST, ui32TstResult);
    }

#endif /* FS_CFG_ENABLE_TEST_PC */

    /* PC register after-reset test node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_FSTEST_PC);
}

/*!
 * PROGRAM COUNTER runt-time test.
 */
static void FS_fsPcTest_FL(void)
{
#if FS_CFG_ENABLE_TEST_PC

    uint32_t ui32TstResult; /* Test result return code. */

    /* Perform the program counter test. */
    ui32TstResult = FS_CMn_PC_Test(FS_CFG_PC_TEST_PATTERN,
                                   FS_CMn_PC_Object,
                                   c_sfsLCFSetup.pui32PCTestFlag);

    /* Check the PC test results. */
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_PC_TEST, ui32TstResult);
    }

#endif /* FS_CFG_ENABLE_TEST_PC */

    /* Continue to the next uninterruptible test. */
    g_sfsSafetyTst.eNoIrqTest = kFS_SafeRamTest;

    /* The safety uninterruptible tests node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_FSTST_UNINTERR);
}

/*                          CPU REGISTRY TESTS                                */
/*!
 * CPU REGISTERS after-reset test.
 */
void FS_fsCpuTest_AR(void)
{
    /* Refresh watchdog. */
    FS_WDOG_REFRESH();

    /* Test following registers:
       -Stacked CPU registers (R0-R7, R12, LR, and APSR).
       -Non-stacked CPU registers (R8 ? R11).
       -Process Stack Pointer (PSP) register.
       -The CONTROL register. */
    FS_fsCpuTest_BL();

    /* Test following registers:
       -The Priority Mask (PRIMASK) register.
       -The Main Stack Pointer (MSP) register. */
    FS_fsCpuTest_FL();

    /* CPU registers after-reset test node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_FSTEST_CPU);
}

/*!
 * CPU REGISTERS interruptible (background) test.
 */
void FS_fsCpuTest_BL(void)
{
    uint32_t ui32PrimaskReg;
    uint32_t ui32TstResult; /* Test result return code. */

    /* Test stacked CPU registers (R0-R7, R12, LR, and APSR). */
    ui32TstResult = FS_CMn_CPU_Register();
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_CPU_STACKED, ui32TstResult);
    }

    /* Test non-stacked CPU registers (R8 ? R11). */
    ui32TstResult = FS_CMn_CPU_NonStackedRegister();
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_CPU_NONSTACKED, ui32TstResult);
    }

    /* Test the CONTROL register. The interrupts are globally disabled during
       the test. */
    ui32PrimaskReg = __get_PRIMASK();
    __disable_irq();
    ui32TstResult = FS_CMn_CPU_Control();
    __set_PRIMASK(ui32PrimaskReg);
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_CPU_CONTROL, ui32TstResult);
    }

    /* Test the Process Stack Pointer (PSP) register. The interrupts are
       globally disabled during the test. */
    ui32PrimaskReg = __get_PRIMASK();
    __disable_irq();
    ui32TstResult = FS_CMn_CPU_SPprocess();
    __set_PRIMASK(ui32PrimaskReg);
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_CPU_PSP, ui32TstResult);
    }

#if defined(__FPU_PRESENT)
    /* Test the group 1 of FPU registers. The interrupts are globally disabled
       during the test. */
    ui32PrimaskReg = __get_PRIMASK();
    __disable_irq();
    ui32TstResult = FS_CM4_CM7_CPU_Float1();
    __set_PRIMASK(ui32PrimaskReg);
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_CPU_FLOAT1, ui32TstResult);
    }

    /* Test the group 2 of FPU registers. The interrupts are globally disabled
       during the test. */
    ui32PrimaskReg = __get_PRIMASK();
    __disable_irq();
    ui32TstResult = FS_CM4_CM7_CPU_Float2();
    __set_PRIMASK(ui32PrimaskReg);
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_CPU_FLOAT2, ui32TstResult);
    }
#endif /* defined(__FPU_PRESENT) */

    /* Background CPU registry safety test node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkBL, FS_FLCK_BL_FSTEST_CPU);
}


/*                              STACK TEST                                    */
/*!
 * STACK TEST after restart and during runtime.
 */
void FS_fsStackTest_AR(void)
{
    uint32_t ui32TstResult; /* Test result return code. */

    /* Perform the stack underflow/overflow test. */
    ui32TstResult = FS_CMn_STACK_Test(FS_CFG_STACK_TEST_PATTERN,
                                      c_sfsLCFSetup.ui32StackStart,
                                      c_sfsLCFSetup.ui32StackEnd,
                                      c_sfsLCFSetup.ui32StackTstBlckSize);

    /* Check the stack underflow/overflow test results. */
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_STACK_TEST, ui32TstResult);
    }

    /* STACK after-reset test node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_FSTEST_STACK);
}

/*!
 * STACK TEST after restart and during runtime.
 */
void FS_fsStackTest_BL(void)
{
    uint32_t ui32TstResult; /* Test result return code. */

    /* Perform the stack underflow/overflow test. */
    ui32TstResult = FS_CMn_STACK_Test(FS_CFG_STACK_TEST_PATTERN,
                                      c_sfsLCFSetup.ui32StackStart,
                                      c_sfsLCFSetup.ui32StackEnd,
                                      c_sfsLCFSetup.ui32StackTstBlckSize);

    /* Check the stack underflow/overflow test results. */
    if(((uint32_t)FS_PASS) != ui32TstResult)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_STACK_TEST, ui32TstResult);
    }

    /* Background stack overflow/underflow runtime test node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkBL, FS_FLCK_BL_FSTEST_STACK);
}


/*                        INTERRUPT HANDLING TEST                             */
/*!
 * Fast control loop interrupt handling test.
 */
void FS_fsISRTest_FL(void)
{
#if FS_CFG_ENABLE_TEST_ISR
    /* Check whether the fast-to-slow ISR execution rate maximum was not
       violated. */
    if(FS_ISR_FL_PER_SL_EXECS_MAX > g_sfsSafetyTst.sISR.ui32CtrFastISR)
    {
        /* Increment the fast control loop execution counter. */
        g_sfsSafetyTst.sISR.ui32CtrFastISR++;
    }
    else
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_ISR_TEST, SAFETY_ERROR_TOO_MANY_FL_EXECS);
    }
#endif /* FS_CFG_ENABLE_TEST_ISR */

    /* The safety interrupt handling tests node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_FSTST_ISR);
}

/*!
 * Slow control loop interrupt handling test.
 */
void FS_fsISRTest_SL(void)
{
#if FS_CFG_ENABLE_TEST_ISR
    /* Check whether the fast-to-slow ISR execution rate maximum was not
       violated. */
    if(FS_ISR_FL_PER_SL_EXECS_MAX < g_sfsSafetyTst.sISR.ui32CtrFastISR)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_ISR_TEST, SAFETY_ERROR_BAD_FL_TO_SL_RATE);
    }
    /* Check whether the fast-to-slow ISR execution rate minimum was not
       violated. */
    else if(FS_ISR_FL_PER_SL_EXECS_MIN > g_sfsSafetyTst.sISR.ui32CtrFastISR)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_ISR_TEST, SAFETY_ERROR_BAD_FL_TO_SL_RATE);
    }
    /* Check whether slow ISR was not executed too many times compared to
       background loop. */
    else if(FS_CFG_ISR_SL_PER_BL_EXECS_MAX < g_sfsSafetyTst.sISR.ui32CtrSlowISR)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_ISR_TEST, SAFETY_ERROR_BAD_SL_TO_BL_RATE);
    }
    else
    {
        /* Reset the fast control loop ISR counter. */
        g_sfsSafetyTst.sISR.ui32CtrFastISR = 0UL;

        /* Increment the slow control loop execution counter. */
        g_sfsSafetyTst.sISR.ui32CtrSlowISR++;
    }
#endif /* FS_CFG_ENABLE_TEST_ISR */

    /* Slow control loop safety clock test node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkSL, FS_FLCK_SL_FSTEST_ISR);
}

/*!
 * Background interrupt handling test.
 */
void FS_fsISRTest_BL(void)
{
#if FS_CFG_ENABLE_TEST_ISR
    /* Check whether slow ISR was not executed too many times compared to
       background loop. */
    if(FS_CFG_ISR_SL_PER_BL_EXECS_MAX > g_sfsSafetyTst.sISR.ui32CtrSlowISR)
    {
        /* Reset the slow control loop ISR counter. */
        g_sfsSafetyTst.sISR.ui32CtrSlowISR = 0UL;
    }
    else
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_ISR_TEST, SAFETY_ERROR_BAD_SL_TO_BL_RATE);
    }
#endif /* FS_CFG_ENABLE_TEST_ISR */

    /* Background interrupt handling test evaluation node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkBL, FS_FLCK_BL_FSTEST_ISR);
}


/*                          SOFTWARE FLOW TEST                                */
/*!
 * Function evaluate the after-reset software flow.
 */
void FS_fsFlowCheck_AR(void)
{
    /* The last after-reset flow check node.*/
    g_sfsFlowChkAR.ui32Signature ^= (FS_FLCK_AR_NODE_LAST ^ FS_FLCK_AR_NODE_START);
    g_sfsFlowChkAR.ui32Correctness |= (g_sfsFlowChkAR.ui32Signature ^ FS_FLCK_AR_NODE_START);

#if FS_CFG_ENABLE_TEST_FLOW
    /* Check whether the software was executed correctly after the reset. */
    if(0UL < g_sfsFlowChkAR.ui32Correctness)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_SW_FLOW_AR_TEST, g_sfsFlowChkAR.ui32Correctness);
    }
#endif /* FS_CFG_ENABLE_TEST_FLOW */

    /* Initialize BL, FL, and SL software flow checks. */
    FS_FLOW_CHK_INIT(g_sfsFlowChkBL, FS_FLCK_BL_NODE_START);
    FS_FLOW_CHK_INIT(g_sfsFlowChkFL, FS_FLCK_FL_NODE_START);
    FS_FLOW_CHK_INIT(g_sfsFlowChkSL, FS_FLCK_SL_NODE_START);
}

/*!
 * Function evaluates the background loop software flow.
 */
void FS_fsFlowCheck_BL(void)
{
    /* The last background loop flow check node.*/
    g_sfsFlowChkBL.ui32Signature ^= (FS_FLCK_BL_NODE_LAST ^ FS_FLCK_BL_NODE_START);
    g_sfsFlowChkBL.ui32Correctness |= (g_sfsFlowChkBL.ui32Signature ^ FS_FLCK_BL_NODE_START);

#if FS_CFG_ENABLE_TEST_FLOW
    /* Check whether the software was executed correctly after the reset. */
    if(0UL < g_sfsFlowChkBL.ui32Correctness)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_SW_FLOW_BL_TEST, g_sfsFlowChkBL.ui32Correctness);
    }
#endif /* FS_CFG_ENABLE_TEST_FLOW */
}

/*!
 * Function evaluates the fast control loop software flow.
 */
void FS_fsFlowCheck_FL(void)
{
    /* The last fast control loop flow check node.*/
    g_sfsFlowChkFL.ui32Signature ^= (FS_FLCK_FL_NODE_LAST ^ FS_FLCK_FL_NODE_START);
    g_sfsFlowChkFL.ui32Correctness |= (g_sfsFlowChkFL.ui32Signature ^ FS_FLCK_FL_NODE_START);

#if FS_CFG_ENABLE_TEST_FLOW
    /* Check whether the software was executed correctly. */
    if(0UL < g_sfsFlowChkFL.ui32Correctness)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_SW_FLOW_FL_TEST, g_sfsFlowChkFL.ui32Correctness);
    }
#endif /* FS_CFG_ENABLE_TEST_FLOW */
}

/*!
 * Function evaluates the slow control loop software flow.
 */
void FS_fsFlowCheck_SL(void)
{
    /* The last slow control loop flow check node.*/
    g_sfsFlowChkSL.ui32Signature ^= (FS_FLCK_SL_NODE_LAST ^ FS_FLCK_SL_NODE_START);
    g_sfsFlowChkSL.ui32Correctness |= (g_sfsFlowChkSL.ui32Signature ^ FS_FLCK_SL_NODE_START);

#if FS_CFG_ENABLE_TEST_FLOW
    /* Check whether the software was executed correctly. */
    if(0UL < g_sfsFlowChkSL.ui32Correctness)
    {
        /* Execute the safety error handling routine. */
        FS_fsErrorHandling(FS_ERROR_SW_FLOW_SL_TEST, g_sfsFlowChkSL.ui32Correctness);
    }
#endif /* FS_CFG_ENABLE_TEST_FLOW */
}
