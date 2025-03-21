/*
 * Copyright (c) 2006-2025 LGT Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-07-04     Evlers       first implementation
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#define BV(x)               (1<<(x))
#define MIN(a, b)           (((a) < (b)) ? (a) : (b))
#define MAX(a, b)           (((a) > (b)) ? (a) : (b))
#define SWAP16(x)           ((x>>8) | (x<<8))
#define SWAP32(x)           (((x>>24)&0x000000FF) | ((x>>8)&0x0000FF00) | \
                            ((x<<8)&0x00FF0000)  | ((x<<24)&0xFF000000))

#define BREAK_UINT32( var, ByteNum ) \
                                (uint8)((uint32)(((var) >>((ByteNum) * 8)) & 0x00FF))

#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
                                ((uint32)((uint32)((Byte0) & 0x00FF) \
                                + ((uint32)((Byte1) & 0x00FF) << 8) \
                                + ((uint32)((Byte2) & 0x00FF) << 16) \
                                + ((uint32)((Byte3) & 0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte) \
                                ((uint16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

#define HI_UINT16(a)            (((a) >> 8) & 0xFF)
#define LO_UINT16(a)            ((a) & 0xFF)

#define BUILD_UINT8(hiByte, loByte) \
                                ((uint8)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))


struct crc32_ctx_t
{
    uint32_t crc_value;
};

void mem_reverse (uint8_t* buffer, int length);
unsigned char bin2bcd (unsigned val);
unsigned bcd2bin (unsigned char val);
uint8_t crc8 (uint8_t *ptr, uint32_t len);
uint8_t sum (uint8_t *ptr, uint32_t n);
uint8_t crc8_maxim (uint8_t *ptr, uint32_t len);
uint16_t crc16_ibm (uint16_t crc, uint8_t *ptr, uint32_t len);
uint16_t crc16_xmodem (uint8_t *ptr, uint32_t len);
uint16_t crc16_modbus (uint8_t *ptr, uint32_t len);
uint32_t crc32 (uint8_t *ptr, uint32_t len);
void crc32_init_ctx (struct crc32_ctx_t *ctx);
uint32_t crc32_calculate (struct crc32_ctx_t *ctx, uint8_t *ptr, uint32_t len);

#endif /* _UTILS_H_ */
