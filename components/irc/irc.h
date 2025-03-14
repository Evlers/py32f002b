/*
 * Copyright (c) 2025-2025 Evlers Developers
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-03-11     Evlers       first implementation
 */

#ifndef _IRC_H_
#define _IRC_H_

/* protocol parameter definition */
#define NEC_LEADER_LOW_US       9000    /* leader code low level time */
#define NEC_LEADER_HIGH_US      4500    /* leader code high level time */
#define NEC_BIT_LOW_US          560     /* data bit low level reference time */
#define NEC_BIT_0_HIGH_US       560     /* logic 0 high level time */
#define NEC_BIT_1_HIGH_US       1680    /* logic 1 high level time */
#define NEC_REPEAT_HIGH_US      2250    /* repeat code high level time */
#define NEC_REPEAT_LOW_US       560     /* repeat code low level time */
#define NEC_REPEAT_PERIOD_US    110320  /* repeat code period time */
#define NEC_ALLOW_RANGE         0.15    /* allow range of time difference (+-15%) */

typedef enum
{
    NEC_STATE_IDLE,                     /* idle state */
    NEC_STATE_LEADER,                   /* leader code detection */
    NEC_STATE_DATA,                     /* data reception */
    NEC_STATE_REPEAT                    /* repeat code detection */
} nec_state_t;

typedef struct
{
    uint8_t data[4];                    /* receive buffer [address, ~address, command, ~command] */
    uint8_t bit_count;                  /* current received bits (0-32) */
    nec_state_t state;                  /* state machine state */
} nec_decoder_t;

typedef struct
{
    volatile nec_decoder_t decoder;     /* infrared decoder */
    volatile bool ready;                /* data ready flag */
    volatile uint8_t address;           /* parsed user address */
    volatile uint8_t command;           /* parsed command code */
    volatile uint8_t repeat;            /* repeat code trigger count */
} irc_rx_t;

bool irx_rx_is_ready (void);
uint8_t irx_rx_get_address (void);
uint8_t irx_rx_get_command (void);
void irc_rx_decoder_reset (void);
uint32_t irc_rx_isr_callback (bool edge, uint32_t elapsed_us);

#endif /* _IRC_H_ */
