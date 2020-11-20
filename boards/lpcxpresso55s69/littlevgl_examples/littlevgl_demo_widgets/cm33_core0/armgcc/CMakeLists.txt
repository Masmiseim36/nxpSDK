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

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

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

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLV_CONF_INCLUDE_SIMPLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLV_EX_CONF_INCLUDE_SIMPLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLV_CONF_INCLUDE_SIMPLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLV_EX_CONF_INCLUDE_SIMPLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

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

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../middleware/littlevgl)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include)

include_directories(${ProjDirPath}/../../../../../../components/ili9341)

include_directories(${ProjDirPath}/../../../../../../components/ft6x06)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/cmsis_drivers)

include_directories(${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Driver/Include)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities)

add_executable(littlevgl_demo_widgets.elf 
"${ProjDirPath}/../../../RTE_Device.h"
"${ProjDirPath}/../littlevgl_demo_widgets.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../lv_conf.h"
"${ProjDirPath}/../lv_ex_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lv_examples/src/lv_demo_widgets/lv_demo_widgets.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lv_examples/src/lv_demo_widgets/lv_demo_widgets.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lv_examples/lv_examples.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../littlevgl_support.h"
"${ProjDirPath}/../../../littlevgl_support.c"
"${ProjDirPath}/../../../../../../components/ili9341/fsl_ili9341.c"
"${ProjDirPath}/../../../../../../components/ili9341/fsl_ili9341.h"
"${ProjDirPath}/../../../../../../components/ft6x06/fsl_ft6x06.c"
"${ProjDirPath}/../../../../../../components/ft6x06/fsl_ft6x06.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_spi_dma.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_spi_dma.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_spi.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_spi.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/cmsis_drivers/fsl_spi_cmsis.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/cmsis_drivers/fsl_spi_cmsis.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_inputmux.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_inputmux.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/cmsis_drivers/fsl_i2c_cmsis.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/cmsis_drivers/fsl_i2c_cmsis.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_inputmux_connections.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_i2c_dma.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_i2c_dma.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_dma.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_dma.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portmacro.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/MemMang/heap_4.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/lvgl.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_api_map.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_conf_internal.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_debug.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_debug.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_disp.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_disp.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_group.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_group.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_indev.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_indev.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_obj.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_obj.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_obj_style_dec.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_refr.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_refr.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_style.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_style.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_arc.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_arc.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_blend.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_blend.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_img.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_img.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_label.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_label.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_line.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_line.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_mask.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_mask.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_rect.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_rect.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_triangle.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_triangle.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_img_buf.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_img_buf.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_img_cache.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_img_cache.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_img_decoder.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_img_decoder.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_dejavu_16_persian_hebrew.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_fmt_txt.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_fmt_txt.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_12.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_12_subpx.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_14.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_16.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_18.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_20.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_22.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_24.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_26.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_28.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_28_compressed.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_30.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_32.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_34.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_36.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_38.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_40.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_42.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_44.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_46.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_montserrat_48.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_simsun_16_cjk.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_unscii_8.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_symbol_def.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_gpu/lv_gpu_nxp_pxp.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_gpu/lv_gpu_nxp_pxp.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_gpu/lv_gpu_nxp_pxp_osa.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_gpu/lv_gpu_nxp_pxp_osa.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_gpu/lv_gpu_stm32_dma2d.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_disp.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_disp.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_indev.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_indev.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_tick.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_tick.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_anim.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_anim.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_area.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_area.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_async.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_async.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_bidi.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_bidi.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_color.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_color.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_fs.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_fs.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_gc.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_gc.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_ll.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_ll.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_log.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_log.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_math.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_math.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_mem.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_mem.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_printf.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_printf.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_task.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_task.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_templ.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_templ.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_txt.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_txt.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_txt_ap.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_txt_ap.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_types.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_utils.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_utils.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_empty.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_empty.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_material.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_material.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_mono.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_mono.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_template.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_template.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_arc.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_arc.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_bar.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_bar.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_btn.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_btn.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_btnmatrix.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_btnmatrix.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_calendar.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_calendar.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_canvas.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_canvas.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_chart.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_chart.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_checkbox.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_checkbox.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_cont.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_cont.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_cpicker.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_cpicker.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_dropdown.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_dropdown.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_gauge.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_gauge.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_img.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_img.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_imgbtn.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_imgbtn.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_keyboard.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_keyboard.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_label.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_label.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_led.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_led.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_line.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_line.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_linemeter.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_linemeter.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_list.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_list.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_msgbox.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_msgbox.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_objmask.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_objmask.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_objx_templ.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_objx_templ.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_page.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_page.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_roller.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_roller.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_slider.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_slider.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_spinbox.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_spinbox.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_spinner.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_spinner.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_switch.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_switch.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_table.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_table.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_tabview.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_tabview.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_textarea.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_textarea.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_tileview.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_tileview.h"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_win.c"
"${ProjDirPath}/../../../../../../middleware/littlevgl/lvgl/src/lv_widgets/lv_win.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/LPC55S69_cm33_core0.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/LPC55S69_cm33_core0_features.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/fsl_device_registers.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/system_LPC55S69_cm33_core0.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/system_LPC55S69_cm33_core0.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/startup_LPC55S69_cm33_core0.S"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_flexcomm.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_iocon.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_power.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_power.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../../components/uart/usart_adapter.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/core_cm33.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/tz_context.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armclang_ltm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../CMSIS/Driver/Include/Driver_I2C.h"
"${ProjDirPath}/../../../../../../CMSIS/Driver/Include/Driver_Common.h"
"${ProjDirPath}/../../../../../../CMSIS/Driver/Include/Driver_SPI.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/croutine.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/event_groups.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/StackMacros.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/atomic.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/croutine.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/deprecated_definitions.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/event_groups.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/list.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/message_buffer.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/mpu_prototypes.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/mpu_wrappers.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/portable.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/projdefs.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/queue.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/semphr.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/stack_macros.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/stream_buffer.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/task.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/timers.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/list.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/MemMang/ReadMe.url"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/queue.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/stream_buffer.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/tasks.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/timers.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/LPC55S69_cm33_core0_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/LPC55S69_cm33_core0_flash.ld -static")

TARGET_LINK_LIBRARIES(littlevgl_demo_widgets.elf -Wl,--start-group)
target_link_libraries(littlevgl_demo_widgets.elf debug m)

target_link_libraries(littlevgl_demo_widgets.elf debug c)

target_link_libraries(littlevgl_demo_widgets.elf debug gcc)

target_link_libraries(littlevgl_demo_widgets.elf debug nosys)

target_link_libraries(littlevgl_demo_widgets.elf optimized m)

target_link_libraries(littlevgl_demo_widgets.elf optimized c)

target_link_libraries(littlevgl_demo_widgets.elf optimized gcc)

target_link_libraries(littlevgl_demo_widgets.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/gcc)

target_link_libraries(littlevgl_demo_widgets.elf debug ${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

link_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/gcc)

target_link_libraries(littlevgl_demo_widgets.elf optimized ${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

TARGET_LINK_LIBRARIES(littlevgl_demo_widgets.elf -Wl,--end-group)


