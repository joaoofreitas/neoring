#pragma once

#include "neopixel.h"

// RMT Driver
#include "driver/rmt_tx.h"
#include "driver/rmt_encoder.h"

// Output pin
#define IN_PIN 14

// Values are 0.1us
#define T0H 4 
#define T1H 8 
#define T0L 85 
#define T1L 45 
#define Treset 550

#define RMT_RESOLUTION_HZ 10000000 // 10MHz, 1tick = 0.1us


static const rmt_symbol_word_t ws2812_bit0 = {
    .level0 = 1,
    .duration0 = T0H,
    .level1 = 0,
    .duration1 = T0L,
};

static const rmt_symbol_word_t ws2812_bit1 = {
    .level0 = 1,
    .duration0 = T1H,
    .level1 = 0,
    .duration1 = T1L,
};

static const rmt_symbol_word_t ws2812_reset = {
	.level0 = 0,
	.duration0 = Treset,
	.level1 = 0,
	.duration1 = 0,
};


size_t encoder_callback(const void* data, size_t data_size, 
    size_t symbols_written, size_t symbols_free,
    rmt_symbol_word_t *symbols, bool* done, void *arg);

void init_ws2812b_rmt(rmt_channel_handle_t *ring_channel, rmt_tx_channel_config_t *tx_channel_conf, rmt_encoder_handle_t *enc, rmt_simple_encoder_config_t *encoder_conf, rmt_transmit_config_t *tx_conf);

void write_ws2812b(buffer_t* buffer, rmt_channel_handle_t ring_channel, rmt_encoder_handle_t enc, rmt_transmit_config_t tx_conf);
