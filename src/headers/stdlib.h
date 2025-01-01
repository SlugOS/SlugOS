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
