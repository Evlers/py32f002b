/*
 * Copyright (c) 2006-2025 Evlers Developers
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author      Notes
 * 2025-02-22   Evlers      first implementation
 */

#include <stddef.h>
#include <stdint.h>
#include "kservice.h"
#include "progress_bar.h"

/* initialize the progress bar context */
void progress_bar_init (progress_bar_t *ctx, const char *prefix, uint32_t (*tick_get) (void))
{
    if (ctx) {
        ctx->last_tick = 0;
        ctx->data_rate = 0;
        ctx->last_size = 0;
        ctx->prefix = prefix ? prefix : "progress"; /* default prefix if null */
        ctx->is_first_update = 1;       /* first update flag */
        ctx->refresh_interval = 100;    /* default refresh interval in ms */
    }
}

/* automatically adjust rate unit based on the value */
static const char *auto_adjust_rate_unit (uint32_t rate, uint32_t *adjusted_rate)
{
    const char *unit = "Byte/s";
    *adjusted_rate = rate; /* default to bytes per second */

    if (rate >= 1024 * 1024 * 1024) { /* >= 1 GB/s */
        *adjusted_rate /= (1024 * 1024 * 1024);
        unit = "GB/s";
    } else if (rate >= 1024 * 1024) { /* >= 1 MB/s */
        *adjusted_rate = rate / (1024 * 1024);
        unit = "MB/s";
    } else if (rate >= 10 * 1024) { /* >= 10 KB/s */
        *adjusted_rate = rate / 1024;
        unit = "KB/s";
    }
    return unit;
}

/* update and display the progress bar */
void progress_bar_update (progress_bar_t *ctx, size_t cur_size, size_t total_size)
{
    if (!ctx || total_size == 0) {
        return; /* invalid context or total size */
    }

    /* calculate current progress percentage */
    uint8_t per = (uint8_t)((cur_size * 100ull) / total_size);
    if (per > 100) {
        per = 100; /* cap progress at 100% */
    }

    /* get current time and calculate elapsed time since last update */
    uint32_t current_tick = ctx->tick_get();
    uint32_t elapsed_time = current_tick - ctx->last_tick;

    /* update progress bar every refresh interval or when download completes */
    if (elapsed_time >= ctx->refresh_interval || cur_size == total_size)
    {
        /* calculate current download rate (bytes per second) */
        uint32_t download_rate = (uint32_t)((cur_size - ctx->last_size) * 1000 / elapsed_time);
        /* apply smoothing to the download rate */
        ctx->data_rate = (ctx->data_rate * 3 + download_rate) / 4;

        /* update last recorded size and timestamp */
        ctx->last_size = cur_size;
        ctx->last_tick = current_tick;

        /* generate the progress bar */
        for (int i = 0; i < 50; i++) {
            if (i < (per / 2)) {
                ctx->progress_sign[i] = '='; /* completed portion */
            } else if (i == (per / 2)) {
                ctx->progress_sign[i] = '>'; /* current progress indicator */
            } else {
                ctx->progress_sign[i] = ' '; /* remaining portion */
            }
        }
        ctx->progress_sign[50] = '\0'; /* ensure null termination */

        /* automatically adjust rate unit */
        uint32_t display_rate;
        const char *unit = auto_adjust_rate_unit(ctx->data_rate, &display_rate);

        /* move cursor up if not the first update */
        if (!ctx->is_first_update) {
            kprintf("\033[1A"); /* move cursor up one line */
        } else {
            ctx->is_first_update = 0; /* clear first update flag */
        }

        /* print the progress bar and download information */
        kprintf("\033[2K\r%s: [%s] %d%%  %u | %u %s\n", ctx->prefix, ctx->progress_sign, per, cur_size, display_rate, unit);
    }
}
