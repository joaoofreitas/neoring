# neoring
A WS2812B Implementation Driver for ESP-IDF


Commands:
`pio run -t upload`

`pio device monitor`

`pio run -t compiledb`

Example usage:

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

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
    if (buffer == NULL) {
	ESP_LOGE(TAG, "Failed to allocate memory for buffer");
	return -1;
    }

    get_buffer(ring, buffer);
    print_buffer(buffer);
    write_ws2812b(buffer, ring_channel, enc, tx_conf);
    
    return 0;
}

```
