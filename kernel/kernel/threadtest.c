#include <thread.h>
#include <slug.h>
#include <stddef.h>

// Dummy functions for threads
void thread_function_1(void* arg) {
    printk("Thread 1\n");
}

void thread_function_2(void* arg) {
    printk("Thread 2\n");
}

int thread_test() {
    // Create threads
    uint32_t thread1 = thread_create(thread_function_1, NULL);
    uint32_t thread2 = thread_create(thread_function_2, NULL);

    // Run the threads
    run_thread(thread1);
    run_thread(thread2);

    // Delete the threads
    thread_terminate(thread1);
    thread_terminate(thread2);

    return 0;
}