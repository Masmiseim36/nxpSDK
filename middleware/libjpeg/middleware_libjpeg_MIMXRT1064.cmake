include_guard()
message("middleware_libjpeg component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/jaricom.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcapimin.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcapistd.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcarith.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jccoefct.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jccolor.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcdctmgr.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jchuff.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcinit.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcmainct.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcmarker.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcmaster.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcomapi.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcparam.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcprepct.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jcsample.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jctrans.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdapimin.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdapistd.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdarith.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdatadst.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdatasrc.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdcoefct.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdcolor.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jddctmgr.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdhuff.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdinput.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdmainct.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdmarker.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdmaster.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdmerge.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdpostct.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdsample.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jdtrans.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jerror.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jfdctflt.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jfdctfst.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jfdctint.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jidctflt.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jidctfst.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jidctint.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jmemmgr.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jmemnobs.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jquant1.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jquant2.c
    ${CMAKE_CURRENT_LIST_DIR}/src/jutils.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/inc
)


