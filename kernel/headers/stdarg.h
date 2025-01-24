#ifndef STDARG_H
#define STDARG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef __builtin_va_list va_list;

#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_arg(ap, type)    __builtin_va_arg(ap, type)
#define va_end(ap)          __builtin_va_end(ap)

#ifdef __cplusplus
}
#endif

#endif