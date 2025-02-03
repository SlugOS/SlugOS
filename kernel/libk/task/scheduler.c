#include "thread.h"
#include <stdbool.h>

// Current running thread ID
static uint32_t current_thread = 0;

// Initialize the scheduler
void scheduler_init(void) {
    current_thread = 0;
}

// Simple round-robin scheduler
void scheduler_run(void) {
    while (true) {
        // Iterate through all possible threads
        for (uint32_t i = 0; i < MAX_THREADS; i++) {
            if (thread_pool[i].state == THREAD_READY) {
                // Set current thread
                current_thread = i;
                
                // Set thread to running state
                thread_pool[i].state = THREAD_RUNNING;
                
                // Run the thread
                run_thread(i);
                
                // After thread yields, set it back to ready if it's still running
                if (thread_pool[i].state == THREAD_RUNNING) {
                    thread_pool[i].state = THREAD_READY;
                }
            }
        }
    }
}

// Get the currently running thread
uint32_t scheduler_get_current_thread(void) {
    return current_thread;
}