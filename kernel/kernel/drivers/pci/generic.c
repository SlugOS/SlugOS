/*
This code will provide functions for reading, writing, and checking if a PCI device even exists.
*/

// Check that NO-PCI is not being passed
#ifndef NO-PCI

#include <stdint.h>
#include <stdbool.h>
#include <io.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

// PCI register offsets
#define PCI_VENDOR_ID_REGISTER 0x00
#define PCI_DEVICE_ID_REGISTER 0x02

// Helper macro for generating the PCI configuration address
#define MAKE_PCI_ADDRESS(bus, device, function, reg) \
    ((0x80000000) | ((bus) << 16) | ((device) << 11) | ((function) << 8) | (reg))

uint32_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t register_offset) {
    uint32_t address = MAKE_PCI_ADDRESS(bus, device, function, register_offset);

    // Write the address to the PCI config address port
    outl(PCI_CONFIG_ADDRESS, address);

    // Read the data from the PCI config data port
    return inl(PCI_CONFIG_DATA);
}

void pci_write(uint8_t bus, uint8_t device, uint8_t function, uint8_t register_offset, uint32_t value) {
    uint32_t address = MAKE_PCI_ADDRESS(bus, device, function, register_offset);

    // Write the address to the PCI config address port
    outl(PCI_CONFIG_ADDRESS, address);

    // Write the value to the PCI config data port
    outl(PCI_CONFIG_DATA, value);
}

bool pci_device_exists(uint8_t bus, uint8_t device, uint8_t function) {
    uint16_t vendor_id = pci_read(bus, device, function, PCI_VENDOR_ID_REGISTER);
    
    // If vendor ID is 0xFFFF, the device is not present
    return (vendor_id != 0xFFFF);  // Return true if the device exists
}

#endif