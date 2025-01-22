#ifndef IO_H
#define IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void io_wait(void);

#ifdef __cplusplus
}
#endif

#endif /* IO_H */