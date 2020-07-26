

PROJECT(NxpRdLib_alVca)

FILE(GLOB NxpRdLib_alVca_Sources
    ./src/phalVca.c
    ./src/phalVca_Int.c
    ./src/phalVca_Int.h
    ./src/SamAV2/phalVca_SamAV2.c
    ./src/SamAV2/phalVca_SamAV2.h
    ./src/SamAV2/phalVca_SamAV2_Int.h
    ./src/SamAV2_X/phalVca_SamAV2_X.c
    ./src/SamAV2_X/phalVca_SamAV2_X.h
    ./src/SamAV2_X/phalVca_SamAV2_X_Int.h
    ./src/Sw/phalVca_Sw.c
    ./src/Sw/phalVca_Sw.h
    ./src/Sw/phalVca_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_alVca
    ${NxpRdLib_alVca_Sources}
)
