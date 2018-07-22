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

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m0plus")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=soft")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m0plus")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=soft")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MKL25Z128VLK4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFRDM_KL25Z")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFREEDOM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPRINTF_ADVANCED_ENABLE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m0plus")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=soft")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MKL25Z128VLK4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFRDM_KL25Z")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFREEDOM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPRINTF_ADVANCED_ENABLE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m0plus")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=soft")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MKL25Z128VLK4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m0plus")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=soft")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MKL25Z128VLK4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m0plus")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=soft")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m0plus")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=soft")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m0plus")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=soft")

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

include_directories(${ProjDirPath}/../../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../../platform/drivers)

include_directories(${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/gpio/kinetis)

include_directories(${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/gpio)

include_directories(${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/host)

include_directories(${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/ads)

include_directories(${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/ads/kinetis)

include_directories(${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/systick)

include_directories(${ProjDirPath}/../../../../../../../CMSIS/Driver/Include)

include_directories(${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors)

include_directories(${ProjDirPath}/../../../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../frdmkl25z)

include_directories(${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers)

include_directories(${ProjDirPath}/../../../../../../../devices/MKL25Z4/cmsis_drivers)

include_directories(${ProjDirPath}/../../../../../../../devices/MKL25Z4/utilities)

include_directories(${ProjDirPath}/../../../../../../../devices/MKL25Z4)

add_executable(kl25z_a8471_host_demo_template.elf 
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/gcc/startup_MKL25Z4.S"
"${ProjDirPath}/../../../../../frdm_kl25z.c"
"${ProjDirPath}/../../../../../frdm_kl25z.h"
"${ProjDirPath}/../../../../../frdm_stbc_a8471_shield.h"
"${ProjDirPath}/../../../../../issdk_hal.h"
"${ProjDirPath}/../../../../../RTE_Device.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/driverexamples/demos/host_demo.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/sensor_drv.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/fxas21002.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/fxlc95000.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/fxls8471q.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/fxls8962.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/fxos8700.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/fxpq3115.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/mag3110.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/mma845x.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/mma865x.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/mma8491q.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/mma9553.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/mpl3115.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/diff_p.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/systick/systick_utils.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/systick/systick_utils.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_lpsci.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_lpsci.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_spi.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_spi.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_lptmr.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_lptmr.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_smc.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_smc.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_dma.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_dma.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_dmamux.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_dmamux.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_i2c_dma.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_i2c_dma.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/cmsis_drivers/fsl_i2c_cmsis.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/cmsis_drivers/fsl_i2c_cmsis.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_spi_dma.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_spi_dma.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/cmsis_drivers/fsl_spi_cmsis.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/cmsis_drivers/fsl_spi_cmsis.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_lpsci_dma.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_lpsci_dma.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/cmsis_drivers/fsl_lpsci_cmsis.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/cmsis_drivers/fsl_lpsci_cmsis.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_uart_dma.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_uart_dma.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/gpio/kinetis/gpio_driver_irq.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/gpio/kinetis/gpio_driver.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/gpio/kinetis/gpio_driver.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/gpio/Driver_GPIO.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/host_io_uart.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/host_io_uart.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/host/comm_if_uart.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/host/comm_interface.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/host/data_format_hdlc.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/host/data_format_hdlc.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/host/data_format_json.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/host/data_format_json.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/host/data_format_service.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/host/host_interface_service.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/host/host_interface_service.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/ads/kinetis/auto_detection_service.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/ads/kinetis/auto_detection_service.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/drivers/ads/auto_detection_shield.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/register_io_i2c.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/register_io_i2c.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/sensor_io_i2c.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/sensor_io_i2c.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/register_io_spi.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/register_io_spi.h"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/sensor_io_spi.c"
"${ProjDirPath}/../../../../../../../middleware/issdk_1.6/sensors/sensor_io_spi.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/utilities/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/utilities/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/system_MKL25Z4.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/system_MKL25Z4.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_flash.c"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_flash.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../../devices/MKL25Z4/drivers/fsl_clock.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MKL25Z128xxx4_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MKL25Z128xxx4_flash.ld -static")

TARGET_LINK_LIBRARIES(kl25z_a8471_host_demo_template.elf -Wl,--start-group)
target_link_libraries(kl25z_a8471_host_demo_template.elf debug m)

target_link_libraries(kl25z_a8471_host_demo_template.elf debug c)

target_link_libraries(kl25z_a8471_host_demo_template.elf debug gcc)

target_link_libraries(kl25z_a8471_host_demo_template.elf debug nosys)

target_link_libraries(kl25z_a8471_host_demo_template.elf optimized m)

target_link_libraries(kl25z_a8471_host_demo_template.elf optimized c)

target_link_libraries(kl25z_a8471_host_demo_template.elf optimized gcc)

target_link_libraries(kl25z_a8471_host_demo_template.elf optimized nosys)

TARGET_LINK_LIBRARIES(kl25z_a8471_host_demo_template.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET kl25z_a8471_host_demo_template.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Osrec ${EXECUTABLE_OUTPUT_PATH}/kl25z_a8471_host_demo_template.elf ${EXECUTABLE_OUTPUT_PATH}/KL25Z-A8471-HOST-DEMO.srec)
