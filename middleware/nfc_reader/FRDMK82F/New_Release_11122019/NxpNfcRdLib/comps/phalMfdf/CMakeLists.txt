

PROJECT(NxpRdLib_alMfdf)

FILE(GLOB NxpRdLib_alMfdf_Sources
    ./src/phalMfdf.c
    ./src/phalMfdf_Int.c
    ./src/phalMfdf_Int.h
    ./src/SamAV2/phalMfdf_SamAV2.c
    ./src/SamAV2/phalMfdf_SamAV2.h
    ./src/SamAV2/phalMfdf_SamAV2_Int.c
    ./src/SamAV2/phalMfdf_SamAV2_Int.h
    ./src/SamAV2_X/phalMfdf_SamAV2_X.c
    ./src/SamAV2_X/phalMfdf_SamAV2_X.h
    ./src/SamAV2_X/phalMfdf_SamAV2_X_Int.c
    ./src/SamAV2_X/phalMfdf_SamAV2_X_Int.h
    ./src/Sw/phalMfdf_Sw.c
    ./src/Sw/phalMfdf_Sw.h
    ./src/Sw/phalMfdf_Sw_Int.c
    ./src/Sw/phalMfdf_Sw_Int.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_palMifare
    NxpRdLib_CryptoSym
    NxpRdLib_CryptoRng
)

