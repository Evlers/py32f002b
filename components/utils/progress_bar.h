/*
 * Copyright (c) 2006-2025 Evlers Developers
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author      Notes
 * 2025-02-22   Evlers      first implementation
 */

#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_

/* progress bar context structure */
typedef struct
{
    char progress_sign[51];         /* 50-character progress bar + 1 null terminator */
    uint32_t last_tick;             /* last timestamp for rate calculation */
    uint32_t data_rate;             /* smoothed data rate in bytes per second */
    size_t last_size;               /* last recorded size for rate calculation */
    const char *prefix;             /* customizable prefix text for the progress bar */
    int is_first_update;            /* flag to indicate first update */
    uint32_t (*tick_get) (void);    /* function pointer to get the current tick count, unit: ms */
    uint32_t refresh_interval;      /* refresh interval in ms, used to control the refresh frame rate */
} progress_bar_t;

/* initialize the progress bar context */
void progress_bar_init (progress_bar_t *ctx, const char *prefix, uint32_t (*tick_get) (void));

/* update and display the progress bar */
void progress_bar_update (progress_bar_t *ctx, size_t cur_size, size_t total_size);

#endif /* _PROGRESS_BAR_H_ */
