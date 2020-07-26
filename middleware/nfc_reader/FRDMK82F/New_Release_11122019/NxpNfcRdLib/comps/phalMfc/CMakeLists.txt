

PROJECT(NxpRdLib_alMfc)

FILE(GLOB NxpRdLib_alMfc_Sources
    ./src/phalMfc.c
    ./src/phalMfc_Int.c
    ./src/phalMfc_Int.h
    ./src/SamAV2_X/phalMfc_SamAV2_X.c
    ./src/SamAV2_X/phalMfc_SamAV2_X.h
    ./src/SamAV2_X/phalMfc_SamAV2_X_Int.h
    ./src/Sw/phalMfc_Sw.c
    ./src/Sw/phalMfc_Sw.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_KeyStore
)
