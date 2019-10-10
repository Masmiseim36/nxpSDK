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

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_K32L3A60VPJ1A_cm4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_K32L3A60VPJ1A_cm4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_K32L3A60VPJ1A_cm4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_K32L3A60VPJ1A_cm4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

include_directories(${ProjDirPath}/../../../../../..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../../../devices/K32L3A60)

include_directories(${ProjDirPath}/../../../../../../../frdmk32l3a6)

include_directories(${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers)

include_directories(${ProjDirPath}/../../../../../../../../devices/K32L3A60/cmsis_drivers)

include_directories(${ProjDirPath}/../../../../../../../../middleware/issdk/drivers/systick)

include_directories(${ProjDirPath}/../../../../../../../../middleware/issdk/drivers/gpio)

include_directories(${ProjDirPath}/../../../../../../../../middleware/issdk/drivers/gpio/kinetis)

include_directories(${ProjDirPath}/../../../../../../../../middleware/issdk/sensors)

include_directories(${ProjDirPath}/../../../../../../../../devices/K32L3A60/utilities/str)

include_directories(${ProjDirPath}/../../../../../../../../devices/K32L3A60/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../../../CMSIS/Driver/Include)

include_directories(${ProjDirPath}/../../../../../../../../devices/K32L3A60/template)

add_executable(fxos8700_poll.elf 
"${ProjDirPath}/../../../../../../frdm_k32l3a6.c"
"${ProjDirPath}/../../../../../../frdm_k32l3a6.h"
"${ProjDirPath}/../../../../../../frdm_stbc_agm01_shield.h"
"${ProjDirPath}/../../../../../../issdk_hal.h"
"${ProjDirPath}/../../../../../../RTE_Device.h"
"${ProjDirPath}/../fxos8700_poll.c"
"${ProjDirPath}/../../pin_mux.c"
"${ProjDirPath}/../../pin_mux.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_edma.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_edma.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_dmamux.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_dmamux.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpuart_edma.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpuart_edma.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/cmsis_drivers/fsl_lpuart_cmsis.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/cmsis_drivers/fsl_lpuart_cmsis.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpi2c.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpi2c.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpi2c_edma.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpi2c_edma.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/cmsis_drivers/fsl_lpi2c_cmsis.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/cmsis_drivers/fsl_lpi2c_cmsis.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpspi.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpspi.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpspi_edma.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_lpspi_edma.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/cmsis_drivers/fsl_lpspi_cmsis.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/cmsis_drivers/fsl_lpspi_cmsis.h"
"${ProjDirPath}/../../../../../../../../middleware/issdk/drivers/systick/systick_utils.c"
"${ProjDirPath}/../../../../../../../../middleware/issdk/drivers/systick/systick_utils.h"
"${ProjDirPath}/../../../../../../../../middleware/issdk/drivers/gpio/Driver_GPIO.h"
"${ProjDirPath}/../../../../../../../../middleware/issdk/drivers/gpio/kinetis/gpio_driver.c"
"${ProjDirPath}/../../../../../../../../middleware/issdk/drivers/gpio/kinetis/gpio_driver.h"
"${ProjDirPath}/../../../../../../../../middleware/issdk/drivers/gpio/kinetis/gpio_driver_irq.c"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/register_io_i2c.c"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/register_io_i2c.h"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/register_io_spi.c"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/register_io_spi.h"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/sensor_drv.h"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/sensor_io_i2c.c"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/sensor_io_i2c.h"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/sensor_io_spi.c"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/sensor_io_spi.h"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/fxos8700.h"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/fxos8700_drv.c"
"${ProjDirPath}/../../../../../../../../middleware/issdk/sensors/fxos8700_drv.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_msmc.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/drivers/fsl_msmc.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../../../components/uart/lpuart_adapter.c"
"${ProjDirPath}/../../../../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/K32L3A60_cm4.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/K32L3A60_cm4_features.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/fsl_device_registers.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/system_K32L3A60_cm4.c"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/system_K32L3A60_cm4.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/core_cm4.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/gcc/startup_K32L3A60_cm4.S"
"${ProjDirPath}/../../../../../../../../CMSIS/Driver/Include/Driver_USART.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Driver/Include/Driver_Common.h"
"${ProjDirPath}/../../../../../../../../devices/K32L3A60/template/RTE_Device.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Driver/Include/Driver_I2C.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Driver/Include/Driver_SPI.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/core_armv8mbl.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/core_armv8mml.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../../../../CMSIS/Include/mpu_armv8.h"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/K32L3A60xxx_cm4_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/K32L3A60xxx_cm4_flash.ld -static")

TARGET_LINK_LIBRARIES(fxos8700_poll.elf -Wl,--start-group)
target_link_libraries(fxos8700_poll.elf debug m)

target_link_libraries(fxos8700_poll.elf debug c)

target_link_libraries(fxos8700_poll.elf debug gcc)

target_link_libraries(fxos8700_poll.elf debug nosys)

target_link_libraries(fxos8700_poll.elf optimized m)

target_link_libraries(fxos8700_poll.elf optimized c)

target_link_libraries(fxos8700_poll.elf optimized gcc)

target_link_libraries(fxos8700_poll.elf optimized nosys)

TARGET_LINK_LIBRARIES(fxos8700_poll.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET fxos8700_poll.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/fxos8700_poll.elf ${EXECUTABLE_OUTPUT_PATH}/FRDM-K32L3A6-FXOS8700-POLL.bin)
