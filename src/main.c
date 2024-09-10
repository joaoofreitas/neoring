#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "neopixel.h"
#include "ws2812b_rmt_driver.h"

static const char *TAG = "Neopixel";

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
