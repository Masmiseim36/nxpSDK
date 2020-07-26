

PROJECT(NxpRdLib_KeyStore)

FILE(GLOB NxpRdLib_KeyStore_Sources
    ./src/phKeyStore.c
    ./src/phKeyStore_Int.h
    ./src/Rc632/phKeyStore_Rc632.c
    ./src/Rc632/phKeyStore_Rc632.h
    ./src/Rc632/phKeyStore_Rc632_Int.h
    ./src/Rc663/phKeyStore_Rc663.c
    ./src/Rc663/phKeyStore_Rc663.h
    ./src/Rc663/phKeyStore_Rc663_Int.h
    ./src/Rd710/phKeyStore_Rd710.c
    ./src/Rd710/phKeyStore_Rd710.h
    ./src/SamAV2/phKeyStore_SamAV2.c
    ./src/SamAV2/phKeyStore_SamAV2.h
    ./src/SamAV2/phKeyStore_SamAV2_Int.c
    ./src/SamAV2/phKeyStore_SamAV2_Int.h
    ./src/Sw/phKeyStore_Sw.c
    ./src/Sw/phKeyStore_Sw.h
    ./src/Sw/phKeyStore_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_KeyStore
    ${NxpRdLib_KeyStore_Sources}
)
