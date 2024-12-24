#pragma once

#include <stddef.h>

void init_memory_manager(void* memory_base_address);
void* malloc(size_t size);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t new_size);
void free(void* ptr);