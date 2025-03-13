/*
 * Copyright (c) 2006-2024 LGT Development Team
 *
 * Change Logs:
 * Date         Author      Notes
 * 2024-10-23   Evlers      first implementation
 */

#ifndef _KSTDIO_H_
#define _KSTDIO_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int k_vsprintf(char *dest, const char *format, va_list arg_ptr);
int k_vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int k_sprintf(char *buf, const char *format, ...);
int k_snprintf(char *buf, size_t size, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* _KSTDIO_H_ */
