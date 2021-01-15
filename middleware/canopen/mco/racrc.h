/**************************************************************************
PROJECT:     Remote Access CRC calculation
MODULE:      CRC.C
CONTAINS:    Checksum function for communication and other purposes.
             The polynomial used was x^16 + x^15 + x^5 + 1
             Compatible with SDO block transfers.
VERSION:     $LastChangedDate: 2019-09-26 12:37:28 +0200 (Thu, 26 Sep 2019) $
             $LastChangedRevision: 4734 $
***************************************************************************/

#ifndef _CRC_H
#define _CRC_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
DOES:    Initialization of CRC. Prepares for new calculation.
RETURNS: -
**************************************************************************/
extern void CRC_Init(uint16_t *pCRC // pointer to new crc
);

/**************************************************************************
DOES:    Adds one byte to the CRC checksum.
RETURNS: -
**************************************************************************/
extern void CRC_Add(uint8_t data,  // byte to add to crc calculation
                    uint16_t *pCRC // pointer to current crc
);

/**************************************************************************
DOES:    Finalizes the CRC calulation
RETURNS: -
**************************************************************************/
extern void CRC_Finalize(uint16_t *pCRC // pointer to current crc
);

#ifdef __cplusplus
}
#endif

#endif // _CRC_H
/**************************************************************************
END OF FILE
**************************************************************************/
