#include <stdint.h>
#include <stdbool.h>
#include <thread.h>

#define MAX_THREADS 256
#define STACK_SIZE 4096

static thread_t thread_pool[MAX_THREADS];
static uint32_t current_thread = 0;
static uint32_t thread_count = 0;

// Initialize a new thread
uint32_t thread_create(void (*entry)(void*), void* arg) {
    if (thread_count >= MAX_THREADS) {
        return 0;
    }

    uint32_t id = thread_count++;
    thread_t* thread = &thread_pool[id];
    
    // Initialize thread structure
    thread->id = id;
    thread->state = THREAD_READY;
    thread->entry_point = entry;
    thread->arg = arg;
    
    // Setup initial stack frame
    uint64_t* stack_top = &thread->stack[STACK_SIZE - 1];
    thread->rsp = (uint64_t)stack_top;
    
    return id;
}

// Remove a thread
bool thread_terminate(uint32_t thread_id) {
    if (thread_id >= thread_count) {
        return false;
    }
    
    thread_pool[thread_id].state = THREAD_TERMINATED;
    return true;
}

// Get thread state
thread_state_t thread_get_state(uint32_t thread_id) {
    if (thread_id >= thread_count) {
        return THREAD_TERMINATED;
    }
    return thread_pool[thread_id].state;
}

// Get current thread ID
uint32_t thread_get_current(void) {
    return current_thread;
}