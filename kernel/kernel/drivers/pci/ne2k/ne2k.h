#ifndef NE2K_H
#define NE2K_H

#include <stdint.h>
#include <stddef.h>

#ifdef NO_NE2K
#error "NE2K is disabled"
#endif

// Function declarations
void ne2k_init();
void setup_ring_buffer();
void ne2k_send_data(uint8_t* data, size_t size);
int ne2k_receive_data(uint8_t* buffer, size_t buffer_size);

#endif // NE2K_H
