#include <stddef.h>
#include <string.h>
#include <memalloc.h>

#define MEMORY_POOL_DEFAULT_OFFSET 0x200000 // Default offset in memory (2MB)
#define MEMORY_POOL_SIZE 1024

static void* base_memory_address = (void*)MEMORY_POOL_DEFAULT_OFFSET; // Starting address of the memory pool

typedef struct Block {
    size_t size;       // Size of the block (including metadata)
    int free;          // Whether the block is free (1) or allocated (0)
    struct Block* next; // Pointer to the next block in the chain
} Block;

static Block* free_list; // Head of the free list

void init_memory_manager(void* memory_base_address) {
    base_memory_address = memory_base_address ? memory_base_address : (void*)MEMORY_POOL_DEFAULT_OFFSET;
    free_list = (Block*)base_memory_address;
    free_list->size = MEMORY_POOL_SIZE - sizeof(Block);
    free_list->free = 1;
    free_list->next = NULL;
}

void* malloc(size_t size) {
    Block* current = free_list;
    while (current) {
        if (current->free && current->size >= size) {
            if (current->size > size + sizeof(Block)) {
                // Split the block
                Block* new_block = (Block*)((char*)current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->free = 1;
                new_block->next = current->next;
                current->next = new_block;
                current->size = size;
            }
            current->free = 0;
            return (void*)((char*)current + sizeof(Block));
        }
        current = current->next;
    }
    return NULL; // Out of memory
}

void* calloc(size_t num, size_t size) {
    size_t total_size = num * size;
    void* ptr = malloc(total_size);
    if (ptr) {
        memset(ptr, 0, total_size);
    }
    return ptr;
}

void* realloc(void* ptr, size_t new_size) {
    if (!ptr) {
        return malloc(new_size);
    }

    Block* block = (Block*)((char*)ptr - sizeof(Block));
    if (block->size >= new_size) {
        return ptr;
    }

    void* new_ptr = malloc(new_size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, block->size);
        free(ptr);
    }
    return new_ptr;
}

void free(void* ptr) {
    if (!ptr) return;

    Block* block_to_free = (Block*)((char*)ptr - sizeof(Block));
    block_to_free->free = 1;

    // Merge adjacent free blocks
    Block* current = free_list;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += current->next->size + sizeof(Block);
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}
