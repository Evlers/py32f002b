/*
 * Copyright (c) 2006-2025 LGT Development Team
 *
 * Change Logs:
 * Date         Author      Notes
 * 2024-10-23   Evlers      first implementation
 */

#ifndef __COMPILER_H__
#define __COMPILER_H__

#define u_section(x)               __attribute__((section(x)))
#define u_used                     __attribute__((used))
#define u_align(n)                 __attribute__((aligned(n)))
#define u_weak                     __attribute__((weak))
#define u_typeof                   __typeof__
#define u_noreturn                 __attribute__ ((noreturn))
#define u_inline                   static __inline
#define u_always_inline            static inline __attribute__((always_inline))
#define u_packed                   __attribute__((packed))

/* extend */
#define u_unused(x)                 ((void)x)

#endif  /* __COMPILER_H__ */
