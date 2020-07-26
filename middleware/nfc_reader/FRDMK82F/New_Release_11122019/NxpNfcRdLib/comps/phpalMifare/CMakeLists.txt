

PROJECT(NxpRdLib_palMifare)

FILE(GLOB NxpRdLib_palMifare_Sources
    ./src/phpalMifare.c
    ./src/phpalMifare_Int.h
    ./src/DUT/phpalMifare_DUT.c
    ./src/DUT/phpalMifare_DUT.h
    ./src/Rd710/phpalMifare_Rd710.c
    ./src/Rd710/phpalMifare_Rd710.h
    ./src/SamAV2_X/phpalMifare_SamAV2_X.c
    ./src/SamAV2_X/phpalMifare_SamAV2_X.h
    ./src/SamAV2_X/phpalMifare_SamAV2_X_Int.h
    ./src/Stub/phpalMifare_Stub.c
    ./src/Stub/phpalMifare_Stub.h
    ./src/Sw/phpalMifare_Sw.c
    ./src/Sw/phpalMifare_Sw.h
)

ADD_LIBRARY(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)


TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_palI14443p4
    NxpRdLib_phTools
)
