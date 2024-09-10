#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "driver/rmt_types.h"
#include "esp_log.h"
#include "esp_err.h"

#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "neopixel.h"
#include "ws2812b_rmt_driver.h"

static const char *TAG = "Neopixel";


static size_t encoder_callback(
    const void* data, size_t data_size, 
    size_t symbols_written, size_t symbols_free,
    rmt_symbol_word_t *symbols, bool* done, void *arg) {

    if (symbols_free < 8) {
	return 0;
    }
    
    size_t data_pos = symbols_written / 8;
    uint8_t* data_bytes = (uint8_t*) data;

    if (data_pos < data_size) {
	size_t symbol_pos = 0;
	for (int bitmask = 0x80; bitmask != 0; bitmask >>= 1) {
	    symbols[symbol_pos++] = (data_bytes[data_pos] & bitmask) ? ws2812_bit1 : ws2812_bit0;
	}
	return symbol_pos;
    } else {
	// All bytes encoded
	symbols[0] = ws2812_reset;
	*done = true;
	return 1;
    }
}

// Initialize the Driver
void init_ws2812b_rmt(rmt_channel_handle_t *ring_channel, rmt_tx_channel_config_t *tx_channel_conf, rmt_encoder_handle_t *enc, rmt_simple_encoder_config_t *encoder_conf, rmt_transmit_config_t *tx_conf) {
    ESP_LOGI(TAG, "Initializing Neopixel Driver");
    gpio_config_t gpio_conf = {
	.pin_bit_mask = 1 << IN_PIN,
	.mode = GPIO_MODE_OUTPUT,
	.pull_up_en = GPIO_PULLUP_DISABLE,
	.pull_down_en = GPIO_PULLDOWN_DISABLE,
	.intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&gpio_conf);

    // Set the pin to low
    gpio_set_level(IN_PIN, 0);

    // RMT Configuration
    ESP_LOGI(TAG, "RMT RX Channel Creation");
    tx_channel_conf->clk_src = RMT_CLK_SRC_DEFAULT;
    tx_channel_conf->gpio_num = IN_PIN;
    tx_channel_conf->mem_block_symbols = 64;
    tx_channel_conf->resolution_hz = RMT_RESOLUTION_HZ;
    tx_channel_conf->trans_queue_depth = 4;
    ESP_ERROR_CHECK(rmt_new_tx_channel(tx_channel_conf, ring_channel));

    ESP_LOGI(TAG, "RMT TX Callback Encoder Setup");
    encoder_conf->callback = encoder_callback;
    ESP_ERROR_CHECK(rmt_new_simple_encoder(encoder_conf, enc));    

    ESP_LOGI(TAG, "Enabling RMT TX Channel");
    ESP_ERROR_CHECK(rmt_enable(*ring_channel));
    tx_conf->loop_count = 0;

    ESP_LOGI(TAG, "Neopixel Driver Initialized");
}


// Sends buffer data to the Neopixel Ring
void write_ws2812b(buffer_t* buffer, rmt_channel_handle_t ring_channel, rmt_encoder_handle_t enc, rmt_transmit_config_t tx_conf) {
    printf("Writing to Neopixel Ring\n");
    
    rmt_transmit(ring_channel, enc, *buffer, BUFFER_SIZE, &tx_conf);
    rmt_tx_wait_all_done(ring_channel, portMAX_DELAY);

    vTaskDelay(500 / portTICK_PERIOD_MS);
    printf("\nDone\n");
}


int app_main() {
    // Initialize the Neopixel Ring
    rmt_channel_handle_t ring_channel = NULL;
    rmt_tx_channel_config_t tx_channel_conf = {0};
    rmt_encoder_handle_t enc = NULL;
    rmt_simple_encoder_config_t encoder_conf = {0};
    rmt_transmit_config_t tx_conf = {0};

    init_ws2812b_rmt(&ring_channel, &tx_channel_conf, &enc, &encoder_conf, &tx_conf);

    Neopixel* ring = init_ring(DEPTH, NULL);
    print_status(ring);

    buffer_t* buffer = (buffer_t*) malloc(sizeof(buffer_t));
    get_buffer(ring, buffer);
    print_buffer(buffer);

    write_ws2812b(buffer, ring_channel, enc, tx_conf);
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    // Now we set first neopixel to green
    ring->g = 100; ring->r = 0; ring->b = 0;

    get_buffer(ring, buffer);
    print_buffer(buffer);
    write_ws2812b(buffer, ring_channel, enc, tx_conf);

    return 0;
}
