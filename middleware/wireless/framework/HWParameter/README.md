# Production Data Storage

Hardware parameters provide production data storage

## Overview

Different platforms/boards need board/network node-specific settings to function according to the design. (Examples of such settings are IEEE® addresses and radio calibration values specific to the node.) For this purpose, the last flash sector is reserved and contains hardware-specific parameters for production data storage. These parameters pertain to the network node as a distinct entity. For example, a silicon mounted on a PCB in a specific configuration, rather than to just the silicon itself. This sector is reserved by the linker file, through the PROD_DATA section and it should be read/written only through the API described below.




> Note :
This sector is not erased/written at code download time and it is not updated via over-the-air firmware update procedures to preserve the respective node-specific data, regardless of the firmware running on it.

## Constant Definitions

Name :

```
extern uint32_t PROD_DATA_BASE_ADDR[];
```

Description :

This symbol is defined in the linker script. It specifies the start address of the PROD_DATA section.

Name :

```
static const uint8_t mProdDataIdentifier[10] = {"PROD_DATA:"};
```

Description :

The value of this constant is copied as identification word (header) at the beginning of the PROD_DATA area and verified by the dedicated read function.


Note: the length of mProdDataIdentifier imposes the definition of PROD_DATA_ID_STRING_SZ as 10.
The legacy HW parameters structure provides headroom for future usage.
There are currently 63 bytes available.

## Data type definitions
Name :


```
typedef PACKED_STRUCT HwParameters_tag
{
    uint8_t identificationWord[PROD_DATA_ID_STRING_SZ]; /* internal usage only: valid data present */
    /*@{*/
    uint8_t bluetooth_address[BLE_MAC_ADDR_SZ];      /*!< Bluetooth address */
    uint8_t ieee_802_15_4_address[IEEE_802_15_4_SZ]; /*!< IEEE 802.15.4 MAC address - K32W1 only */
    uint8_t xtalTrim;                                /*!< XTAL 32MHz Trim value */
    uint8_t xtalCap32K;                              /*!< XTAL 32kHz capacitance value */
    /* For forward compatibility additional fields may be added here
       Existing data in flash will not be compatible after modifying the hardwareParameters_t typedef.
       In this case the size of the padding has to be adjusted.
    */
    uint8_t reserved[1];
    /* first byte of padding : actual size if 63 for legacy HwParameters but
      complement to 128 bytes in the new structure */
}
hardwareParameters_t;
```

Description:

Defines the structure of the hardware-dependent information.


> Note :
Some members of this structure may be ignored on a specific board/silicon configuration. Also, new members may be added for implementation-specific purposes and the backward compatibility must be maintained.

The CRC calculation starts from the reserved field of the hardwareParameters_t and ends before the hardwareParamsCrc field. Additional members to this structure may be added using the following method :

Add new fields before the reserved field. This method does not cause a CRC fail, but you must keep in mind to subtract the total size of the new fields from the size of the reserved field. For example, if a field of uint8_t size is added using this method, the size of the reserved field shall be changed to 63.

# Co-locating application factory data in HW Parameters flash sector.

The sector containing the Hardware parameter structure may be located in the internal flash, usually at its last sector.
The actual Hardware parameter structure has a size of 128 bytes - including padding reserved for future use.
Since there is plenty of room available in a flash sector (4kB or 8kB), co-locating Application Factory Data in the same structure prevents from reserving another flash sector for these data. The application designer may adopt this solution by defining gHwParamsAppFactoryDataExtension_d as 1.
A total of 2kB is alloted to this purpose.

If this option was chosen, whenever any of the Hardware parameter fields is modified, its CRC16 will change so the sector will need erasing. The gHwParamsAppFactoryDataPreserveOnHwParamUpdate_d compilation option deals with restoring the contents of the App Factory Data. Nonetheless this requires a temporary allocation a 2kB buffer to preserve the previous content and restore then on completion of the Hw Parameter update.

# Special reserved area at start of IFR1 in range [0x02002000..0x02002600[
On development boards a 1536 byte area is reserved and the actual Hardware parameter area begins at offset 0x600.
Preserving this area on a HW parameter update also requires a temporary 1.5kB dynamic allocation (in addition to the App Factory 2kB allocation), to be able to restore on completion of update operation.

# HW Parameters Production Data placement options

The placement of production data (PROD_DATA) can be selected based on the definition of gHwParamsProdDataPlacement_c (see fwk_config.h).
The productions data seldom need update for final products, once calibration data, MAC addresses or others have been programmed.
Two cases exist, plus a transition mode :

   1) gHwParamsProdDataMainFlashMode_c (0) :
      - PROD_DATA are located  at top of Main Flash. Hardware parameters section is placed in the last sector of internal flash [0xfe000..0x100000[.
      - The linker script must reserve this area explicitly so as to prevent placement of NVM or text sections at that location by setting gUseProdInfoMainFlash_d.

   2)  gHwParamsProdDataMainFlash2IfrMode_c(1) :
      - PROD_DATA are located in IFR1, but MainFlash version still exists during interim period.
      - If the contents of the PROD_DATA section in MainFlash is valid (not blank and correct CRC) but the IFR PROD_DATA is still blank, copy the contents of MainFlash PROD_DATA to IFR location.
      - When done PROD_DATA in IFR are used. Once the transition is done, an application using (2: gHwParamsProdDataPlacementIfrMode_c) may be programmed.
  
   3) gHwParamsProdDataIfrMode_c (2) :
      - PROD_DATA section dwells in the IFR1 sector [0x02002000..0x02004000[
      - in development phase the area comprised between [0x02002000..0x02002600[ must be reserved for internal purposes.
      - This allows to free up the top sector of Main Flash by linking with gUseProdInfoMainFlash_d unset.


