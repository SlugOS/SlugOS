#ifndef GENERIC_H
#define GENERIC_H

/*
This code will provide functions for reading, writing, and checking if a PCI device even exists.
*/

#include <stdint.h>
#include <stdbool.h>

uint32_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t register_offset);
void pci_write(uint8_t bus, uint8_t device, uint8_t function, uint8_t register_offset, uint32_t value);
bool pci_device_exists(uint8_t bus, uint8_t device, uint8_t function);

#endif // GENERIC_H
