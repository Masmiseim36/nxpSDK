AML
================================================================================
Analog middleware layer is designed to improve compatibility between MCUXpresso
SDK and S32 SDK mainly by using the same function prototypes. This layer
automatically distinguishes which type of peripheral is used (for example FTM
or TPM as a timer and SPI, DSPI or LPSPI as type of SPI). This layer supports
main functionality of peripherals only. There can be implemented support
for additional custom peripherals and for custom MCUs using same prototypes.

Versions
================================================================================

Version 1.4.3
Fixed compiler errors in MDK v6.

Version 1.4.2
Based on MCUXpresso SDK 2.4 and S32 SDK 0.8.6 EAR.
Fixed errors and warnings in MDK and IAR toolchains.

Version 1.4.1
Based on MCUXpresso SDK 2.3 and S32 SDK 0.8.6 EAR.

Version 1.4
Based on MCUXpresso SDK 2.3 and S32 SDK 0.8.4 EAR.
SDK_2_0 macro definition was replaced by general SDK_KSDK definition.
GPIO_AML_SetDirection: Fixed issue in MCUXpresso part of AML. Insted of changing
the pin direction, it also used to modify pin output value.
ADC_AML_InstallHandler, AML_GPIO_InstallHandler: Functions were removed from AML
as they were not used in any driver. In case of keeping them, all projects using
GPIO or ADC AML would need to define ENABLE_RAM_VECTOR_TABLE symbol because of
the MCUXpresso SDK change.
Added CAN.
Added kStatus_Busy to _generic_status enum.

Version 1.3
S32 SDK API was changed (1.0.0 RTM). Changes were implemented to AML.
S32 SDK migrated to global status_t. AML status_t must be replaced by aml_status_t.
It was done using: 
	#define status_t aml_status_t
	typedef int32_t status_t;
Type used for all status and error return values. This code is taken from KSDK
2.0. File common_aml.h defines value for aml_status_t. This status is returned
from AML but S32 SDK driver returns status_t from status.h file. AML needs to be
compatible with older existing drivers. Be careful if you use status from AML or
from S32 SDK driver directly.
SPI_AML: Bit count per frame was inserted into AML SPI configuration structures
for both master and slave. For S32K, interrupts were set as transfer type. 
TMR_AML_Init: Fixed issue with FTM initialization if there is no channel for
allocation.
TMR_AML_ResetTimer: Fixed issue with FTM, which did not leave this function.

Version 1.2
TMR_AML_Init: Fixed issue with initialization timer if there is no channel for
allocation (e.g. KL25 call assert if there was 0 channels).

Version 1.1
Added new implemented feature: Wait

Version 1.0 (initial).
Based on KSDK 2.0 and S32K144 SDK 0.8.1 EAR.
Implemented peripherals:
Timer - TPM, FTM
SPI - SPI, DSPI
ADC
GPIO
I2C
