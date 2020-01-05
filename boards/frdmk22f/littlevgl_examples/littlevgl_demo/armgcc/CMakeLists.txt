INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

 
# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})


SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MK22FN512VLH12")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLV_CONF_INCLUDE_SIMPLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFRDM_K22F")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFREEDOM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MK22FN512VLH12")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLV_CONF_INCLUDE_SIMPLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFRDM_K22F")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFREEDOM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MK22FN512VLH12")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MK22FN512VLH12")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Map=output.map")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/portable/GCC/ARM_CM4F)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../middleware/littlevgl)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include)

include_directories(${ProjDirPath}/../../../../../components/ili9341)

include_directories(${ProjDirPath}/../../../../../components/ft6x06)

include_directories(${ProjDirPath}/../../../../../devices/MK22F51212/drivers)

include_directories(${ProjDirPath}/../../../../../devices/MK22F51212/cmsis_drivers)

include_directories(${ProjDirPath}/../../../../../devices/MK22F51212)

include_directories(${ProjDirPath}/../../../../../devices/MK22F51212/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MK22F51212/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../CMSIS/Driver/Include)

include_directories(${ProjDirPath}/../../../../../devices/MK22F51212/utilities)

add_executable(littlevgl_demo.elf 
"${ProjDirPath}/../../RTE_Device.h"
"${ProjDirPath}/../littlevgl_demo.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../lv_conf.h"
"${ProjDirPath}/../lv_ex_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_examples/demo/demo.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_examples/demo/demo.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_examples/demo/img_bubble_pattern.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../littlevgl_support.h"
"${ProjDirPath}/../../littlevgl_support.c"
"${ProjDirPath}/../../../../../components/ili9341/fsl_ili9341.c"
"${ProjDirPath}/../../../../../components/ili9341/fsl_ili9341.h"
"${ProjDirPath}/../../../../../components/ft6x06/fsl_ft6x06.c"
"${ProjDirPath}/../../../../../components/ft6x06/fsl_ft6x06.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_edma.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_edma.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_dmamux.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_dmamux.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_dspi.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_dspi.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_dspi_edma.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_dspi_edma.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/cmsis_drivers/fsl_dspi_cmsis.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/cmsis_drivers/fsl_dspi_cmsis.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_i2c_edma.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_i2c_edma.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/cmsis_drivers/fsl_i2c_cmsis.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/cmsis_drivers/fsl_i2c_cmsis.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/portable/MemMang/heap_4.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_conf_checker.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_group.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_group.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_indev.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_indev.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_lang.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_lang.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_obj.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_obj.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_refr.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_refr.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_style.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_style.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_vdb.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_core/lv_vdb.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_arc.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_arc.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_img.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_img.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_label.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_label.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_line.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_line.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_rbasic.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_rbasic.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_rect.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_rect.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_triangle.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_triangle.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_vbasic.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_draw/lv_draw_vbasic.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_builtin.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_builtin.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_10.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_10_cyrillic.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_10_latin_sup.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_20.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_20_cyrillic.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_20_latin_sup.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_30.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_30_cyrillic.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_30_latin_sup.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_40.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_40_cyrillic.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_dejavu_40_latin_sup.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_monospace_8.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_symbol_10.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_symbol_20.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_symbol_30.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_fonts/lv_font_symbol_40.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_hal/lv_hal.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_hal/lv_hal_disp.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_hal/lv_hal_disp.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_hal/lv_hal_indev.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_hal/lv_hal_indev.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_hal/lv_hal_tick.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_hal/lv_hal_tick.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_anim.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_anim.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_area.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_area.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_circ.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_circ.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_color.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_color.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_font.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_font.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_fs.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_fs.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_gc.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_gc.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_ll.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_ll.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_log.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_log.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_math.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_math.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_mem.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_mem.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_symbol_def.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_task.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_task.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_templ.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_templ.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_txt.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_txt.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_ufs.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_misc/lv_ufs.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_arc.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_arc.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_bar.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_bar.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_btn.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_btn.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_btnm.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_btnm.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_calendar.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_calendar.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_canvas.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_canvas.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_cb.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_cb.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_chart.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_chart.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_cont.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_cont.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_ddlist.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_ddlist.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_gauge.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_gauge.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_img.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_img.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_imgbtn.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_imgbtn.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_kb.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_kb.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_label.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_label.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_led.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_led.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_line.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_line.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_list.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_list.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_lmeter.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_lmeter.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_mbox.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_mbox.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_objx_templ.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_objx_templ.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_page.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_page.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_preload.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_preload.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_roller.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_roller.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_slider.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_slider.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_spinbox.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_spinbox.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_sw.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_sw.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_ta.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_ta.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_table.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_table.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_tabview.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_tabview.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_tileview.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_tileview.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_win.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_objx/lv_win.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_alien.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_alien.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_default.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_default.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_material.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_material.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_mono.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_mono.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_nemo.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_nemo.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_night.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_night.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_templ.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_templ.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_zen.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_themes/lv_theme_zen.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_version.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/MK22F51212.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/MK22F51212_features.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/uart/uart_adapter.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/gcc/startup_MK22F51212.S"
"${ProjDirPath}/../../../../../devices/MK22F51212/system_MK22F51212.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/system_MK22F51212.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_smc.c"
"${ProjDirPath}/../../../../../devices/MK22F51212/drivers/fsl_smc.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm4.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang_ltm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv81mml.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mbl.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mml.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_I2C.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_Common.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_SPI.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/event_groups.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/atomic.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/deprecated_definitions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/event_groups.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/list.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/message_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/mpu_prototypes.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/mpu_wrappers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/portable.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/projdefs.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/queue.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/semphr.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/stack_macros.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/stream_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/task.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/timers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/list.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/portable/GCC/ARM_CM4F/portmacro.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/portable/MemMang/ReadMe.url"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/queue.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/stream_buffer.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/tasks.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/timers.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../devices/MK22F51212/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MK22FN512xxx12_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MK22FN512xxx12_flash.ld -static")

TARGET_LINK_LIBRARIES(littlevgl_demo.elf -Wl,--start-group)
target_link_libraries(littlevgl_demo.elf debug m)

target_link_libraries(littlevgl_demo.elf debug c)

target_link_libraries(littlevgl_demo.elf debug gcc)

target_link_libraries(littlevgl_demo.elf debug nosys)

target_link_libraries(littlevgl_demo.elf optimized m)

target_link_libraries(littlevgl_demo.elf optimized c)

target_link_libraries(littlevgl_demo.elf optimized gcc)

target_link_libraries(littlevgl_demo.elf optimized nosys)

TARGET_LINK_LIBRARIES(littlevgl_demo.elf -Wl,--end-group)


