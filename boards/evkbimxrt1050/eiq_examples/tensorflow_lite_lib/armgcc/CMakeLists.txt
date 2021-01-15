# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.1.1)

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

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(tensorflow_lite_lib)

set(MCUX_SDK_PROJECT_NAME tensorflow_lite_lib.a)

include(${ProjDirPath}/flags.cmake)

add_library(${MCUX_SDK_PROJECT_NAME} STATIC
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities
)

# include modules
include(middleware_eiq_tensorflow_lite_MIMXRT1052)

include(middleware_eiq_tensorflow_lite_third_party_eigen_MIMXRT1052)

include(middleware_eiq_tensorflow_lite_third_party_farmhash_MIMXRT1052)

include(middleware_eiq_tensorflow_lite_third_party_fft2d_MIMXRT1052)

include(middleware_eiq_tensorflow_lite_third_party_flatbuffers_MIMXRT1052)

include(middleware_eiq_tensorflow_lite_third_party_gemmlowp_MIMXRT1052)

include(middleware_eiq_tensorflow_lite_third_party_absl_MIMXRT1052)

include(middleware_eiq_tensorflow_lite_third_party_ruy_MIMXRT1052)

include(utilities_misc_utilities_MIMXRT1052)



