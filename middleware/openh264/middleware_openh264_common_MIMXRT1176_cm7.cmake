include_guard(GLOBAL)
message("middleware_openh264_common component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/common_tables.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/copy_mb.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/cpu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/crt_util_safe_x.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/deblocking_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/expand_pic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/intra_pred_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/mc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/memory_align.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/sad_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/welsCodecTrace.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/WelsTaskThread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/WelsThread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/WelsThreadLib.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/WelsThreadPool.cpp
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/codec/common/inc
)


