

PROJECT(NxpRdLib_CryptoSym)

FILE(GLOB NxpRdLib_CryptoSym_Sources
    ./src/phCryptoSym.c
    ./src/phCryptoSym_Int.h
    ./src/Stub/phCryptoSym_Stub.c
    ./src/Stub/phCryptoSym_Stub.h
    ./src/Sw/phCryptoSym_Sw.c
    ./src/Sw/phCryptoSym_Sw.h
    ./src/Sw/phCryptoSym_Sw_Aes.c
    ./src/Sw/phCryptoSym_Sw_Aes.h
    ./src/Sw/phCryptoSym_Sw_Aes_Int.h
    ./src/Sw/phCryptoSym_Sw_Des.c
    ./src/Sw/phCryptoSym_Sw_Des.h
    ./src/Sw/phCryptoSym_Sw_Des_Int.h
    ./src/Sw/phCryptoSym_Sw_Int.c
    ./src/Sw/phCryptoSym_Sw_Int.h
)
ADD_LIBRARY(NxpRdLib_CryptoSym
    ${NxpRdLib_CryptoSym_Sources}
)
