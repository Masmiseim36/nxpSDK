if(NOT MIDDLEWARE_MOTOR_CONTROL-PMSM_SAFETY_MKV11Z7_INCLUDED)
    
    set(MIDDLEWARE_MOTOR_CONTROL-PMSM_SAFETY_MKV11Z7_INCLUDED true CACHE BOOL "middleware_motor_control-pmsm_safety component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/common_test/wdog/iec60730b_wdog.c
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/core_test/cm0/programCounter/iec60730b_cm0_pc_object.S
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/core_test
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/core_test/cm0
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/common_test/aio
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/common_test/clock
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/common_test/dio
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/common_test/tsi
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/common_test/wdog
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/compiler
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/core_test/cm0/flash
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/core_test/cm0/programCounter
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/core_test/cm0/ram
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/core_test/cm0/register
        ${CMAKE_CURRENT_LIST_DIR}/safety/v4_1/core_test/cm0/stack
    )


endif()
