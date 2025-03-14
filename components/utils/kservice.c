/*
 * Copyright (c) 2006-2025 LGT Development Team
 *
 * Change Logs:
 * Date         Author      Notes
 * 2024-10-23   Evlers      first implementation
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "compiler.h"
#include "kstdio.h"

/* configuration */
// #define USE_STDLIB_VSPRINTF
#ifndef KPRINTF_BUF_SIZE
#define KPRINTF_BUF_SIZE    256
#endif

/**
 * @brief This function will put string to the console.
 *
 * @param str is the string output to the console.
 */
u_weak void _kputs(const char *str, long len)
{

}

u_weak int kprintf(const char *fmt, ...)
{
    va_list args;
    size_t length = 0;
    static char log_buf[KPRINTF_BUF_SIZE];

    va_start(args, fmt);

#ifndef USE_STDLIB_VSPRINTF
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the log_buf, we have to adjust the output
     * length. */
    length = k_vsnprintf(log_buf, sizeof(log_buf) - 1, fmt, args);
#else
    length = vsnprintf(log_buf, sizeof(log_buf) - 1, fmt, args);
#endif /* USE_STDLIB_VSPRINTF */
    if (length > KPRINTF_BUF_SIZE - 1)
    {
        length = KPRINTF_BUF_SIZE - 1;
    }

    _kputs(log_buf, length);

    va_end(args);

    return length;
}

#ifndef NDEBUG
/* assert rewrite */
void __assert_fail (const char *__function, unsigned int __line, const char *__file, const char *__assertion)
{
    kprintf("assertion \"%s\" failed: file \"%s\", line %u\n", __assertion, __file, __line);
    kprintf("function \"%s\"\n", __function);
    while (1)
    {
        __asm__ ("nop");
    }
}
#endif
