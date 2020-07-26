

PROJECT(NxpRdLib_CryptoRng)

FILE(GLOB NxpRdLib_CryptoRng_Sources
    ./src/phCryptoRng.c
    ./src/Stub/phCryptoRng_Stub.c
    ./src/Stub/phCryptoRng_Stub.h
    ./src/Sw/phCryptoRng_Sw.c
    ./src/Sw/phCryptoRng_Sw.h
    ./src/Sw/phCryptoRng_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_CryptoRng
    ${NxpRdLib_CryptoRng_Sources}
)
