/*
 * Copyright 2012-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdbool.h>
#include <stdint.h>
#include "bootloader_common.h"
#include "keyblob.h"
#include "microseconds/microseconds.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if 0
#define KEYBLOB_PRINTF(...)        \
    do                             \
    {                              \
        debug_printf(__VA_ARGS__); \
    } while (0)
#else
#define KEYBLOB_PRINTF(...)
#endif

#if !BL_FEATURE_KEYBLOB_USE_SECURE_MEMORY
#warning when using HAB for encrypt boot, secure memory must be initialized to generate the key blob, otherwise the key blob type will be mismatched.
#endif

/*! @brief The timeout to wait for CAAM busy clearance.*/
#define CAAM_TIMEOUT_WAIT_BUSY (100)
/*! @brief The timeout to wait for CAAM to complete a task. 0 means no timeout(wait infinitly). */
#define CAAM_TIMEOUT_RUN_INFINITE (0)
/*! @brief The timeout to wait for secure memory command completation.*/
#define CAAM_TIMEOUT_SECURE_MEMORY_CMD (100)
/*! @brief The timeout to wait for secure memory page to be available.*/
#define CAAM_TIMEOUT_SECURE_MEMORY_PAGE_AVAILABLE (100)

/*! @brief Job ring index selected to opeartion key blob generation. */
#define CAAM_JOB_RING_INDEX (0)
/*! @brief Job ring entry count for key blob generation. */
#define CAAM_JOB_RING_ENTRIES (1)

/*! @brief SNVS security state machine. */
enum
{
    kSNVS_Security_State_Init = 0,
    kSNVS_Security_State_HardFail = 1,
    kSNVS_Security_State_SoftFail = 3,
    kSNVS_Security_State_InitIntermediate = 8,
    kSNVS_Security_State_Check = 9,
    kSNVS_Security_State_NonSecure = 11,
    kSNVS_Security_State_Trusted = 13,
    kSVNS_Security_State_Secure = 15,
};

/*! @brief master key selection for SNVS. */
enum
{
    kKeySource_OTPMK = 0,     /*!< OTPMK from FUSE is used as KEK */
    kKeySource_OTPMK_Sub = 1, /*!< 2nd enum value for OTPMK, the same effect with kKeySource_OTPMK */
    kKeySource_ZMK = 2,       /*!< ZMK from SVNS is used as KEK  */
    kKeySource_CMK = 3,       /*!< CMK from SVNS is used as KEK  */
};

/*! @brief The SNVS power glitch violation proper value. */
#define SNVS_LP_LVD_VALUE (0x41736166)

/*! @brief Flags for secure memory command execution. */
enum
{
    kSecureMemory_Command_Flag_PG = 0,
    kSecureMemory_Command_Flag_JR = 1,
};

/*! @brief Flags for secure memory partition/page allocation. */
enum
{
    kSecureMemory_Allocate_Flag_Specific = 0,
    kSecureMemory_Allocate_Flag_Any = 1,
};

/*! @brief Command tag for secure memory. */
enum
{
    kSecureMemory_Command_Tag_AllocatePage = 1,
    kSecureMemory_Command_Tag_DeallocatePage = 2,
    kSecureMemory_Command_Tag_DeallocatePartition = 3,
    kSecureMemory_Command_Tag_PageInquiry = 5,
};

/*! @brief Status code for secure memory command execution. */
enum
{
    kSecureMemory_Command_Status_Completed = 0,
    kSecureMemory_Command_Status_NotCompleted = 1,
    kSecureMemory_Command_Status_SecurityFailure = 2,
    kSecureMemory_Command_Status_Overflow = 3,
};

/*! @brief Status code for secure memory partition. */
enum
{
    kSecureMemory_Partition_State_Available = 0,
    kSecureMemory_Partition_State_NotExist = 1,
    kSecureMemory_Partition_State_OwnedByOther = 2,
    kSecureMemory_Partition_State_OwnedByCurrent = 3,
};

/*! @brief Status code for secure memory page. */
enum
{
    kSecureMemory_Page_State_Available = 0,
    kSecureMemory_Page_State_NotAvailable = 1,
    kSecureMemory_Page_State_OwnedByOther = 2,
    kSecureMemory_Page_State_OwnedByCurrent = 3,
};

/*! @brief Settings for secure memory permission. */
enum
{
    kSecureMemory_Permission_FullAccess = CAAM_PX_SMAPR_JR_G1_TDO_MASK | CAAM_PX_SMAPR_JR_G1_READ_MASK |
                                          CAAM_PX_SMAPR_JR_G1_WRITE_MASK | CAAM_PX_SMAPR_JR_G1_SMBLOB_MASK |
                                          CAAM_PX_SMAPR_JR_G2_TDO_MASK | CAAM_PX_SMAPR_JR_G2_READ_MASK |
                                          CAAM_PX_SMAPR_JR_G2_WRITE_MASK | CAAM_PX_SMAPR_JR_G2_SMBLOB_MASK,

    kSecureMemory_Permission_KeyBlobGen = CAAM_PX_SMAPR_JR_CSP_MASK | CAAM_PX_SMAPR_JR_SMAP_LCK_MASK |
                                          CAAM_PX_SMAPR_JR_SMAG_LCK_MASK | CAAM_PX_SMAPR_JR_G1_SMBLOB_MASK,
};

/*! @brief JOB RING address alignment. */
#define CAAM_JOB_RING_ADDR_ALIGN (8)

/*! @brief Key blob header parameters. */
enum _keyblob_header_parameters_
{
    kKeyBlob_Header_Tag = 0x81, /*!< Tag for the Key Blob header */

    kKeyBlob_Header_Version = 0x41, /*!< Key Blob version, Ver4.1 */

    kKeyBlob_Header_Mode_CCM = 0x66, /*!< CCM algorithm, CCM */

    kKeyBlob_Header_Algorithm_AES = 0x55, /*!< Wrap algorithm, AES */

    kKeyBlob_Header_Size_128Bit = 0x10, /*!< Wrap key length, 128bit */
    kKeyBlob_Header_Size_192Bit = 0x18, /*!< Wrap key length, 128bit */
    kKeyBlob_Header_Size_256Bit = 0x20, /*!< Wrap key length, 128bit */
};

/*! @brief Key blob header structure. */
typedef struct _keyblob_header_
{
    uint8_t tag;     /*!< Tag for the Key Blob header */
    uint8_t len[2];  /*!< Length in bytes of the entire Key Blob, including header, big-endian */
    uint8_t version; /*!< Key Blob version. */
    uint8_t mode;    /*!< CCM algorithm. */
    uint8_t alg;     /*!< Wrap algorithm. */
    uint8_t size;    /*!< Wrapped key length. */
    uint8_t flag;    /*!< Not used yet. */
} keyblob_header_t;

/*!@brief KeyBlob Block definition. */
typedef struct _keyblob_block
{
    keyblob_header_t header; /*!< KeyBlob header. */
    uint8_t blob[];          /*!< Key Blob field. */
} keyblob_block_t;

/*!@brief CAAM job ring execution result status. */
#define CAAM_JOB_TERMINITION_STATUS_SUCCESS (0)

/*!@brief CAAM input job ring structure. */
typedef struct _caam_job_ring_input
{
    uint32_t dscAddr; /*!< Address of Descriptor. */
} caam_job_ring_input_t;

/*!@brief CAAM output job ring structure. */
typedef struct _caam_job_ring_output
{
    uint32_t dscAddr; /*!< Address of Descriptor. */
    uint32_t status;  /*!< Job termination status/error. */
    uint32_t length;  /*!< Output sequence length(optional). */
} caam_job_ring_output_t;

/* The definations of secure memory buffers used by caam job rings.*/
/*
 * Note: The locations of the buffers must be put at one page. And the min page size for secure memory is 1K bytes.
 */
/*!@brief The address of secure memory buffer for caam input job ring. */
#define CAAM_INPUT_RING_OFFSET (0x0)
/*!@brief The address of secure memory buffer for caam output job ring. */
#define CAAM_OUTPUT_RING_OFFSET (0x10)
/*!@brief The address of secure memory buffer for caam job descriptor. */
#define CAAM_JOB_DSC_OFFSET (0x100)
/*!@brief The address of secure memory buffer containing key blob. */
#define CAAM_BLOB_OFFSET (0x200)

/*!@brief The context for caam key blob encapsulation. */
typedef struct caam_blob_context
{
    CAAM_Type *base;                    /*!< Base address for CAAM module. */
    SNVS_Type *snvsBase;                /*!< Base address for SNVS module. */
    uint32_t jobRing;                   /*!< Job ring index used for Key Blob operation. */
    caam_job_ring_input_t *inputRing;   /*!< Base address for Input Ring. */
    caam_job_ring_output_t *outputRing; /*!< Base address for Output Ring. */
    uint32_t jobDscAddr;                /*!< Base address for Job Descriptor. */
    uint32_t inputAddr;                 /*!< Base address for input data. */
    uint32_t outputAddr;                /*!< Base address for output data. */
#if BL_FEATURE_KEYBLOB_USE_SECURE_MEMORY
    uint32_t dataPartition; /*!< Partition index to store job ring data. */
    uint32_t dataPage;      /*!< Page index to store job ring data. */
    uint32_t keyPartition;  /*!< Partition index to store key. */
    uint32_t keyPage;       /*!< Page index to store key. */
#endif
} caam_blob_context_t;

/*!@brief Get the words of the descriptor. */
#define CAAM_DSC_GET_LENGTH_IN_WORD(dscStartAddr) ((*((uint32_t *)dscStartAddr)) & 0x7F)

/*!@brief Mask of the length bitfield of key modifier. */
#define CAAM_DSC_KEY_MODIFIER_LENGTH_MASK (0x00FF0000)
/*!@brief Shift of the length bitfield of key modifier. */
#define CAAM_DSC_KEY_MODIFIER_LENGTH_SHIFT (16)
/*!@brief Set the length bitfield of key modifier. */
#define CAAM_DSC_KEY_MODIFIER_LENGTH(x) \
    (((uint32_t)(((uint32_t)(x)) << CAAM_DSC_KEY_MODIFIER_LENGTH_SHIFT)) & CAAM_DSC_KEY_MODIFIER_LENGTH_MASK)
/*!@brief Mask of the length bitfield of SEQ IN PTR command. */
#define CAAM_DSC_SEQ_IN_PTR_LENGTH_MASK (0x0000FFFF)
/*!@brief Shift of the length bitfield of SEQ IN PTR command. */
#define CAAM_DSC_SEQ_IN_PTR_LENGTH_SHIFT (0)
/*!@brief Set the length bitfield of SEQ IN PTR command. */
#define CAAM_DSC_SEQ_IN_PTR_LENGTH(x) \
    (((uint32_t)(((uint32_t)(x)) << CAAM_DSC_SEQ_IN_PTR_LENGTH_SHIFT)) & CAAM_DSC_SEQ_IN_PTR_LENGTH_MASK)
/*!@brief Mask of the length bitfield of SEQ OUT PTR command. */
#define CAAM_DSC_SEQ_OUT_PTR_LENGTH_MASK (0x0000FFFF)
/*!@brief Shift of the length bitfield of SEQ OUT PTR command. */
#define CAAM_DSC_SEQ_OUT_PTR_LENGTH_SHIFT (0)
/*!@brief Set the length bitfield of SEQ OUT PTR command. */
#define CAAM_DSC_SEQ_OUT_PTR_LENGTH(x) \
    (((uint32_t)(((uint32_t)(x)) << CAAM_DSC_SEQ_OUT_PTR_LENGTH_SHIFT)) & CAAM_DSC_SEQ_OUT_PTR_LENGTH_MASK)

/*!@brief CAAM RNG instantiation descriptor - HEADER. Descriptor header. */
#define CAAM_RNG_INST_DESC0 (0xB0800009)
/*!@brief CAAM RNG instantiation descriptor - LOAD. Load 8 bytes personalization string to Class 1 Context Register. */
#define CAAM_RNG_INST_DESC1 (0x12A00008)
/*!@brief CAAM RNG instantiation descriptor - DATA. 8 bytes personalization string. */
#define CAAM_RNG_INST_DESC2 (0x01020304)
#define CAAM_RNG_INST_DESC3 (0x05060708)
/*!@brief CAAM RNG instantiation descriptor - ALGORITHM OPERATION. Instantiate State Handle 0 without prediction
 * resistance. */
#define CAAM_RNG_INST_DESC4 (0x82500404)
/*!@brief CAAM RNG instantiation descriptor - JUMP/HALT. Wait instantiation done then local jump to next command. */
#define CAAM_RNG_INST_DESC5 (0xA2000001)
/*!@brief CAAM RNG instantiation descriptor - LOAD. Load 4 bytes data to Clear Written Register. */
#define CAAM_RNG_INST_DESC6 (0x10880004)
/*!@brief CAAM RNG instantiation descriptor - DATA. Clear the Class 1 Mode Register. */
#define CAAM_RNG_INST_DESC7 (0x00000001)
/*!@brief CAAM RNG instantiation descriptor - ALGORITHM OPERATION. Generate class 1 secure keys. */
#define CAAM_RNG_INST_DESC8 (0x82501000)

/*!@brief CAAM blob encapsulation descriptor - HEADER. Descriptor header. */
#define CAAM_ENCAP_BLOB_SM_DESC0 (0xB0800009)
/*!@brief CAAM blob encapsulation descriptor - Load. key modifier into Class 2 Key Register. */
#define CAAM_ENCAP_BLOB_SM_DESC1 (0x14C00C08)
/*!@brief CAAM blob encapsulation descriptor - DATA. 8 bytes key modifier. */
#define CAAM_ENCAP_BLOB_SM_DESC2 (0x00105566)
#define CAAM_ENCAP_BLOB_SM_DESC3 (0x00000000)
/*!@brief CAAM blob encapsulation descriptor - SEQ IN PTR. Set loading data key. */
#define CAAM_ENCAP_BLOB_SM_DESC4 (0xF0000010)
/*!@brief CAAM blob encapsulation descriptor - DATA. Data key address, provided at run-time. */
#define CAAM_ENCAP_BLOB_SM_DESC5 (0)
/*!@brief CAAM blob encapsulation descriptor - SEQ OUT PTR. Set outputing key blob. */
#define CAAM_ENCAP_BLOB_SM_DESC6 (0xF8000040)
/*!@brief CAAM blob encapsulation descriptor - DATA. key blob address, provided at run-time. */
#define CAAM_ENCAP_BLOB_SM_DESC7 (0)
/*!@brief CAAM blob encapsulation descriptor - PROTOCAL. Perform key blob generation. */
#define CAAM_ENCAP_BLOB_SM_DESC8 (0x870D0008)

/*!@brief CAAM blob encapsulation descriptor - HEADER. Descriptor header. */
#define CAAM_ENCAP_BLOB_SRAM_DESC0 (0xB080000b)
/*!@brief CAAM blob encapsulation descriptor - Load. key modifier into Class 2 Key Register. */
#define CAAM_ENCAP_BLOB_SRAM_DESC1 (0x14C00010)
/*!@brief CAAM blob encapsulation descriptor - DATA. 16 bytes key modifier. */
#define CAAM_ENCAP_BLOB_SRAM_DESC2 (0x00105566)
#define CAAM_ENCAP_BLOB_SRAM_DESC3 (0x00000000)
#define CAAM_ENCAP_BLOB_SRAM_DESC4 (0x00000000)
#define CAAM_ENCAP_BLOB_SRAM_DESC5 (0x00000000)
/*!@brief CAAM blob encapsulation descriptor - SEQ IN PTR. Set loading data key. */
#define CAAM_ENCAP_BLOB_SRAM_DESC6 (0xF0000010)
/*!@brief CAAM blob encapsulation descriptor - DATA. Data key address, provided at run-time. */
#define CAAM_ENCAP_BLOB_SRAM_DESC7 (0)
/*!@brief CAAM blob encapsulation descriptor - SEQ OUT PTR. Set outputing key blob. */
#define CAAM_ENCAP_BLOB_SRAM_DESC8 (0xF8000040)
/*!@brief CAAM blob encapsulation descriptor - DATA. key blob address, provided at run-time. */
#define CAAM_ENCAP_BLOB_SRAM_DESC9 (0)
/*!@brief CAAM blob encapsulation descriptor - PROTOCAL. Perform key blob generation. */
#define CAAM_ENCAP_BLOB_SRAM_DESCa (0x870D0000)

#if BL_FEATURE_KEYBLOB_USE_SECURE_MEMORY
/*!@brief CAAM blob encapsulation descriptor index which contains the key modifier. */
#define CAAM_ENCAP_BLOB_DESC_KEYMODIFIER_INDEX (2)
/*!@brief CAAM blob encapsulation descriptor index of the SEQ IN PTR command. */
#define CAAM_ENCAP_BLOB_DESC_SEQINPTR_INDEX (4)
#else
/*!@brief CAAM blob encapsulation descriptor index which contains the key modifier. */
#define CAAM_ENCAP_BLOB_DESC_KEYMODIFIER_INDEX (2)
/*!@brief CAAM blob encapsulation descriptor index of the SEQ IN PTR command. */
#define CAAM_ENCAP_BLOB_DESC_SEQINPTR_INDEX (6)
#endif
/*******************************************************************************
 * Prototype
 ******************************************************************************/
/*!
 * @brief Secure memory initialization function.
 *
 * @param base CAAM peripheral base address.
 * @retval kStatus_Success Operate successfully.
 * @retval kStatus_Fail Failed to operate.
 */
static inline status_t secure_memory_init(CAAM_Type *base);

/*!
 * @brief Secure memory deinitialization function.
 *
 * @param base CAAM peripheral base address.
 * @retval kStatus_Success Operate successfully.
 * @retval kStatus_Fail Failed to operate.
 */
static inline status_t secure_memory_deinit(CAAM_Type *base);

/*!
 * @brief Calculate the partition count of the secure memory.
 *
 * @param base CAAM peripheral base address.
 * @retval partition count.
 */
static inline uint32_t secure_memory_get_partition_count(CAAM_Type *base);

/*!
 * @brief Calculate the page size of the secure memory.
 *
 * @param base CAAM peripheral base address.
 * @retval page size in bytes.
 */
static inline uint32_t secure_memory_get_page_size(CAAM_Type *base);

/*!
 * @brief Calculate the page count of the secure memory.
 *
 * @param base CAAM peripheral base address.
 * @retval page count.
 */
static inline uint32_t secure_memory_get_page_count(CAAM_Type *base);

/*!
 * @brief Execute a secure memory command.
 *
 * @param base CAAM peripheral base address.
 * @param jobRing CAAM job ring index.
 * @param partition Secure memory partition index.
 * @param page Secure memory page index.
 * @param cmd Secure memory command tag.
 * @param timeoutMs timeout in microseconds to vait the command completed.
 * @param flag Secure memory command flag.
 * @retval kStatus_Success Operate successfully.
 * @retval kStatus_Fail Failed to operate.
 * @retval kStatus_Timeout Timeout reaches. Stop waiting and return.
 */
static status_t secure_memory_command(CAAM_Type *base,
                                      uint32_t jobRing,
                                      uint32_t partition,
                                      uint32_t page,
                                      uint32_t cmd,
                                      uint32_t timeoutMs,
                                      uint32_t flag);

/*!
 * @brief Restore the secure memory configuration before generate key blob.
 *
 * @param base CAAM peripheral base address.
 */
static void secure_memory_restore_configuration(CAAM_Type *base);

/*!
 * @brief Allocate a secure memory partition to a job ring.
 *
 * @param base CAAM peripheral base address.
 * @param jobRing CAAM job ring index.
 * @param partition the partition index.
 * @param permission Permission for the partition.
 * @param flag Flags for partition allocation.
 * @retval kStatus_Success Operate successfully.
 * @retval kStatus_Fail Failed to allocate.
 */
static status_t secure_memory_allocate_partition(
    CAAM_Type *base, uint32_t jobRing, uint32_t *partition, uint32_t permission, uint32_t flag);

/*!
 * @brief release secure memory page 0 partition.
 *
 * @param base CAAM peripheral base address.
 * @retval kStatus_Success Operate successfully.
 * @retval kStatus_Fail Failed to release.
 * @retval kStatus_Timeout Timeout reaches. Stop waiting and return.
 */
static inline status_t secure_memory_release_default_partition(CAAM_Type *base);

/*!
 * @brief Release all pages of an allocated secure memory partition.
 *
 * @param base CAAM peripheral base address.
 * @param jobRing CAAM job ring index.
 * @param partition Secure memory partition index.
 * @retval kStatus_Success Operate successfully.
 * @retval kStatus_Fail Failed to release.
 * @retval kStatus_Timeout Timeout reaches. Stop waiting and return.
 */
static inline status_t secure_memory_release_partition(CAAM_Type *base, uint32_t jobRing, uint32_t partition);

/*!
 * @brief Allocate a page to a secure memory partition.
 *
 * @param base CAAM peripheral base address.
 * @param jobRing CAAM job ring index.
 * @param partition Secure memory partition index.
 * @param page the page index.
 * @param flag Flags for page allocation.
 * @retval kStatus_Success Operate successfully.
 * @retval kStatus_Fail Failed to allocate.
 * @retval kStatus_Timeout Timeout reaches. Stop waiting and return.
 */
static status_t secure_memory_allocate_page(
    CAAM_Type *base, uint32_t jobRing, uint32_t partition, uint32_t *page, uint32_t flag);

/*!
 * @brief Release an allocated page.
 *
 * @param base CAAM peripheral base address.
 * @param jobRing CAAM job ring index.
 * @param page Secure memory page index.
 * @retval kStatus_Success Operate successfully.
 * @retval kStatus_Fail Failed to allocate.
 * @retval kStatus_Timeout Timeout reaches. Stop waiting and return.
 */
static inline status_t secure_memory_release_page(CAAM_Type *base, uint32_t jobRing, uint32_t page);

/*!
 * @brief CAAM initialization function.
 *
 * @param base CAAM peripheral base address.
 * @retval kStatus_Success Operate successfully.
 */
static inline status_t caam_init(CAAM_Type *base);

/*!
 * @brief Wait previous task done, and CAAM is idle.
 *
 * @param base CAAM peripheral base address.
 * @param timeoutMs microseconds to wait for idle. 0 means infinite.
 * @retval kStatus_Timeout Timeout reaches. Stop waiting and return.
 * @retval kStatus_Success Operate successfully.
 */
static inline status_t caam_wait_idle(CAAM_Type *base, uint32_t timeoutMs);

/*!
 * @brief Wait the current task done, and output is available.
 *
 * @param base CAAM peripheral base address.
 * @param timeoutMs microseconds to wait for idle. 0 means infinite.
 * @retval kStatus_Timeout Timeout reaches. Stop waiting and return.
 * @retval kStatus_Success Operate successfully.
 */
static inline status_t caam_wait_done(CAAM_Type *base, uint32_t timeoutMs);

/*!
 * @brief Run CAAM task.
 *
 * @param base CAAM peripheral base address.
 * @param timeoutMs microseconds to wait for idle.
 * @retval kStatus_Timeout Timeout reaches. Stop waiting and return.
 * @retval kStatus_Fail Failed to execute CAAM task.
 * @retval kStatus_Success Operate successfully.
 */
static status_t caam_run(CAAM_Type *base);

/*!
 * @brief CAAM TRNG instantiation.
 *
 * @param base CAAM peripheral base address.
 * @retval kStatus_Timeout Timeout reaches for the operation.
 * @retval kStatus_Fail Failed to instantiate TRNG.
 * @retval kStatus_Success Operate successfully.
 */
static inline status_t rng_inst(CAAM_Type *base);

/*!
 * @brief Use CAAM to generate a key blob.
 *
 * @param base CAAM peripheral base address.
 * @param keySize data key size in bytes.
 * @retval kStatus_Timeout Timeout reaches for the operation.
 * @retval kStatus_Fail Failed to generate key blob.
 * @retval kStatus_Success Operate successfully.
 */
static status_t caam_gen_blob(CAAM_Type *base, uint32_t keySize);

/*!
 * @brief CAAM keyblob encapsulation selftest function.
 */
void generate_key_blob_selftest(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*!@brief 128bit Data Encryption Key example used for self test */
static const uint8_t dek_128_example[16] = { 0xCC, 0x20, 0x02, 0x73, 0xFE, 0xED, 0x33, 0x2D,
                                             0x6B, 0xCB, 0x35, 0xB2, 0xAE, 0x09, 0x76, 0x84 };
/*!@brief 72 byte Key Blob example used for self test, and generated with the dek_128_example, */
static uint8_t keyblob_128_example[72] = { 0 };
/*!@brief 256bit Data Encryption Key example used for self test */
static const uint8_t dek_256_example[32] = { 0x9E, 0xE7, 0x85, 0x5D, 0x26, 0xF8, 0x4F, 0x23, 0x95, 0x72, 0x25,
                                             0x84, 0xBE, 0x89, 0x93, 0xD9, 0x2C, 0x9D, 0x0E, 0x36, 0x72, 0xBB,
                                             0x91, 0x41, 0xFF, 0x9D, 0x03, 0x0D, 0x2E, 0x2D, 0x00, 0xF1 };
/*!@brief 88 byte Key Blob example used for self test, and generated with the dek_256_example, */
static uint8_t keyblob_256_example[88] = { 0 };

#if BL_FEATURE_KEYBLOB_USE_SECURE_MEMORY
/*!@brief CAAM operation context, and filled at run-time. */
static caam_blob_context_t s_caamBlobContext = { 0 };
#else
/*!@brief CAAM input job ring buffer. */
BL_ALIGN(CAAM_JOB_RING_ADDR_ALIGN) caam_job_ring_input_t s_caamInputRingBuffer;
/*!@brief CAAM output job ring buffer. */
BL_ALIGN(CAAM_JOB_RING_ADDR_ALIGN) caam_job_ring_output_t s_caamOutputRingBuffer;
/*!@brief CAAM job ring descriptor buffer. */
BL_ALIGN(CAAM_JOB_RING_ADDR_ALIGN) uint8_t s_caamJobDscBuffer[0x40];
/*!@brief CAAM input data buffer. */
BL_ALIGN(CAAM_JOB_RING_ADDR_ALIGN) uint8_t s_caamInputBuffer[kKeyBlob_DEK_MaxSize];
/*!@brief CAAM output data buffer. */
BL_ALIGN(CAAM_JOB_RING_ADDR_ALIGN) uint8_t s_caamOutputBuffer[kKeyBlob_Blob_MaxSize];
/*!@brief CAAM operation context. */
static const caam_blob_context_t s_caamBlobContext = {
    .base = CAAM,
    .snvsBase = SNVS,
    .jobRing = CAAM_JOB_RING_INDEX,
    .inputRing = (caam_job_ring_input_t *)&s_caamInputRingBuffer,
    .outputRing = (caam_job_ring_output_t *)&s_caamOutputRingBuffer,
    .jobDscAddr = (uint32_t)&s_caamJobDscBuffer,
    .inputAddr = (uint32_t)&s_caamInputBuffer,
    .outputAddr = (uint32_t)&s_caamOutputBuffer,
};
#endif

#if BL_FEATURE_KEYBLOB_USE_SECURE_MEMORY
/*!@brief Descriptor table to encapsulate the key blob. */
static const uint32_t s_blobEncapDsc[] = {
    CAAM_ENCAP_BLOB_SM_DESC0, CAAM_ENCAP_BLOB_SM_DESC1, CAAM_ENCAP_BLOB_SM_DESC2,
    CAAM_ENCAP_BLOB_SM_DESC3, CAAM_ENCAP_BLOB_SM_DESC4, CAAM_ENCAP_BLOB_SM_DESC5,
    CAAM_ENCAP_BLOB_SM_DESC6, CAAM_ENCAP_BLOB_SM_DESC7, CAAM_ENCAP_BLOB_SM_DESC8
};
#else
static const uint32_t s_blobEncapDsc[] = {
    CAAM_ENCAP_BLOB_SRAM_DESC0, CAAM_ENCAP_BLOB_SRAM_DESC1, CAAM_ENCAP_BLOB_SRAM_DESC2, CAAM_ENCAP_BLOB_SRAM_DESC3,
    CAAM_ENCAP_BLOB_SRAM_DESC4, CAAM_ENCAP_BLOB_SRAM_DESC5, CAAM_ENCAP_BLOB_SRAM_DESC6, CAAM_ENCAP_BLOB_SRAM_DESC7,
    CAAM_ENCAP_BLOB_SRAM_DESC8, CAAM_ENCAP_BLOB_SRAM_DESC9, CAAM_ENCAP_BLOB_SRAM_DESCa
};
#endif
/*!@brief Descriptor table to instantiate RNG State Handle 0 and load secure keys(JDKEK, TDKEK, TDSK). */
static const uint32_t s_rngInstDsc[] = { CAAM_RNG_INST_DESC0, CAAM_RNG_INST_DESC1, CAAM_RNG_INST_DESC2,
                                         CAAM_RNG_INST_DESC3, CAAM_RNG_INST_DESC4, CAAM_RNG_INST_DESC5,
                                         CAAM_RNG_INST_DESC6, CAAM_RNG_INST_DESC7, CAAM_RNG_INST_DESC8 };

/*******************************************************************************
 * Codes
 ******************************************************************************/
#if BL_FEATURE_KEYBLOB_USE_SECURE_MEMORY
static inline status_t secure_memory_init(CAAM_Type *base)
{
    status_t status = kStatus_Fail;

    do
    {
        /* Assign job ring DID to access secure memory. */
        base->JRADID[s_caamBlobContext.jobRing].JRDID_MS =
            CAAM_JRDID_MS_TZ_OWN_MASK | CAAM_JRDID_MS_PRIM_TZ_MASK | CAAM_JRDID_MS_PRIM_DID(BL_FEATURE_JOBRING_DID);
        __ISB();
        __DSB();

        /* Allocate the data partition to the job ring. */
        status = secure_memory_allocate_partition(base, s_caamBlobContext.jobRing, &s_caamBlobContext.dataPartition,
                                                  kSecureMemory_Permission_FullAccess, kSecureMemory_Allocate_Flag_Any);
        if (status != kStatus_Success)
        {
            KEYBLOB_PRINTF("Flashloader: %s, failed to allocate data partition.\n", __func__);
            break;
        }
        else
        {
            KEYBLOB_PRINTF("Flashloader: %s, allocate partition %x as data partition.\n", __func__,
                           s_caamBlobContext.dataPartition);
        }

        /* Allocate the data page to the partition. */
        status = secure_memory_allocate_page(base, s_caamBlobContext.jobRing, s_caamBlobContext.dataPartition,
                                             &s_caamBlobContext.dataPage, kSecureMemory_Allocate_Flag_Any);
        if (status != kStatus_Success)
        {
            KEYBLOB_PRINTF("Flashloader: %s, failed to allocate data page.\n", __func__);
            break;
        }
        else
        {
            KEYBLOB_PRINTF("Flashloader: %s, allocate page %x as data page.\n", __func__, s_caamBlobContext.dataPage);
        }

        /* Allocate the blob partition to the job ring. */
        status = secure_memory_allocate_partition(base, s_caamBlobContext.jobRing, &s_caamBlobContext.keyPartition,
                                                  kSecureMemory_Permission_FullAccess, kSecureMemory_Allocate_Flag_Any);
        if (status != kStatus_Success)
        {
            KEYBLOB_PRINTF("Flashloader: %s, failed to allocate key partition.\n", __func__);
            break;
        }
        else
        {
            KEYBLOB_PRINTF("Flashloader: %s, allocate partition %x as key partition.\n", __func__,
                           s_caamBlobContext.keyPartition);
        }

        /* Allocate the blob page to the partition. */
        status = secure_memory_allocate_page(base, s_caamBlobContext.jobRing, s_caamBlobContext.keyPartition,
                                             &s_caamBlobContext.keyPage, kSecureMemory_Allocate_Flag_Any);
        if (status != kStatus_Success)
        {
            KEYBLOB_PRINTF("Flashloader: %s, failed to allocate key page.\n", __func__);
            break;
        }
        else
        {
            KEYBLOB_PRINTF("Flashloader: %s, allocate page %x as key page.\n", __func__, s_caamBlobContext.keyPage);
        }

        uint32_t dataPageAddress =
            BL_FEATURE_SECURE_MEMORY_BASE + s_caamBlobContext.dataPage * secure_memory_get_page_size(base);
        s_caamBlobContext.inputRing = (caam_job_ring_input_t *)(dataPageAddress + CAAM_INPUT_RING_OFFSET);
        s_caamBlobContext.outputRing = (caam_job_ring_output_t *)(dataPageAddress + CAAM_OUTPUT_RING_OFFSET);
        s_caamBlobContext.jobDscAddr = dataPageAddress + CAAM_JOB_DSC_OFFSET;
        s_caamBlobContext.outputAddr = dataPageAddress + CAAM_BLOB_OFFSET;

        s_caamBlobContext.inputAddr =
            BL_FEATURE_SECURE_MEMORY_BASE + s_caamBlobContext.keyPage * secure_memory_get_page_size(base);

        KEYBLOB_PRINTF("Flashloader: %s, inputRing = %x.\n", __func__, s_caamBlobContext.inputRing);
        KEYBLOB_PRINTF("Flashloader: %s, outputRing = %x.\n", __func__, s_caamBlobContext.outputRing);
        KEYBLOB_PRINTF("Flashloader: %s, jobDscAddr = %x.\n", __func__, s_caamBlobContext.jobDscAddr);
        KEYBLOB_PRINTF("Flashloader: %s, inputAddr = %x.\n", __func__, s_caamBlobContext.inputAddr);
        KEYBLOB_PRINTF("Flashloader: %s, outputAddr = %x.\n", __func__, s_caamBlobContext.outputAddr);

    } while (0);
    return status;
}

static inline status_t secure_memory_deinit(CAAM_Type *base)
{
    status_t status = kStatus_Fail;

    do
    {
        /* Release all pages on the data partition*/
        status = secure_memory_release_partition(base, s_caamBlobContext.jobRing, s_caamBlobContext.dataPartition);
        if (status != kStatus_Success)
        {
            KEYBLOB_PRINTF("Flashloader: %s, failed to release data partition.\n", __func__);
            break;
        }
        /* Release all pages on the key partition*/
        status = secure_memory_release_partition(base, s_caamBlobContext.jobRing, s_caamBlobContext.keyPartition);
        if (status != kStatus_Success)
        {
            KEYBLOB_PRINTF("Flashloader: %s, failed to release key partition.\n", __func__);
            break;
        }
    } while (0);

    return status;
}

static inline uint32_t secure_memory_get_partition_count(CAAM_Type *base)
{
    return ((base->SMVID_MS & CAAM_SMVID_MS_NPRT_MASK) >> CAAM_SMVID_MS_NPRT_SHIFT) + 1;
}

static inline uint32_t secure_memory_get_page_size(CAAM_Type *base)
{
    return (1 << ((base->SMVID_LS & CAAM_SMVID_LS_PSIZ_MASK) >> CAAM_SMVID_LS_PSIZ_SHIFT)) * 1024;
}

static inline uint32_t secure_memory_get_page_count(CAAM_Type *base)
{
    return ((base->SMVID_MS & CAAM_SMVID_MS_NPAG_MASK) >> CAAM_SMVID_MS_NPAG_SHIFT) + 1;
}

static status_t secure_memory_command(CAAM_Type *base,
                                      uint32_t jobRing,
                                      uint32_t partition,
                                      uint32_t page,
                                      uint32_t cmd,
                                      uint32_t timeoutMs,
                                      uint32_t flag)
{
    KEYBLOB_PRINTF(
        "Flashloader: %s, start to execute secure memory command, jobring = %x, partiton =%x, page = %x, cmd = %x, flag "
        "= %x.\n",
        __func__, jobRing, partition, page, cmd, flag);

    status_t status = kStatus_Fail;

    __IO uint32_t *smCmd = NULL;
    __I uint32_t *smStatus = NULL;

    /* Get the command register address. */
    if (flag == kSecureMemory_Command_Flag_PG)
    {
        smCmd = &base->SMCR_PG0;
        smStatus = &base->SMCSR_PG0;
    }
    else if (flag == kSecureMemory_Command_Flag_JR)
    {
        smCmd = &base->JOBRING[jobRing].SMCR_JR;
        smStatus = &base->JOBRING[jobRing].SMCSR_JR;
    }
    else
    {
        return kStatus_InvalidArgument;
    }

    /* Execute the commnad*/
    /*
     * Note: use CAAM_SMCR_JR_PAGE, CAAM_SMCR_JR_PRTN and CAAM_SMCR_JR_CMD is also ok. SMCR has the same bit filed for
     * Page 0 and Job Rings.
     */
    *smCmd = CAAM_SMCR_PG0_PAGE(page) | CAAM_SMCR_PG0_PRTN(partition) | CAAM_SMCR_PG0_CMD(cmd);
    KEYBLOB_PRINTF("Flashloader: %s, execute command [%x] = %x.\n", __func__, (uint32_t)smCmd,
                   CAAM_SMCR_PG0_PAGE(page) | CAAM_SMCR_PG0_PRTN(partition) | CAAM_SMCR_PG0_CMD(cmd));

    const uint64_t startTicks = microseconds_get_ticks();
    uint64_t timeOutTicks = microseconds_convert_to_ticks(timeoutMs * 1000);
    uint64_t endTicks = startTicks;
    uint64_t deltaTicks = 0;

    /* Wait the command is completed, or an error happens, or timeout reaches. */
    while (((*smStatus & CAAM_SMCSR_PG0_CERR_MASK) >> CAAM_SMCSR_PG0_CERR_SHIFT) ==
           kSecureMemory_Command_Status_NotCompleted)
    {
        endTicks = microseconds_get_ticks();
        deltaTicks = endTicks - startTicks;

        /* Check timer roll over */
        if (endTicks < startTicks)
        {
            deltaTicks = endTicks + (~startTicks) + 1;
        }

        if (timeOutTicks && (deltaTicks >= timeOutTicks))
        {
            return kStatus_Timeout;
        }
    }

    /* Check the result, and no error. */
    if ((*smStatus & (CAAM_SMCSR_PG0_CERR_MASK | CAAM_SMCSR_PG0_AERR_MASK)) == 0)
    {
        KEYBLOB_PRINTF("Flashloader: %s, execute secure memory successfully.\n", __func__);
        status = kStatus_Success;
    }
    else
    {
        KEYBLOB_PRINTF("Flashloader: %s, execute secure memory failed, status = %x.\n", __func__, *smStatus);
    }

    return status;
}

static void secure_memory_restore_configuration(CAAM_Type *base)
{
    /* Get the DID of current job ring. */
    uint32_t jrDid = (base->JRADID[s_caamBlobContext.jobRing].JRDID_MS & CAAM_JRDID_MS_PRIM_DID_MASK) >>
                     CAAM_JRDID_MS_PRIM_DID_SHIFT;
    /* Clear the configuration and the lock bits. */
    base->JOBRING[s_caamBlobContext.jobRing].PX_JR[s_caamBlobContext.keyPartition].PX_SMAPR_JR = 0;
    /* Set the bus master DID for group 1. */
    base->JOBRING[s_caamBlobContext.jobRing].PX_JR[s_caamBlobContext.keyPartition].PX_SMAG1_JR = 1 << jrDid;
    /* Clear the configurations in group 2. */
    base->JOBRING[s_caamBlobContext.jobRing].PX_JR[s_caamBlobContext.keyPartition].PX_SMAG2_JR = 0;
    /* Sey the permissions and lock bits. */
    base->JOBRING[s_caamBlobContext.jobRing].PX_JR[s_caamBlobContext.keyPartition].PX_SMAPR_JR =
        kSecureMemory_Permission_KeyBlobGen;
}

static status_t secure_memory_allocate_partition(
    CAAM_Type *base, uint32_t jobRing, uint32_t *partition, uint32_t permission, uint32_t flag)
{
    status_t status = kStatus_Fail;

    uint32_t i = 0;
    uint32_t partCount = 0;
    if (flag == kSecureMemory_Allocate_Flag_Any)
    {
        /* Allocate any partition is OK. */
        i = 0;
        partCount = secure_memory_get_partition_count(base);
    }
    else
    {
        /* Only allocate the partition specified by the argument partition. */
        i = *partition;
        partCount = *partition + 1;
    }

    uint32_t partStates = CAAM->JOBRING[0].SMPO_JR;
    KEYBLOB_PRINTF("Flashloader: %s, partition count = %x, owner state = %x.\n", __func__, partCount, partStates);

    for (; i < partCount; i++)
    {
        uint32_t curPartState = (partStates >> (i * 2)) & 0x3;
        if (curPartState == kSecureMemory_Partition_State_Available)
        {
            break;
        }
        else if (curPartState == kSecureMemory_Partition_State_OwnedByCurrent)
        {
            /* Current partition is owned by the job ring. But considering it might used for other task, not use it for
             * key blob. */
            continue;
        }
        else
        {
            /*
             * kSecureMemory_Partition_State_NotExist, current partition does not exist.
             * kSecureMemory_Partition_State_OwnedByOther, current partition is owned by others.
             * Skip to the next partition.
             */
            continue;
        }
    }

    if (i < partCount)
    {
        /* Allocate the partion. */

        /* Get the DID of current job ring. */
        uint32_t jrDid = (base->JRADID[jobRing].JRDID_MS & CAAM_JRDID_MS_PRIM_DID_MASK) >> CAAM_JRDID_MS_PRIM_DID_SHIFT;
        KEYBLOB_PRINTF("Flashloader: %s, Job Ring DID = %x.\n", __func__, jrDid);

        /* Clear the configuration and the lock bits. */
        base->JOBRING[jobRing].PX_JR[i].PX_SMAPR_JR = 0;
        /* Set the bus master DID for group 1. */
        base->JOBRING[jobRing].PX_JR[i].PX_SMAG1_JR = 1 << jrDid;
        /* Clear the configurations in group 2. */
        base->JOBRING[jobRing].PX_JR[i].PX_SMAG2_JR = 0x0;
        /* Set the permissions and lock bits. */
        base->JOBRING[jobRing].PX_JR[i].PX_SMAPR_JR = permission;

        *partition = i;
        status = kStatus_Success;
    }
    else
    {
        status = kStatus_Fail;
        /* No free partition is avaible, return failure.*/
    }

    return status;
}

static inline status_t secure_memory_release_default_partition(CAAM_Type *base)
{
    /* Release partition 0 owned by Page 0. */
    return secure_memory_command(base, 0U /*Job Ring is ignored*/, 0U /*Default partition index*/,
                                 0U /*Page is ignored*/, kSecureMemory_Command_Tag_DeallocatePartition,
                                 CAAM_TIMEOUT_SECURE_MEMORY_CMD, kSecureMemory_Command_Flag_PG);
}

static inline status_t secure_memory_release_partition(CAAM_Type *base, uint32_t jobRing, uint32_t partition)
{
    return secure_memory_command(base, jobRing, partition, 0U /*Page is ignored*/,
                                 kSecureMemory_Command_Tag_DeallocatePartition, CAAM_TIMEOUT_SECURE_MEMORY_CMD,
                                 kSecureMemory_Command_Flag_JR);
}

static status_t secure_memory_allocate_page(
    CAAM_Type *base, uint32_t jobRing, uint32_t partition, uint32_t *page, uint32_t flag)
{
    status_t status = kStatus_Fail;

    uint32_t i = 0;
    uint32_t pageCount = 0;
    if (flag == kSecureMemory_Allocate_Flag_Any)
    {
        /* Allocate any page is OK. */
        i = 0;
        pageCount = secure_memory_get_page_count(base);
    }
    else
    {
        /* Only allocate the partition specified by argument partition. */
        i = *page;
        pageCount = *page + 1;
    }
    KEYBLOB_PRINTF("Flashloader: %s, page count = %x.\n", __func__, pageCount);

    for (; i < pageCount; /* Empty, not every time to increase i. */)
    {
        /* Perform page inquiry command to check the ownership of the selected page. */
        status = secure_memory_command(base, jobRing, 0U /*Partition is ignored*/, i,
                                       kSecureMemory_Command_Tag_PageInquiry, CAAM_TIMEOUT_SECURE_MEMORY_CMD,
                                       kSecureMemory_Command_Flag_JR);
        if (status != kStatus_Success)
        {
            break;
        }

        uint32_t pageState = (base->JOBRING[jobRing].SMCSR_JR & CAAM_SMCSR_JR_PO_MASK) >> CAAM_SMCSR_JR_PO_SHIFT;
        KEYBLOB_PRINTF("Flashloader: %s, pageState = %x.\n", __func__, pageState);

        if (pageState == kSecureMemory_Page_State_OwnedByCurrent)
        {
            /* The selected page is already owned by the jobring, then check the partition. */
            uint32_t pagePartition =
                (base->JOBRING[jobRing].SMCSR_JR & CAAM_SMCSR_JR_PRTN_MASK) >> CAAM_SMCSR_JR_PRTN_SHIFT;

            KEYBLOB_PRINTF("Flashloader: %s,pagePartition = %x.\n", __func__, pagePartition);

            if (pagePartition != partition)
            {
                /* If the page is not allocated to the specific partition, skip to next page.*/
                i++;
                continue;
            }
            else
            {
                /* The selected page is already allocated to the partition, return success directly. */
                status = kStatus_Success;
                break;
            }
        }
        else if (pageState == kSecureMemory_Page_State_Available)
        {
            /* Allocate the page. */
            status = secure_memory_command(base, jobRing, partition, i, kSecureMemory_Command_Tag_AllocatePage,
                                           CAAM_TIMEOUT_SECURE_MEMORY_CMD, kSecureMemory_Command_Flag_JR);
            if (status != kStatus_Success)
            {
                break;
            }
            else
            {
                /* Re-execute inquiry to make sure the page is allocated. */
                continue;
            }
        }
        else if (pageState == kSecureMemory_Page_State_NotAvailable)
        {
            /* The page is not initialized, and need to wait. */
            continue;
        }
        else if (pageState == kSecureMemory_Page_State_OwnedByOther)
        {
            /* The selected page is owned by another job ring. skip to next page.*/
            i++;
            continue;
        }
        else
        {
            /* Invalid values, return failure.*/
            status = kStatus_Fail;
            break;
        }
    }

    if (status == kStatus_Success)
    {
        if (i < pageCount)
        {
            *page = i;
        }
        else
        {
            /* No free page is available, return failure.*/
            status = kStatus_Fail;
        }
    }

    return status;
}

static inline status_t secure_memory_release_page(CAAM_Type *base, uint32_t jobRing, uint32_t page)
{
    return secure_memory_command(base, jobRing, 0U /*Partition is ignored*/, page,
                                 kSecureMemory_Command_Tag_DeallocatePage, CAAM_TIMEOUT_SECURE_MEMORY_CMD,
                                 kSecureMemory_Command_Flag_JR);
}
#endif // #if BL_FEATURE_KEYBLOB_USE_SECURE_MEMORY

static inline status_t snvs_master_zmk_enable(SNVS_Type *base)
{
    status_t status = kStatus_Fail;

    do
    {
        /* Check if ZMK writting operation is allowed or not. */
        if ((base->HPLR & SNVS_HPLR_ZMK_WSL_MASK) || (base->LPLR & SNVS_LPLR_ZMK_WHL_MASK))
        {
            status = kStatus_Fail;
            break;
        }

        /* Clear power glitch */
        if (base->LPSR & SNVS_LPSR_LVD_MASK)
        {
            /* Write the power glitch proper value. */
            base->LPLVDR = SNVS_LP_LVD_VALUE;
            /* Write 1 clear */
            base->LPSR |= SNVS_LPSR_LVD_MASK;
            /* Re-check if the power glitch is cleared. */
            if (base->LPSR & SNVS_LPSR_LVD_MASK)
            {
                /* If power glitch is not cleared, it means that the power glitch is existing on current hardware. ZMK
                 * cannot be generated. */
                status = kStatus_Fail;
                break;
            }
        }

        /* Set the hardware programming mode. */
        base->LPMKCR |= SNVS_LPMKCR_ZMK_HWP_MASK;
        __DSB();
        __ISB();

        /* Start to generate ZMK. */
        base->HPCOMR |= SNVS_HPCOMR_PROG_ZMK_MASK;
        /* Wait for the completation. */
        while (!(base->LPMKCR & SNVS_LPMKCR_ZMK_VAL_MASK))
            ;

        /* enable ZMK ECC*/
        base->LPMKCR |= SNVS_LPMKCR_ZMK_ECC_EN_MASK;

        status = kStatus_Success;

    } while (0);

    if (status != kStatus_Success)
    {
        KEYBLOB_PRINTF("Flashloader: %s, snvs zmk enablement error %x.\n", __func__, status);
    }
    else
    {
        KEYBLOB_PRINTF("Flashloader: %s, snvs zmk enablement success.\n", __func__);
    }

    return status;
}

static inline status_t snvs_master_key_select(SNVS_Type *base, uint32_t keySel)
{
    assert(base);

    status_t status = kStatus_Fail;

    do
    {
        if (keySel > ((SNVS_LPMKCR_MASTER_KEY_SEL_MASK + 1) >> SNVS_LPMKCR_MASTER_KEY_SEL_SHIFT))
        {
            /* keySel value is not in the range of key selection. */
            status = kStatus_InvalidArgument;
            break;
        }

        uint32_t state = (base->HPSR & SNVS_HPSR_SSM_STATE_MASK) >> SNVS_HPSR_SSM_STATE_SHIFT;
        KEYBLOB_PRINTF("Flashloader: %s, snvs state = %x.\n", __func__, state);
        if ((state != kSNVS_Security_State_NonSecure) && (state != kSNVS_Security_State_Trusted) &&
            (state != kSVNS_Security_State_Secure))
        {
            /* SNVS state is incorrect. */
            status = kStatus_Fail;
            break;
        }

        if (((keySel == kKeySource_ZMK) || (keySel == kKeySource_CMK)) && (!(base->LPMKCR & SNVS_LPMKCR_ZMK_VAL_MASK)))
        {
            /* ZMK is selected, but it is invalid, start to generate one. */
            status = snvs_master_zmk_enable(base);
            if (status != kStatus_Success)
            {
                break;
            }
            /* Double check if ZMK is valid.*/
            if (!(base->LPMKCR & SNVS_LPMKCR_ZMK_VAL_MASK))
            {
                status = kStatus_Fail;
                break;
            }
        }

        uint32_t curKeySel =
            (base->HPCOMR & SNVS_HPCOMR_MKS_EN_MASK) ?
                ((base->LPMKCR & SNVS_LPMKCR_MASTER_KEY_SEL_MASK) >> SNVS_LPMKCR_MASTER_KEY_SEL_SHIFT) :
                kKeySource_OTPMK;
        if (keySel == curKeySel)
        {
            /* No operation is needed. */
            status = kStatus_Success;
            break;
        }

        if ((base->HPLR & SNVS_HPLR_MKS_SL_MASK) || (base->LPLR & SNVS_LPLR_MKS_HL_MASK))
        {
            /* SNVS master selection is locked. So the change cannot be applied. */
            status = kStatus_Fail;
            break;
        }

        /* Enable the master key select and select the specified key.*/
        base->HPCOMR |= SNVS_HPCOMR_MKS_EN_MASK;
        base->LPMKCR = (base->LPMKCR & (~SNVS_LPMKCR_MASTER_KEY_SEL_MASK)) | SNVS_LPMKCR_MASTER_KEY_SEL(keySel);

        status = kStatus_Success;
    } while (0);

    if (status != kStatus_Success)
    {
        KEYBLOB_PRINTF("Flashloader: %s, snvs master key(%x)selection error %x.\n", __func__, keySel, status);
    }
    else
    {
        KEYBLOB_PRINTF("Flashloader: %s, snvs master key(%x)selection success.\n", __func__, keySel);
    }

    return status;
}

static inline status_t caam_init(CAAM_Type *base)
{
    assert(base);

    /* Initialize job ring addresses */
    if (base->MCFGR & CAAM_MCFGR_DWT_MASK)
    {
        /* Big endian for double words sequence. */
        base->JOBRING[s_caamBlobContext.jobRing].IRBAR_JR = (uint64_t)((size_t)s_caamBlobContext.inputRing) << 32;
        base->JOBRING[s_caamBlobContext.jobRing].ORBAR_JR = (uint64_t)((size_t)s_caamBlobContext.outputRing) << 32;
    }
    else
    {
        /* Little endian for double words sequence. */
        base->JOBRING[s_caamBlobContext.jobRing].IRBAR_JR = (uint64_t)((size_t)s_caamBlobContext.inputRing);
        base->JOBRING[s_caamBlobContext.jobRing].ORBAR_JR = (uint64_t)((size_t)s_caamBlobContext.outputRing);
    }

    /* Initialize job ring sizes to 1 */
    base->JOBRING[s_caamBlobContext.jobRing].IRSR_JR = CAAM_JOB_RING_ENTRIES;
    base->JOBRING[s_caamBlobContext.jobRing].ORSR_JR = CAAM_JOB_RING_ENTRIES;

    /* Disables interrupts for JR0, and use polling mode. */
    base->JOBRING[s_caamBlobContext.jobRing].JRCFGR_JR_LS |= CAAM_JRCFGR_JR_LS_IMSK_MASK;

    return kStatus_Success;
}

static inline status_t caam_wait_idle(CAAM_Type *base, uint32_t timeoutMs)
{
    const uint64_t startTicks = microseconds_get_ticks();

    uint64_t timeOutTicks = microseconds_convert_to_ticks(timeoutMs * 1000);
    uint64_t endTicks = startTicks;
    uint64_t deltaTicks = 0;

    /* Wait CSTA[IDLE] to be set, and CSTA[BSY] to be cleared, and IRSAR_JR0 to be cleared. */
    while (((!(base->CSTA & CAAM_CSTA_IDLE_MASK)) || (base->CSTA & CAAM_CSTA_BSY_MASK)) &&
           (base->JOBRING[s_caamBlobContext.jobRing].IRSAR_JR & CAAM_IRSAR_JR_IRSA_MASK))
    {
        endTicks = microseconds_get_ticks();
        deltaTicks = endTicks - startTicks;

        /* Check timer roll over. */
        if (endTicks < startTicks)
        {
            deltaTicks = endTicks + (~startTicks) + 1;
        }

        if (timeOutTicks && (deltaTicks >= timeOutTicks))
        {
            return kStatus_Timeout;
        }
    }

    return kStatus_Success;
}

static inline status_t caam_wait_done(CAAM_Type *base, uint32_t timeoutMs)
{
    const uint64_t startTicks = microseconds_get_ticks();

    uint64_t timeOutTicks = microseconds_convert_to_ticks(timeoutMs * 1000);
    uint64_t endTicks = startTicks;
    uint64_t deltaTicks = 0;

    /* Wait input slot is empty, and output slot is available. */
    while (!(base->JOBRING[s_caamBlobContext.jobRing].ORSFR_JR & CAAM_ORSFR_JR_ORSF_MASK))
    {
        endTicks = microseconds_get_ticks();
        deltaTicks = endTicks - startTicks;

        /* Check timer roll over. */
        if (endTicks < startTicks)
        {
            deltaTicks = endTicks + (~startTicks) + 1;
        }

        if (timeOutTicks && (deltaTicks >= timeOutTicks))
        {
            return kStatus_Timeout;
        }
    }

    return kStatus_Success;
}

static status_t caam_run(CAAM_Type *base)
{
    status_t status = kStatus_Fail;

    do
    {
        status = caam_wait_idle(base, CAAM_TIMEOUT_WAIT_BUSY);
        if (status != kStatus_Success)
        {
            break;
        }

        /* Clear success status of the previous operation. */
        status = kStatus_Fail;

        /* For debugging, print the descriptor table.*/
        uint32_t *pDsc = (uint32_t *)s_caamBlobContext.jobDscAddr;
        uint32_t pDscWords = CAAM_DSC_GET_LENGTH_IN_WORD(pDsc);
        KEYBLOB_PRINTF("Flashloader: %s, Dump Job Ring Descriptor:", __func__);
        for (int i = 0; i < pDscWords; i++)
        {
            KEYBLOB_PRINTF("\tDSC[%d] @%x = %x.\n", i, (uint32_t)&pDsc[i], pDsc[i]);
        }

        /* Clear the output. */
        s_caamBlobContext.outputRing->dscAddr = 0;
        s_caamBlobContext.outputRing->status = 0;
        /* Add job to input ring */
        s_caamBlobContext.inputRing->dscAddr = (uint32_t)s_caamBlobContext.jobDscAddr;
        /* Make sure the previous operation is done before executing the job to avoid incorrect execution. */
        __DSB();
        __ISB();

        /* Increment jobs added */
        base->JOBRING[s_caamBlobContext.jobRing].IRJAR_JR = 0x1;

        /* Wait for job ring to complete the job infinitly. */
        caam_wait_done(base, CAAM_TIMEOUT_RUN_INFINITE);

        /* Check that descriptor address is the one expected in the out ring */
        if (s_caamBlobContext.outputRing->dscAddr == s_caamBlobContext.jobDscAddr)
        {
            /* Check if any error is reported in the output ring */
            if (s_caamBlobContext.outputRing->status != CAAM_JOB_TERMINITION_STATUS_SUCCESS)
            {
                KEYBLOB_PRINTF("Flashloader: %s, [Error]CAAM job completed with errors %x.\n", __func__,
                               s_caamBlobContext.outputRing->status);
            }
            else
            {
                status = kStatus_Success;
                KEYBLOB_PRINTF("Flashloader: %s, [Success]CAAM job completed successfully.\n", __func__);
            }
        }
        else
        {
            KEYBLOB_PRINTF(
                "Flashloader: %s, [Error]CAAM job output ring descriptor address does not match, %x != %x, status = "
                "%x.\n",
                __func__, s_caamBlobContext.jobDscAddr, s_caamBlobContext.outputRing->dscAddr,
                s_caamBlobContext.outputRing->status);
        }
    } while (0);

    /* Remove job fomr job ring output queue.*/
    base->JOBRING[s_caamBlobContext.jobRing].ORJRR_JR = 0x1;

    return status;
}

static inline status_t rng_inst(CAAM_Type *base)
{
    status_t status = kStatus_Fail;

    do
    {
        if (base->RDSTA & CAAM_RDSTA_IF0_MASK)
        {
            /* If RNG already instantiated then skip it */
            status = kStatus_Success;
            KEYBLOB_PRINTF("Flashloader: %s, [Success]RNG instantiation is already completed by previous operation.\n",
                           __func__);
            break;
        }

        uint32_t *pRngDsc = (uint32_t *)s_caamBlobContext.jobDscAddr;
        for (int i = 0; i < (sizeof(s_rngInstDsc) / sizeof(s_rngInstDsc[0])); i++)
        {
            pRngDsc[i] = s_rngInstDsc[i];
        }

        /* Clear the ERR bit in RTMCTL.*/
        base->RTMCTL |= CAAM_RTMCTL_ERR_MASK;

        status = caam_run(base);
        if (status != kStatus_Success)
        {
            break;
        }

        /* Clear the previous success status. */
        status = kStatus_Fail;

        /* Ensure that the RNG was correctly instantiated at non-test mode. */
        if (!((base->RDSTA & CAAM_RDSTA_IF0_MASK) && (base->RDSTA & CAAM_RDSTA_SKVN_MASK)))
        {
            if (!(base->RDSTA & CAAM_RDSTA_SKVN_MASK))
            {
                KEYBLOB_PRINTF("Flashloader: %s, [Error]RNG instantiation is opearated at test mode.\n", __func__);
            }
            else
            {
                KEYBLOB_PRINTF("Flashloader: %s, [Error]RNG instantiation failed.\n", __func__);
            }
        }
        else
        {
            status = kStatus_Success;
            KEYBLOB_PRINTF("Flashloader: %s, [Success]RNG instantiation is completed.\n", __func__);
        }

    } while (0);

    return status;
}

static status_t caam_gen_blob(CAAM_Type *base, uint32_t keySize)
{
    /* Calculate the byte count of the Key Blob. */
    uint32_t blobSize = kKeyBlob_BK_MaxSize + keySize + kKeyBlob_MACSize;
    /* Clear the output data buffer. */
    memset((void *)s_caamBlobContext.outputAddr, 0, blobSize);

    uint32_t *const pBlobEncapDsc = (uint32_t *)s_caamBlobContext.jobDscAddr;

    for (int i = 0; i < (sizeof(s_blobEncapDsc) / sizeof(s_blobEncapDsc[0])); i++)
    {
        pBlobEncapDsc[i] = s_blobEncapDsc[i];
    }

    /* Update the key modifier. */
    pBlobEncapDsc[CAAM_ENCAP_BLOB_DESC_KEYMODIFIER_INDEX] =
        (pBlobEncapDsc[CAAM_ENCAP_BLOB_DESC_KEYMODIFIER_INDEX] & (~CAAM_DSC_KEY_MODIFIER_LENGTH_MASK)) |
        CAAM_DSC_KEY_MODIFIER_LENGTH(keySize);
    /* Fill in the input key length. */
    pBlobEncapDsc[CAAM_ENCAP_BLOB_DESC_SEQINPTR_INDEX] =
        (pBlobEncapDsc[CAAM_ENCAP_BLOB_DESC_SEQINPTR_INDEX] & (~CAAM_DSC_SEQ_IN_PTR_LENGTH_MASK)) |
        CAAM_DSC_SEQ_IN_PTR_LENGTH(keySize);
    /* Fill in the address where the data key resides. */
    pBlobEncapDsc[CAAM_ENCAP_BLOB_DESC_SEQINPTR_INDEX + 1] = s_caamBlobContext.inputAddr;
    /* Fill in the output key blob length. */
    pBlobEncapDsc[CAAM_ENCAP_BLOB_DESC_SEQINPTR_INDEX + 2] =
        (pBlobEncapDsc[CAAM_ENCAP_BLOB_DESC_SEQINPTR_INDEX + 2] & (~CAAM_DSC_SEQ_OUT_PTR_LENGTH_MASK)) |
        CAAM_DSC_SEQ_OUT_PTR_LENGTH(blobSize);
    /* Fill in output blob addr. */
    pBlobEncapDsc[CAAM_ENCAP_BLOB_DESC_SEQINPTR_INDEX + 3] = s_caamBlobContext.outputAddr;

    return caam_run(base);
}

status_t generate_key_blob(const uint8_t *pKeyAddr,
                           const uint32_t keySize,
                           const uint32_t keySel,
                           uint8_t *pKeyBlobAddr,
                           uint32_t *pKeyBlobSize)
{
    status_t status = kStatus_InvalidArgument;

    *pKeyBlobSize = 0;

#if BL_FEATURE_KEYBLOB_USE_SECURE_MEMORY
    s_caamBlobContext.jobRing = CAAM_JOB_RING_INDEX;
    s_caamBlobContext.base = CAAM;
    s_caamBlobContext.snvsBase = SNVS;
#endif

    do
    {
        /* Check input pointer. */
        if ((pKeyAddr == NULL) || (pKeyBlobAddr == NULL) || (pKeyBlobSize == NULL))
        {
            break;
        }
        /* Check key size. CAAM only supports 128, 192 and 256 bit AES*/
        if ((keySize != kKeyBlob_Header_Size_128Bit) && (keySize != kKeyBlob_Header_Size_192Bit) &&
            (keySize != kKeyBlob_Header_Size_256Bit))
        {
            break;
        }

        if ((keySel != kKeySource_OTPMK) && (keySel != kKeySource_OTPMK_Sub) && (keySel != kKeySource_ZMK) &&
            (keySel != kKeySource_CMK))
        {
            break;
        }

#if BL_FEATURE_KEYBLOB_USE_SECURE_MEMORY
        /* Secure memory initialization */
        status = secure_memory_init(s_caamBlobContext.base);
        if (status != kStatus_Success)
        {
            break;
        }
#endif

        /* CAAM initialization. */
        /*
         * Note: Do not perform CAAM initialization before secure memory initialization. Job ring context is based on
         * the result of secure memory allocation.
         */
        status = caam_init(s_caamBlobContext.base);
        if (status != kStatus_Success)
        {
            break;
        }

        /* RNG instantiation, which used to generate blob key(BK). */
        status = rng_inst(s_caamBlobContext.base);
        if (status != kStatus_Success)
        {
            break;
        }

        /* Set the SNVS master key. */
        status = snvs_master_key_select(s_caamBlobContext.snvsBase, keySel);
        if (status != kStatus_Success)
        {
            break;
        }

        /* Copy Data Encryption Key to the context*/
        memcpy((void *)s_caamBlobContext.inputAddr, pKeyAddr, keySize);
#if BL_FEATURE_KEYBLOB_USE_SECURE_MEMORY
        /* Restore the configuration of key partition. */
        secure_memory_restore_configuration(s_caamBlobContext.base);
#endif
        /* Generate the blob to encapsulate the DEK */
        status = caam_gen_blob(s_caamBlobContext.base, keySize);
        if (status != kStatus_Success)
        {
            *pKeyBlobSize = 0;
            KEYBLOB_PRINTF("Flashloader: %s, error during blob encapsulation operation: %x.\n", __func__, status);
            break;
        }
        /* Calculate the byte count of the Key Blob. */
        uint32_t blobSize = sizeof(keyblob_header_t) + kKeyBlob_BK_MaxSize + keySize + kKeyBlob_MACSize;
        /* Initialize the header of the Key Blob. */
        keyblob_block_t *keyBlob = (keyblob_block_t *)pKeyBlobAddr;
        keyBlob->header.tag = kKeyBlob_Header_Tag;
        keyBlob->header.version = kKeyBlob_Header_Version;
        keyBlob->header.mode = kKeyBlob_Header_Mode_CCM;
        keyBlob->header.alg = kKeyBlob_Header_Algorithm_AES;
        keyBlob->header.size = keySize;
        keyBlob->header.flag = 0;
        keyBlob->header.len[0] = blobSize >> 8;
        keyBlob->header.len[1] = blobSize & 0xFF;

        /* Copy the body of the generated Key Blob to the destination.*/
        memcpy(keyBlob->blob, (void *)s_caamBlobContext.outputAddr, blobSize - sizeof(keyblob_header_t));

        /* Set the Key Blob size only when very operation is done. */
        *pKeyBlobSize = blobSize;

#if BL_FEATURE_KEYBLOB_USE_SECURE_MEMORY
        /* Free the secure memory. */
        secure_memory_deinit(s_caamBlobContext.base);
#endif

        /* Print the generated key blob for debugging. */
        KEYBLOB_PRINTF("Flashloader: %s, Key Blob is available and equals:", __func__);
        for (size_t i = 0; i < (blobSize / sizeof(uint32_t)); i++)
        {
            KEYBLOB_PRINTF("%x", ((uint32_t *)keyBlob)[i]);
        }

    } while (0);

    return status;
}

void generate_key_blob_selftest(void)
{
    uint32_t keyBlobSize = 0;
    /* Self test for AES-128bit. */
    generate_key_blob(dek_128_example, sizeof(dek_128_example), kKeySource_OTPMK, keyblob_128_example, &keyBlobSize);
    if (keyBlobSize != sizeof(keyblob_128_example))
    {
        assert(false);
    }

    /* Self test for AES-256bit. */
    generate_key_blob(dek_256_example, sizeof(dek_256_example), kKeySource_CMK, keyblob_256_example, &keyBlobSize);
    if (keyBlobSize != sizeof(keyblob_256_example))
    {
        assert(false);
    }
}
