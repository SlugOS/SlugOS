#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern uintptr_t kernel_end;

#define MEMORY_POOL_SIZE (1024 * 1024) // 1MB
#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

static uint8_t memory_pool[MEMORY_POOL_SIZE];
static uintptr_t memory_start;
static uintptr_t memory_end;

// Allocation metadata structure
typedef struct block_header {
    size_t size;                // Size of the block, including this header
    struct block_header *next;  // Pointer to the next free block
    int free;                   // 1 if the block is free, 0 if it's allocated
} block_header_t;

static block_header_t *free_list = NULL;

// Initialize the memory allocator
void init_allocator() {
    memory_start = (uintptr_t)&kernel_end;
    memory_end = memory_start + MEMORY_POOL_SIZE;

    // Initialize the free list with a single block covering the entire pool
    free_list = (block_header_t *)memory_pool;
    free_list->size = MEMORY_POOL_SIZE;
    free_list->next = NULL;
    free_list->free = 1;
}

// Split a block if it's larger than requested
static void split_block(block_header_t *block, size_t size) {
    if (block->size >= size + sizeof(block_header_t) + ALIGNMENT) {
        block_header_t *new_block = (block_header_t *)((uint8_t *)block + size);
        new_block->size = block->size - size;
        new_block->next = block->next;
        new_block->free = 1;

        block->size = size;
        block->next = new_block;
    }
}

// Allocate memory
void *malloc(size_t size) {
    if (size == 0) return NULL;

    size = ALIGN(size + sizeof(block_header_t)); // Include space for the header
    block_header_t *current = free_list;
    block_header_t *prev = NULL;

    while (current) {
        if (current->free && current->size >= size) {
            split_block(current, size);
            current->free = 0;
            return (void *)((uint8_t *)current + sizeof(block_header_t));
        }
        prev = current;
        current = current->next;
    }

    return NULL; // No suitable block found
}

// Allocate zero-initialized memory
void *calloc(size_t num, size_t size) {
    size_t total_size = num * size;
    void *ptr = malloc(total_size);
    if (ptr) memset(ptr, 0, total_size);
    return ptr;
}

// Reallocate memory
void *realloc(void *ptr, size_t size) {
    if (!ptr) return malloc(size); // If ptr is NULL, realloc behaves like malloc
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    block_header_t *block = (block_header_t *)((uint8_t *)ptr - sizeof(block_header_t));
    if (block->size >= size + sizeof(block_header_t)) return ptr; // Reuse if block is big enough

    void *new_ptr = malloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, block->size - sizeof(block_header_t));
        free(ptr);
    }

    return new_ptr;
}

// Free memory
void free(void *ptr) {
    if (!ptr) return;

    block_header_t *block = (block_header_t *)((uint8_t *)ptr - sizeof(block_header_t));
    block->free = 1;

    // Merge adjacent free blocks
    block_header_t *current = free_list;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}
