SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE " \
    -DNDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -mcpu=cortex-m33 \
    -Wall \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -mthumb \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE " \
    -DNDEBUG \
    -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 \
    -DCPU_MIMXRT685SFVKB_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DARM_MATH_CM33 \
    -DBL_TARGET_FLASH \
    -DMIMXRT685S \
    -Os \
    -mcpu=cortex-m33 \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
")
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE " \
    -DNDEBUG \
    -Os \
    -mcpu=cortex-m33 \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE " \
    -mcpu=cortex-m33 \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    --specs=nano.specs \
    --specs=nosys.specs \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -z \
    -Xlinker \
    muldefs \
    -Wall \
    -Xlinker \
    -Map=output.map \
    -T${ProjDirPath}/../../../../../middleware/mcu-boot/targets/common/linker/gcc/MIMXRT685S/MIMXRT685S_flash_bootloader.ld -static \
")
