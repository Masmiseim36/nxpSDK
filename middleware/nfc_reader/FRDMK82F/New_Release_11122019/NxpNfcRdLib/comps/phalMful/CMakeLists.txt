

PROJECT(NxpRdLib_alMful)

FILE(GLOB NxpRdLib_alMful_Sources
    ./src/phalMful.c
    ./src/phalMful_Int.c
    ./src/phalMful_Int.h
    ./src/SamAV2/phalMful_SamAV2.c
    ./src/SamAV2/phalMful_SamAV2.h
    ./src/SamAV2_X/phalMful_SamAV2_X.c
    ./src/SamAV2_X/phalMful_SamAV2_X.h
    ./src/Sw/phalMful_Sw.c
    ./src/Sw/phalMful_Sw.h
)
ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_palMifare
    NxpRdLib_CryptoSym
    NxpRdLib_CryptoRng
)
