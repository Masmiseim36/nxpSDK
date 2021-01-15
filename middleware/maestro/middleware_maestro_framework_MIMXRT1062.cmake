if(NOT MIDDLEWARE_MAESTRO_FRAMEWORK_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_MAESTRO_FRAMEWORK_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_maestro_framework component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/inc
        ${CMAKE_CURRENT_LIST_DIR}/mcu-libs/osa
        ${CMAKE_CURRENT_LIST_DIR}/mcu-libs/osa/freertos
        ${CMAKE_CURRENT_LIST_DIR}/mcu-libs/common
        ${CMAKE_CURRENT_LIST_DIR}/streamer/inc
    )

    include(middleware_maestro_framework_doc_MIMXRT1062)

endif()
