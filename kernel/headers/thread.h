#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_THREADS 256
#define STACK_SIZE 4096

typedef enum {
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED,
    THREAD_TERMINATED
} thread_state_t;

typedef struct {
    uint64_t rsp;           // Stack pointer
    uint64_t stack[STACK_SIZE];
    thread_state_t state;
    uint32_t id;
    void (*entry_point)(void*);
    void* arg;
} thread_t;

// Initialize a new thread
uint32_t thread_create(void (*entry)(void*), void* arg);
// Remove a thread
bool thread_terminate(uint32_t thread_id);
// Get thread state
thread_state_t thread_get_state(uint32_t thread_id);
// Get current thread ID
uint32_t thread_get_current(void);

#endif