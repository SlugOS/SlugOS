// Define the va_list type, which holds information needed by the macros
typedef char *va_list;

// macors to initialize and process variable argument lists
#define va_start(ap, param) ((ap) = (va_list)&param + sizeof(param))
#define va_arg(ap, type)     (*(type *)((ap) += sizeof(type)) - sizeof(type))
#define va_end(ap)           (void)(ap)