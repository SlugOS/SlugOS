#pragma once

#include <stddef.h>

/*
Memory allocation
*/

// Allocate memory
void *malloc(size_t size);
// Allocate zero-initialized memory
void *calloc(size_t num, size_t size);
// Reallocate memory
void *realloc(void *ptr, size_t size);
// Free memory
void free(void *ptr);

/*
Misc core system functions like sleep();
*/

// Sleep for the specified number of milliseconds
void sleep(uint32_t milliseconds);