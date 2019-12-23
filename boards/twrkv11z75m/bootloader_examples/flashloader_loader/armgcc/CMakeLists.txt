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


SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DLITE_FOOTPRINT")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -g")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m0plus")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=soft")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MKV11Z128VLH7")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DBL_TARGET_FLASH")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTWR_KV11Z75M")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m0plus")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m0plus")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -g")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m0plus")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=soft")

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

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/include)

include_directories(${ProjDirPath}/../../flashloader/armgcc)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/include)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/startup)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/crc)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/microseconds)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MKV11Z7/src)

include_directories(${ProjDirPath}/../../../../../devices/MKV11Z7)

include_directories(${ProjDirPath}/../../../../../devices/MKV11Z7/drivers)

add_executable(flashloader_loader.elf 
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MKV11Z7/src/bootloader_config.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MKV11Z7/src/target_config.h"
"${ProjDirPath}/../../../../../devices/MKV11Z7/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MKV11Z7/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MKV11Z7/MKV11Z7.h"
"${ProjDirPath}/../../../../../devices/MKV11Z7/MKV11Z7_features.h"
"${ProjDirPath}/../../../../../devices/MKV11Z7/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MKV11Z7/system_MKV11Z7.c"
"${ProjDirPath}/../../../../../devices/MKV11Z7/system_MKV11Z7.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MKV11Z7/src/startup/gcc/startup_MKV11Z7.S"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/flashloader_image.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_exception_handler.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_flashloader.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/include/bootloader_common.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/fsl_assert.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/src/fsl_assert.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/vector_table_info.h"
"${ProjDirPath}/../../../../../devices/MKV11Z7/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../devices/MKV11Z7/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/include/bootloader_core.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/startup/startup.c"
"${ProjDirPath}/../../../../../devices/MKV11Z7/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MKV11Z7/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm0plus.h"
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
)


set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MKV11Z128xxx7_flashloader_loader.ld -static")

TARGET_LINK_LIBRARIES(flashloader_loader.elf -Wl,--start-group)
target_link_libraries(flashloader_loader.elf optimized m)

target_link_libraries(flashloader_loader.elf optimized c)

target_link_libraries(flashloader_loader.elf optimized gcc)

target_link_libraries(flashloader_loader.elf optimized nosys)

TARGET_LINK_LIBRARIES(flashloader_loader.elf -Wl,--end-group)


