/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_flash.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define BUFFER_LEN 4

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void error_trap(void);
void app_finalize(void);
static uint32_t read_word_from_byte_address(const uint8_t *src, uint32_t i);
static void write_word_to_byte_address(uint8_t *dst, uint32_t *word,
                                       uint32_t i);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Flash driver Structure */
static flexnvm_config_t s_flashDriver;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * @brief Gets called when an error occurs.
 *
 * @details Print error message and trap forever.
 */
void error_trap(void) {
  PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLASH ERROR! ----");
  while (1) {
  }
}

/*
 * @brief Gets called when the app is complete.
 *
 * @details Print finshed message and trap forever.
 */
void app_finalize(void) {
  /* Print finished message. */
  PRINTF("\r\n End of FlexNVM EEprom Example \r\n");
  while (1) {
  }
}

/*! @brief Writes word to byte address.*/
static void write_word_to_byte_address(uint8_t *dst, uint32_t *word,
                                       uint32_t i) {
  dst += i * 4;
  word += i * 1;
  if (!((uint32_t)dst % 4)) {
    *(uint32_t *)dst = *word;
  } else {
    for (uint32_t i = 0; i < 4; i++) {
      *dst = *(uint8_t *)word;
      dst++;
      word++;
    }
  }
}

/*! @brief Reads word from byte address.*/
static uint32_t read_word_from_byte_address(const uint8_t *src, uint32_t i) {
  uint32_t word = 0;
  src += i * 4;
  if (!((uint32_t)src % 4)) {
    word = *(const uint32_t *)src;
  } else {
    for (uint32_t i = 0; i < 4; i++) {
      word |= (uint32_t)(*src) << (i * 8);
      src++;
    }
  }

  return word;
}
/*!
 * @brief Use Standard Software Drivers (SSD) to modify flexram
 *
 * @details This function uses SSD to demonstrate flash mode:
 *            + Check flexram information.
 *            + Program a sector.
 */
int main(void) {
  ftfx_security_state_t securityStatus =
      kFTFx_SecurityStateNotSecure; /* Return protection status */
  status_t result; /* Return code from each flash driver function */

  /* Init hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

  /* Clean up Flash driver Structure*/
  memset(&s_flashDriver, 0, sizeof(flexnvm_config_t));

  /* print welcome message */
  PRINTF("\r\n FlexNVM EEprom Example Start \r\n");

#if defined(SIM_FCFG2_PFLSH_MASK)
  if (SIM->FCFG2 & SIM_FCFG2_PFLSH_MASK) {
    PRINTF("\r\n Current device doesn't support FlexNVM feature \r\n");
    app_finalize();
  }
#endif

  /* Setup flash driver structure for device and initialize variables. */
  result = FLEXNVM_Init(&s_flashDriver);
  if (kStatus_FTFx_Success != result) {
    error_trap();
  }

  /* Check security status. */
  result = FLEXNVM_GetSecurityState(&s_flashDriver, &securityStatus);
  if (kStatus_FTFx_Success != result) {
    error_trap();
  }

  /* Print security status. */
  switch (securityStatus) {
  case kFTFx_SecurityStateNotSecure:
    PRINTF("\r\n Flash is UNSECURE!");
    break;
  case kFTFx_SecurityStateBackdoorEnabled:
    PRINTF("\r\n Flash is SECURE, BACKDOOR is ENABLED!");
    break;
  case kFTFx_SecurityStateBackdoorDisabled:
    PRINTF("\r\n Flash is SECURE, BACKDOOR is DISABLED!");
    break;
  default:
    break;
  }
  PRINTF("\r\n");

  /* Debug message for user. */
  /* Test flexnvm dflash feature only if flash is unsecure. */
  if (kFTFx_SecurityStateNotSecure != securityStatus) {
    PRINTF("\r\n EEprom opeation will not be executed, as Flash is SECURE!");
    app_finalize();
  } else {
    uint32_t s_buffer[BUFFER_LEN];     /* Buffer for program */
    uint32_t s_buffer_rbc[BUFFER_LEN]; /* Buffer for readback */
    uint32_t destAdrss;                /* Address of the target location */

    uint32_t dflashBlockBase = 0;
    uint32_t dflashTotalSize = 0;
    uint32_t flexramBlockBase = 0;
    uint32_t flexramTotalSize = 0;

    /* Get flash properties*/
    FLEXNVM_GetProperty(&s_flashDriver, kFLEXNVM_PropertyDflashBlockBaseAddr,
                        &dflashBlockBase);
    FLEXNVM_GetProperty(&s_flashDriver, kFLEXNVM_PropertyFlexRamBlockBaseAddr,
                        &flexramBlockBase);
    FLEXNVM_GetProperty(&s_flashDriver, kFLEXNVM_PropertyFlexRamTotalSize,
                        &flexramTotalSize);

    FLEXNVM_GetProperty(&s_flashDriver, kFLEXNVM_PropertyDflashTotalSize,
                        &dflashTotalSize);

    PRINTF("\r\n Make FlexRAM available for ram ");
    result = FLEXNVM_SetFlexramFunction(&s_flashDriver,
                                        kFTFx_FlexramFuncOptAvailableAsRam);
    if (kStatus_FTFx_Success != result) {
      error_trap();
    }
    PRINTF("\r\n Now ram data is read and written by accessing the FlexRAM "
           "address space ");
    PRINTF("\r\n FlexRAM Base Address: (0x%x) ", flexramBlockBase);

    /* Print message for user. */
    /* Prepare buffer. */
    for (uint32_t i = 0; i < BUFFER_LEN; i++) {
      s_buffer[i] = i + 10;
    }
    /* Program user buffer into flexRAM*/
    PRINTF("\r\n write 16 bytes data to flexram space");
    destAdrss = flexramBlockBase;
    for (uint32_t i = 0; i < BUFFER_LEN; i++) {
      write_word_to_byte_address((uint8_t *)destAdrss, s_buffer, i);
    }

#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    /* Clean the D-Cache before reading the flash data*/
    SCB_CleanInvalidateDCache();
#endif
    /* Verify programming by reading back from flexRAM directly*/
    for (uint32_t i = 0; i < BUFFER_LEN; i++) {
      s_buffer_rbc[i] = read_word_from_byte_address((uint8_t *)destAdrss, i);
      if (s_buffer_rbc[i] != s_buffer[i]) {
        error_trap();
      }
    }

    PRINTF(
        "\r\n Successfully Programmed and Verified Location 0x%x -> 0x%x \r\n",
        destAdrss, (destAdrss + sizeof(s_buffer)));

    app_finalize();
  }

  return 0;
}
