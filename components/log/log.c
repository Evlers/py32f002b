/*
 * Copyright (c) 2006-2024 LGT Development Team
 *
 * Change Logs:
 * Date         Author      Notes
 * 2025-01-06   Evlers      first implementation
 */

#include "stdio.h"
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include "kstdio.h"
#include "log.h"

#define BYTES_PER_LINE      16
#define LOG_BUF_SIZE        256

static char log_buf[LOG_BUF_SIZE];

void log_write(int level, const char *tag, const char *format, ...)
{
    va_list args;
    size_t length = 0;

    va_start(args, format);
    /* the return value of vsnprintf is the number of bytes that would be
    * written to buffer had if the size of the buffer been sufficiently
    * large excluding the terminating null byte. If the output string
    * would be larger than the log_buf, we have to adjust the output
    * length. */
    length = k_vsnprintf(log_buf, sizeof(log_buf) - 1, format, args);
    extern void _kputs(const char *str, long len);
    _kputs(log_buf, length);
    va_end(args);
}

void log_buffer_hexdump_internal(const char *tag, const void *buffer, uint16_t buff_len, log_level_t log_level)
{
    const char *ptr_buffer = buffer;
    const char *ptr_line;
    static char hd_buffer[2 + sizeof(void *) * 2 + 3 + BYTES_PER_LINE * 3 + 1 + 3 + BYTES_PER_LINE + 1 + 1];
    char *ptr_hd;
    int bytes_cur_line;

    if (buff_len == 0) {
        return;
    }

    do {
        if (buff_len > BYTES_PER_LINE) {
            bytes_cur_line = BYTES_PER_LINE;
        } else {
            bytes_cur_line = buff_len;
        }
        //use memcpy to get around alignment issue
        ptr_line = ptr_buffer;
        ptr_hd = hd_buffer;

        ptr_hd += sprintf(ptr_hd, "%p ", ptr_buffer);
        for (int i = 0; i < BYTES_PER_LINE; i ++) {
            if ((i & 7) == 0) {
                ptr_hd += sprintf(ptr_hd, " ");
            }
            if (i < bytes_cur_line) {
                ptr_hd += sprintf(ptr_hd, " %02x", (unsigned char) ptr_line[i]);
            } else {
                ptr_hd += sprintf(ptr_hd, "   ");
            }
        }
        ptr_hd += sprintf(ptr_hd, "  |");
        for (int i = 0; i < bytes_cur_line; i ++) {
            if (isprint((int)ptr_line[i])) {
                ptr_hd += sprintf(ptr_hd, "%c", ptr_line[i]);
            } else {
                ptr_hd += sprintf(ptr_hd, ".");
            }
        }
        ptr_hd += sprintf(ptr_hd, "|");

        LOG_LEVEL(log_level, tag, "%s", hd_buffer);
        ptr_buffer += bytes_cur_line;
        buff_len -= bytes_cur_line;
    } while (buff_len);
}
