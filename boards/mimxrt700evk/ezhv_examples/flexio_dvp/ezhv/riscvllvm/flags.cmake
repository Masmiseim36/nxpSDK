IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_DEBUG " \
    ${CMAKE_ASM_FLAGS_DEBUG} \
    -DMCUXPRESSO_SDK \
    -mcpu=zv2116 \
    -mabi=ilp32 \
")
SET(CMAKE_ASM_FLAGS_RELEASE " \
    ${CMAKE_ASM_FLAGS_RELEASE} \
    -DMCUXPRESSO_SDK \
    -mcpu=zv2116 \
    -mabi=ilp32 \
")
SET(CMAKE_C_FLAGS_DEBUG " \
    ${CMAKE_C_FLAGS_DEBUG} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DEZHV_DVP_EN=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT798SGFOA_ezhv \
    -g \
    -O0 \
    -Wall \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -std=gnu99 \
    -mcpu=zv2116 \
    -mabi=ilp32 \
    -falign-functions=4 \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_RELEASE " \
    ${CMAKE_C_FLAGS_RELEASE} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DEZHV_DVP_EN=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT798SGFOA_ezhv \
    -Os \
    -Wall \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -std=gnu99 \
    -mcpu=zv2116 \
    -mabi=ilp32 \
    -falign-functions=4 \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_DEBUG " \
    ${CMAKE_CXX_FLAGS_DEBUG} \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -g \
    -O0 \
    -Wall \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -fno-rtti \
    -fno-exceptions \
    -mcpu=zv2116 \
    -mabi=ilp32 \
    -falign-functions=4 \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_RELEASE " \
    ${CMAKE_CXX_FLAGS_RELEASE} \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -Os \
    -Wall \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -fno-rtti \
    -fno-exceptions \
    -mcpu=zv2116 \
    -mabi=ilp32 \
    -falign-functions=4 \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_DEBUG} \
    -g \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -Map=output.map \
    -Wl,--print-memory-usage \
    -mcpu=zv2116 \
    -mabi=ilp32 \
    -falign-functions=4 \
    -T\"${ProjDirPath}/MIMXRT798Sxxxx_ezhv_ram.ld\" -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_RELEASE} \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -Map=output.map \
    -Wl,--print-memory-usage \
    -mcpu=zv2116 \
    -mabi=ilp32 \
    -falign-functions=4 \
    -T\"${ProjDirPath}/MIMXRT798Sxxxx_ezhv_ram.ld\" -static \
")
