/* For MCU Integration */


#define BOARD_InitHardware() \
    BOARD_InitPins();    \
    BOARD_BootClockRUN(); \
    BOARD_InitDebugConsole()
