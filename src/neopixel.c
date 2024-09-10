#include "neopixel.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "esp_log.h"
#include "esp_err.h"


static const char* TAG = "Neopixel Driver";

Neopixel *init_ring(uint8_t depth, Neopixel *first) {
    Neopixel *head = (Neopixel*) malloc(sizeof(Neopixel));
    head->r = 100;
    head->g = 0;
    head->b = 0;

    head->next = head;
    head->prev = head;

    if (first == NULL) {
	first = head;
    }
    if (depth > 1) {
    	head->next = init_ring(depth - 1, first);
    	head->next->prev = head;
    } else {
	head->next = first;
	first->prev = head;
    }
    return head;
}

void print_status(Neopixel *ring) {
    for (int i = 0; i < DEPTH; i++) {
	ESP_LOGI(TAG, "Neopixel ID: %d in pointer %p: %d %d %d, %p", i, ring, ring->r, ring->g, ring->b, ring->next);
	ring = ring->next;
    } 
}

// Transform the Neopixel Linked List into a buffer of 12 * 24bits
#define BUFFER_SIZE DEPTH * 3
typedef uint8_t buffer_t[BUFFER_SIZE];
void get_buffer(Neopixel *ring, buffer_t *buffer) {
    for (int i = 0; i < DEPTH; i++) {
	(*buffer)[i * 3] = ring->g;
	(*buffer)[i * 3 + 1] = ring->r;
	(*buffer)[i * 3 + 2] = ring->b;
	ring = ring->next;
    }
}

void print_buffer(buffer_t *buffer) {
    ESP_LOGI(TAG, "Buffer: 0x");
    for (int i = 0; i < DEPTH; i++) {
	printf("%02x%02x%02x", (*buffer)[i * 3], (*buffer)[i * 3 + 1], (*buffer)[i * 3 + 2]);
    }
    printf("\n");
}

