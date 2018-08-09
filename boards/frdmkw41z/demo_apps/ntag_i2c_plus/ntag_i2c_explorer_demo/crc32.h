#ifndef CRC32_H
#define CRC32_H

#include <limits.h>
#include <stddef.h>

uint32_t crc32(const uint32_t seed, const unsigned char *bytes, size_t n);

/* Returns the 32-bit CRC of the first n bytes in the array.  */
/* Each byte must be in the range 0..255.           */
/* The seed allows to concat the CRC value of several blocks  */

uint32_t crc32_update(uint32_t crc, const unsigned char *bytes, uint16_t n);

/* To compute the CRC incrementally, pass the first set     */
/* of bytes to this function with crc = 0xffffffff.  Then   */
/* pass each subsequent set of bytes with crc equal to the  */
/* return value from the previous call.  After all bytes    */
/* have been processed, the CRC of the entire sequence of   */
/* bytes is r ^ 0xffffffff, where r is the return value     */
/* from the last call, and ^ is the C bitwise xor operator, */
/* not exponentiation.                                      */

#endif /* CRC32_H */
