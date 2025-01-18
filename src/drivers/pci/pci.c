#ifndef NO_PCI

#include <slug.h>
#include <drivers/pci.h>
#include <stdint.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

// Write to a PCI configuration space register
void pci_write(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t value) {
    uint32_t address = (1 << 31)               // Enable bit
                      | (bus << 16)            // Bus number
                      | (slot << 11)           // Slot number
                      | (function << 8)        // Function number
                      | (offset & 0xFC);       // Register offset (aligned to 4 bytes)

    outl(PCI_CONFIG_ADDRESS, address);
    io_wait();
    outl(PCI_CONFIG_DATA, value);
    io_wait();
}

// Read from a PCI configuration space register
uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
    uint32_t address = (1 << 31)               // Enable bit
                      | (bus << 16)            // Bus number
                      | (slot << 11)           // Slot number
                      | (function << 8)        // Function number
                      | (offset & 0xFC);       // Register offset (aligned to 4 bytes)

    outl(PCI_CONFIG_ADDRESS, address);
    io_wait();
    return inl(PCI_CONFIG_DATA);
}

// Scan for a PCI device based on its vendor and device ID
int pci_scan(uint16_t target_vendor_id, uint16_t target_device_id, pci_device_t *device) {
    for (uint8_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            for (uint8_t function = 0; function < 8; function++) {
                uint32_t id = pci_read(bus, slot, function, 0);
                uint16_t vendor_id = id & 0xFFFF;
                uint16_t device_id = (id >> 16) & 0xFFFF;

                if (vendor_id == target_vendor_id && device_id == target_device_id) {
                    device->vendor_id = vendor_id;
                    device->device_id = device_id;
                    device->bus = bus;
                    device->slot = slot;
                    device->function = function;
                    return 0; // Device found
                }
            }
        }
    }
    return -1; // Device not found
}

#endif