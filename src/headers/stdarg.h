#pragma once

#define va_start(v, p) (v = (va_list)((char *)&(p) + sizeof(p)))
#define va_arg(v, t) (*(t *)((v = (va_list)((char *)(v) + sizeof(t)))-sizeof(t)))
#define va_end(v)

typedef char* va_list;
