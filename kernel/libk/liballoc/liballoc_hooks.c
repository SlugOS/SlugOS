#include <stdint.h>
#include <stddef.h>

extern uint32_t _kernel_end;  // The address of the end of the kernel, defined in your linker script

#define PAGE_SIZE 4096        // Standard page size (4KB)
#define MAX_ALLOC_SIZE 1048576 // 1MB (1024 * 1024 bytes)

static volatile uint32_t lock = 0;  // 0 = unlocked, 1 = locked
static uint32_t heap_start;  // Start the heap at the kernel end address

// Free list to track freed memory blocks
typedef struct FreeBlock {
    struct FreeBlock* next;  // Pointer to the next free block
    uint32_t size;           // Size of the block in bytes
} FreeBlock;

static FreeBlock* free_list = NULL;  // Start with no free blocks

void heap_init() {
    heap_start = (uint32_t) &_kernel_end;
}

// Spin-lock logic: Try to set lock to 1 (locked)
int liballoc_lock() {
    while (__sync_lock_test_and_set(&lock, 1)) {
        // Spin until the lock is released
    }
    return 0;  // Lock acquired
}

// Release the lock
int liballoc_unlock() {
    __sync_lock_release(&lock);  // Release the lock by setting it back to 0
    return 0;  // Lock released
}

// Function to allocate memory (with 1MB limit and free list checking)
void* liballoc_alloc(int pages) {
    uint32_t alloc_size = pages * PAGE_SIZE;  // Total memory required in bytes
    
    // If there's any available free block, reuse it
    FreeBlock* prev_free = NULL;
    FreeBlock* curr_free = free_list;

    while (curr_free != NULL) {
        if (curr_free->size >= alloc_size) {
            // Found a free block large enough
            void* allocated_memory = (void*) curr_free;

            // Adjust the free list if there's unused memory left in the block
            if (curr_free->size > alloc_size) {
                FreeBlock* new_block = (FreeBlock*) ((uint32_t) curr_free + alloc_size);
                new_block->size = curr_free->size - alloc_size;
                new_block->next = curr_free->next;  // Link the new block to the rest of the list

                if (prev_free != NULL) {
                    prev_free->next = new_block;  // Previous free block points to the new one
                } else {
                    free_list = new_block;  // Update the head if necessary
                }
            } else {
                // If no leftover space, remove the block from the free list
                if (prev_free != NULL) {
                    prev_free->next = curr_free->next;
                } else {
                    free_list = curr_free->next;
                }
            }

            return allocated_memory;
        }

        prev_free = curr_free;
        curr_free = curr_free->next;
    }

    // No reusable free block found, so allocate new memory within the 1MB limit
    if (heap_start + alloc_size > ((uint32_t) &_kernel_end + MAX_ALLOC_SIZE)) {
        return NULL;  // Out of memory or request exceeds 1MB
    }

    // Allocate new memory by moving the heap start pointer
    void* allocated_memory = (void*) heap_start;
    heap_start += alloc_size;  // Increment the heap start by the allocated size

    return allocated_memory;  // Return the allocated memory address
}

// Function to free memory and add it to the free list
int liballoc_free(void* ptr, int pages) {
    uint32_t size_to_free = pages * PAGE_SIZE;  // Size in bytes to be freed
    
    // We will create a new free block
    FreeBlock* freed_block = (FreeBlock*) ptr;
    freed_block->size = size_to_free;
    
    // Add the freed block to the start of the free list
    freed_block->next = free_list;  // The freed block points to the previous head of the list
    free_list = freed_block;  // Now the freed block is the first item in the free list

    return 0;  // Successfully freed
}
