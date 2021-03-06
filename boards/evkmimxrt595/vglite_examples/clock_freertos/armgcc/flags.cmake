SET(CMAKE_ASM_FLAGS_DEBUG " \
    -DDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -g \
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
SET(CMAKE_ASM_FLAGS_RELEASE " \
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
SET(CMAKE_ASM_FLAGS_FLASH_DEBUG " \
    -DDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -g \
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
SET(CMAKE_ASM_FLAGS_FLASH_RELEASE " \
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
SET(CMAKE_C_FLAGS_DEBUG " \
    -DDEBUG \
    -DCPU_MIMXRT595SFFOC_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DSSD1963_DATA_WITDH=8 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
    -DSDK_DEBUGCONSOLE_UART \
    -DVG_RESOLVE_ENGINE=0 \
    -DVG_PE_COLOR_KEY=0 \
    -DVG_IM_INDEX_FORMAT=0 \
    -DVG_AYUV_INPUT_OUTPUT=0 \
    -DVG_DOUBLE_IMAGE=0 \
    -DVG_RECTANGLE_STRIP_MODE=0 \
    -DVG_MMU=0 \
    -DVG_IM_FILTER=0 \
    -DVG_IM_YUV_PACKET=1 \
    -DVG_IM_YUV_PLANAR=0 \
    -DVG_PE_YUV_PACKET=1 \
    -DVG_TARGET_TILED=1 \
    -DVG_COMMAND_CALL=1 \
    -DVG_SHARE_BUFFER_IM_16K=0 \
    -DVG_OFFLINE_MODE=0 \
    -DVG_RESOLUTION_2880=0 \
    -DVG_PE_PREMULTIPLY=0 \
    -DVG_POST_CONVERTER=0 \
    -DVG_PRE_CONVERTER=0 \
    -DVG_RENDER_BY_MESH=0 \
    -DVG_TARGET_FAST_CLEAR=0 \
    -DVG_BUFFER_NUMBER_OF_TARGET=0 \
    -DVG_VIDEO_CLEAR_CONTROL=0 \
    -DVG_VIDEO_CONTROL=0 \
    -DVGLITE_TST_FIRMWARE=0 \
    -DVG_LITE_SYS_GPU_CTRL=0 \
    -DVG_RENDER_TEXT=1 \
    -DRTOS \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DFSL_RTOS_FREE_RTOS \
    -g \
    -O0 \
    -mcpu=cortex-m33 \
    -Wall \
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
SET(CMAKE_C_FLAGS_RELEASE " \
    -DNDEBUG \
    -DCPU_MIMXRT595SFFOC_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DSSD1963_DATA_WITDH=8 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
    -DSDK_DEBUGCONSOLE_UART \
    -DVG_RESOLVE_ENGINE=0 \
    -DVG_PE_COLOR_KEY=0 \
    -DVG_IM_INDEX_FORMAT=0 \
    -DVG_AYUV_INPUT_OUTPUT=0 \
    -DVG_DOUBLE_IMAGE=0 \
    -DVG_RECTANGLE_STRIP_MODE=0 \
    -DVG_MMU=0 \
    -DVG_IM_FILTER=0 \
    -DVG_IM_YUV_PACKET=1 \
    -DVG_IM_YUV_PLANAR=0 \
    -DVG_PE_YUV_PACKET=1 \
    -DVG_TARGET_TILED=1 \
    -DVG_COMMAND_CALL=1 \
    -DVG_SHARE_BUFFER_IM_16K=0 \
    -DVG_OFFLINE_MODE=0 \
    -DVG_RESOLUTION_2880=0 \
    -DVG_PE_PREMULTIPLY=0 \
    -DVG_POST_CONVERTER=0 \
    -DVG_PRE_CONVERTER=0 \
    -DVG_RENDER_BY_MESH=0 \
    -DVG_TARGET_FAST_CLEAR=0 \
    -DVG_BUFFER_NUMBER_OF_TARGET=0 \
    -DVG_VIDEO_CLEAR_CONTROL=0 \
    -DVG_VIDEO_CONTROL=0 \
    -DVGLITE_TST_FIRMWARE=0 \
    -DVG_LITE_SYS_GPU_CTRL=0 \
    -DVG_RENDER_TEXT=1 \
    -DRTOS \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DFSL_RTOS_FREE_RTOS \
    -Os \
    -mcpu=cortex-m33 \
    -Wall \
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
SET(CMAKE_C_FLAGS_FLASH_DEBUG " \
    -DDEBUG \
    -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 \
    -DCPU_MIMXRT595SFFOC_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DSSD1963_DATA_WITDH=8 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
    -DSDK_DEBUGCONSOLE_UART \
    -DVG_RESOLVE_ENGINE=0 \
    -DVG_PE_COLOR_KEY=0 \
    -DVG_IM_INDEX_FORMAT=0 \
    -DVG_AYUV_INPUT_OUTPUT=0 \
    -DVG_DOUBLE_IMAGE=0 \
    -DVG_RECTANGLE_STRIP_MODE=0 \
    -DVG_MMU=0 \
    -DVG_IM_FILTER=0 \
    -DVG_IM_YUV_PACKET=1 \
    -DVG_IM_YUV_PLANAR=0 \
    -DVG_PE_YUV_PACKET=1 \
    -DVG_TARGET_TILED=1 \
    -DVG_COMMAND_CALL=1 \
    -DVG_SHARE_BUFFER_IM_16K=0 \
    -DVG_OFFLINE_MODE=0 \
    -DVG_RESOLUTION_2880=0 \
    -DVG_PE_PREMULTIPLY=0 \
    -DVG_POST_CONVERTER=0 \
    -DVG_PRE_CONVERTER=0 \
    -DVG_RENDER_BY_MESH=0 \
    -DVG_TARGET_FAST_CLEAR=0 \
    -DVG_BUFFER_NUMBER_OF_TARGET=0 \
    -DVG_VIDEO_CLEAR_CONTROL=0 \
    -DVG_VIDEO_CONTROL=0 \
    -DVGLITE_TST_FIRMWARE=0 \
    -DVG_LITE_SYS_GPU_CTRL=0 \
    -DVG_RENDER_TEXT=1 \
    -DRTOS \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DFSL_RTOS_FREE_RTOS \
    -g \
    -O0 \
    -mcpu=cortex-m33 \
    -Wall \
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
SET(CMAKE_C_FLAGS_FLASH_RELEASE " \
    -DNDEBUG \
    -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 \
    -DCPU_MIMXRT595SFFOC_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DSSD1963_DATA_WITDH=8 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
    -DSDK_DEBUGCONSOLE_UART \
    -DVG_RESOLVE_ENGINE=0 \
    -DVG_PE_COLOR_KEY=0 \
    -DVG_IM_INDEX_FORMAT=0 \
    -DVG_AYUV_INPUT_OUTPUT=0 \
    -DVG_DOUBLE_IMAGE=0 \
    -DVG_RECTANGLE_STRIP_MODE=0 \
    -DVG_MMU=0 \
    -DVG_IM_FILTER=0 \
    -DVG_IM_YUV_PACKET=1 \
    -DVG_IM_YUV_PLANAR=0 \
    -DVG_PE_YUV_PACKET=1 \
    -DVG_TARGET_TILED=1 \
    -DVG_COMMAND_CALL=1 \
    -DVG_SHARE_BUFFER_IM_16K=0 \
    -DVG_OFFLINE_MODE=0 \
    -DVG_RESOLUTION_2880=0 \
    -DVG_PE_PREMULTIPLY=0 \
    -DVG_POST_CONVERTER=0 \
    -DVG_PRE_CONVERTER=0 \
    -DVG_RENDER_BY_MESH=0 \
    -DVG_TARGET_FAST_CLEAR=0 \
    -DVG_BUFFER_NUMBER_OF_TARGET=0 \
    -DVG_VIDEO_CLEAR_CONTROL=0 \
    -DVG_VIDEO_CONTROL=0 \
    -DVGLITE_TST_FIRMWARE=0 \
    -DVG_LITE_SYS_GPU_CTRL=0 \
    -DVG_RENDER_TEXT=1 \
    -DRTOS \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DFSL_RTOS_FREE_RTOS \
    -Os \
    -mcpu=cortex-m33 \
    -Wall \
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
SET(CMAKE_CXX_FLAGS_DEBUG " \
    -DDEBUG \
    -DSERIAL_PORT_TYPE_UART=1 \
    -g \
    -O0 \
    -mcpu=cortex-m33 \
    -Wall \
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
SET(CMAKE_CXX_FLAGS_RELEASE " \
    -DNDEBUG \
    -DSERIAL_PORT_TYPE_UART=1 \
    -Os \
    -mcpu=cortex-m33 \
    -Wall \
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
SET(CMAKE_CXX_FLAGS_FLASH_DEBUG " \
    -DDEBUG \
    -DSERIAL_PORT_TYPE_UART=1 \
    -g \
    -O0 \
    -mcpu=cortex-m33 \
    -Wall \
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
SET(CMAKE_CXX_FLAGS_FLASH_RELEASE " \
    -DNDEBUG \
    -DSERIAL_PORT_TYPE_UART=1 \
    -Os \
    -mcpu=cortex-m33 \
    -Wall \
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
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG " \
    -g \
    -mcpu=cortex-m33 \
    -Wall \
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
    -Xlinker \
    -Map=output.map \
    -T${ProjDirPath}/MIMXRT595Sxxxx_cm33_ram.ld -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE " \
    -mcpu=cortex-m33 \
    -Wall \
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
    -Xlinker \
    -Map=output.map \
    -T${ProjDirPath}/MIMXRT595Sxxxx_cm33_ram.ld -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG " \
    -g \
    -mcpu=cortex-m33 \
    -Wall \
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
    -Xlinker \
    -Map=output.map \
    -T${ProjDirPath}/MIMXRT595Sxxxx_cm33_flash.ld -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE " \
    -mcpu=cortex-m33 \
    -Wall \
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
    -Xlinker \
    -Map=output.map \
    -T${ProjDirPath}/MIMXRT595Sxxxx_cm33_flash.ld -static \
")
