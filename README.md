# Neoring-Driver

A WS2812B Implementation Driver for ESP-IDF

## Overview

The Neopixel Ring is implemented as a Circular Linked List, facilitating efficient traversal and manipulation of the LEDs. Each LED points to the next, forming a loop that simplifies the management of the ring structure. This design is particularly useful for creating continuous light effects around the ring.

### Macro for Defining Ring Size

The library provides a macro to define the size of the Neopixel Ring. This macro helps in setting up the ring with a specific number of LEDs, ensuring consistent initialization and usage throughout the code.

```c
#define DEPTH 12
```
## Neopixel as a Circular Linked List

```c
typedef struct Neopixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    struct Neopixel *next;
    struct Neopixel *prev;
} Neopixel;
```

Below is an illustration of the Circular Linked List used to represent the Neopixel Ring:

![Circular Linked List](https://www.geeksforgeeks.org/wp-content/uploads/CircularLinkedList.png)


### Example Usage
Here's an example of how to initialize and use the Neopixel Ring:

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

## Functions

- **init_ring**: Allocates memory and initializes the Neopixels Linked List.
- **print_status**: Prints the status of the Neopixels Linked List.
- **get_buffer**: Transforms the Linked List into a buffer.
- **print_buffer**: Prints the buffer.
- **destroy_ring**: Recursively destroys the Neopixels Linked List.
