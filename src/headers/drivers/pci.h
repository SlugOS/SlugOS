#pragma once

// This is a simple setup for reading, and writing, and even searching for PCI
#ifndef NO_PCI

#include <slug.h>
#include <stdint.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

// Structure for identifying PCI device
typedef struct {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
} pci_device_t;

// Write to a PCI configuration space register
void pci_write(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t value);
// Read from a PCI configuration space register
uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);
// Scan for a PCI device based on its vendor and device ID
int pci_scan(uint16_t target_vendor_id, uint16_t target_device_id, pci_device_t *device);
#endif