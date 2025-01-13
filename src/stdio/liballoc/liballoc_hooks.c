#include <stdint.h>
#include <stddef.h>

// Locking code

static volatile int lock_flag = 0; // The lock status

int liballoc_lock() {
    while (__sync_lock_test_and_set(&lock_flag, 1)) {
        // Spin until the lock is acquired (this is only here since we will go multitasking at some point)
    }
    return 0; // Lock acquired successfully
}

int liballoc_unlock() {
    __sync_lock_release(&lock_flag); // Unlock the lock_flag
    return 0; // Lock released successfully
}

// Allocation code

extern uint32_t kernel_end; // Provided by your linker script
#define HEAP_START ((uintptr_t)&kernel_end)
#define HEAP_SIZE  (1024 * 1024) // 1MB (1024*1024 bytes)
#define HEAP_END   (HEAP_START + HEAP_SIZE)

static uintptr_t heap_current = HEAP_START;

typedef struct Block {
    struct Block* next;
    uintptr_t size;
} Block;

static Block* free_list = NULL; // Start with an empty free list

/*
Allocates memory for the requested number of pages (4KB per page).
\param pages The number of 4KB pages to allocate.
\return A pointer to the allocated memory, or NULL if not enough space.
*/
void* liballoc_alloc(int pages) {
    uintptr_t size = pages * 4096; // Each page is 4KB

    // Check if there's a free block that can be reused
    Block* prev = NULL;
    Block* current = free_list;

    while (current != NULL) {
        if (current->size >= size) {
            // We've found a free block big enough to satisfy the request
            // Remove the block from the free list
            if (prev) {
                prev->next = current->next;
            } else {
                free_list = current->next;
            }
            // Return the memory block's address
            return (void*)((uintptr_t)current + sizeof(Block));
        }
        prev = current;
        current = current->next;
    }

    // No suitable free block, allocate a new one from the heap
    if (heap_current + size > HEAP_END) {
        return NULL; // Not enough space in the heap
    }

    uintptr_t allocated_block = heap_current;
    heap_current += size;

    // Create the block header at the beginning of the allocated space
    Block* new_block = (Block*)allocated_block;
    new_block->size = size;
    new_block->next = NULL;

    // Return the usable memory after the block header
    return (void*)((uintptr_t)new_block + sizeof(Block));
}

/*
Frees previously allocated memory.
Adds the freed block to the free list for future reuse.
\param ptr Pointer to the allocated memory (should be returned by liballoc_alloc).
\param pages The number of pages to free.
\return 0 on success.
*/
int liballoc_free(void* ptr, int pages) {
    uintptr_t size = pages * 4096; // Number of bytes to free

    // Make sure the pointer is within the valid heap region
    if ((uintptr_t)ptr < HEAP_START || (uintptr_t)ptr >= HEAP_END) {
        return -1; // Invalid pointer
    }

    // The memory block is located just before the requested pointer
    Block* block_to_free = (Block*)((uintptr_t)ptr - sizeof(Block));
    block_to_free->size = size; // Set the size of the block

    // Add this block to the free list
    block_to_free->next = free_list;
    free_list = block_to_free;

    return 0; // Successfully freed memory
}
