#pragma once

#include <stdint.h>

#define DEPTH 12
#define BUFFER_SIZE DEPTH * 3

typedef uint8_t buffer_t[BUFFER_SIZE];

// Circular Linked List of 12 Neopixels
typedef struct Neopixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    struct Neopixel *next;
    struct Neopixel *prev;
} Neopixel;

// Recursive function to allocate memory and initialize the Neopixels Linked List
Neopixel *init_ring(uint8_t depth, Neopixel *first);

// Prints the status of the Neopixels Linked List
void print_status(Neopixel *ring);

// Transforms the LinkedList into a buffer
void get_buffer(Neopixel *ring, buffer_t *buffer);

// Print the buffer
void print_buffer(buffer_t *buffer);

// Recursive function to destroy the Neopixels Linked List
void destroy_ring(Neopixel *ring);
