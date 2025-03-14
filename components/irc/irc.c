/*
 * Copyright (c) 2025-2025 Evlers Developers
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-03-11     Evlers       first implementation
 */

/* infrared remote control */
/* nec protocol: https://www.sbprojects.net/knowledge/ir/nec.php */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "compiler.h"
#include "irc.h"

#define get_timeout(val)            ((uint32_t)(val * 1.5))
#define is_in_range(val, base)      ((val > (uint32_t)(base * (1.0 - NEC_ALLOW_RANGE))) && (val < (uint32_t)(base * (1.0 + NEC_ALLOW_RANGE))))

static volatile irc_rx_t irc_rx = { 0 };

bool irx_rx_is_ready (void)
{
    bool ready = irc_rx.ready;
    irc_rx.ready = false;
    return ready;
}

uint8_t irx_rx_get_address (void)
{
    return irc_rx.address;
}

uint8_t irx_rx_get_command (void)
{
    return irc_rx.command;
}

void irc_rx_decoder_reset (void)
{
    memset((void *)&irc_rx.decoder, 0, sizeof(irc_rx.decoder));
    irc_rx.repeat = 0;
}

/**
  * @brief external interrupt callback function (double edge trigger)
  * @param edge rising or falling edge (true: rising edge, false: falling edge)
  * @param elapsed_us time difference between the current edge and the last edge (microseconds)
  * @return timeout period of the next interrupt, 0 does not require a timeout
  */
uint32_t irc_rx_isr_callback (bool edge, uint32_t elapsed_us)
{
    uint32_t timeout = 0;

    switch (irc_rx.decoder.state)
    {
        case NEC_STATE_IDLE:
            /* detect lrader code low */
            if (edge == true && is_in_range(elapsed_us, NEC_LEADER_LOW_US)) {
                irc_rx.decoder.state = NEC_STATE_LEADER;
                timeout = get_timeout(NEC_LEADER_HIGH_US);
            }
        break;

        case NEC_STATE_LEADER:
            /* distinguish between normal lrader codes and repeat codes */
            if (edge == false && is_in_range(elapsed_us, NEC_LEADER_HIGH_US)) {
                irc_rx.decoder.state = NEC_STATE_DATA;
                irc_rx.decoder.bit_count = 0;
                timeout = get_timeout(NEC_BIT_LOW_US);
            } else if (edge == false && is_in_range(elapsed_us, NEC_REPEAT_HIGH_US)) {
                irc_rx.decoder.state = NEC_STATE_REPEAT;
                timeout = get_timeout(NEC_REPEAT_LOW_US);
            } else {
                irc_rx_decoder_reset();
            }
        break;

        case NEC_STATE_REPEAT:
            /* verify repeat code tail 560us low level */
            if (edge == true && is_in_range(elapsed_us, NEC_REPEAT_LOW_US)) {
                if (irc_rx.repeat < 0xFF) {
                    irc_rx.repeat ++;  /* trigger repetitive action */
                }
                timeout = get_timeout(NEC_REPEAT_PERIOD_US);
            }
            irc_rx_decoder_reset();
        break;

        case NEC_STATE_DATA:
            /* detect start bits */
            if (edge == true)
            {
                if (!is_in_range(elapsed_us, NEC_BIT_LOW_US)) {
                    irc_rx_decoder_reset(); /* start bit error */
                } else {
                    timeout = get_timeout(NEC_BIT_1_HIGH_US);
                }
            }
            /* detect logic bits */
            else
            {
                uint8_t bit_val = 0;
                if (is_in_range(elapsed_us, NEC_BIT_1_HIGH_US)) {
                    bit_val = 1;
                } else if (is_in_range(elapsed_us, NEC_BIT_0_HIGH_US)) {
                    bit_val = 0;
                } else {
                    irc_rx_decoder_reset(); /* logic bit error */
                    break;
                }

                /* stored data (lsb priority) */
                uint8_t byte_pos = irc_rx.decoder.bit_count / 8;
                uint8_t bit_pos = irc_rx.decoder.bit_count % 8;
                irc_rx.decoder.data[byte_pos] |= (bit_val << bit_pos);

                if (++irc_rx.decoder.bit_count >= 32) {
                    /* verify data reverse code */
                    if ((irc_rx.decoder.data[2] == (uint8_t)~irc_rx.decoder.data[3]) &&
                        (irc_rx.decoder.data[0] == (uint8_t)~irc_rx.decoder.data[1])) {
                        irc_rx.address = irc_rx.decoder.data[0];
                        irc_rx.command = irc_rx.decoder.data[2];
                        irc_rx.ready = true;
                    }
                    irc_rx_decoder_reset();
                }
                else {
                    timeout = get_timeout(NEC_BIT_LOW_US);
                }
            }
        break;

        default:
            irc_rx_decoder_reset();
    }

    return timeout;
}
