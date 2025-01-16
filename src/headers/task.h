#pragma once

void task_yield(void);
void task_create(void (*task_function)());
void task_scheduler(void);