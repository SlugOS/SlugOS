#include "ne2k.h"
#include <stdint.h>

#ifdef NO_NE2K
#error "NE2K is disabled"
#endif

// Base I/O port for NE2000 NIC (can change based on your configuration)
#define NE2000_BASE_ADDRESS 0x300

// NE2K register defines, same as earlier
#define COMMAND            0x00
#define CLDA0              0x01
#define CLDA1              0x02
#define BNRY               0x03
#define ISR                0x07
#define RSR                0x0C
#define RCR                0x0A
#define IMR                0x0F
#define PSTART             0x20
#define PSTOP              0x24
#define TCR                0x07
#define FIFO               0x10

#define ETH_P_IP           0x0800  // Ethertype for IPv4

// Helper I/O functions
#define outb(port, value) *(volatile uint8_t*)(port) = (value)
#define inb(port)         *(volatile uint8_t*)(port)

// Ethernet Frame structure
typedef struct {
    uint8_t dest_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype;
    uint8_t data[1500]; // Ethernet data size (max frame size: 1500 bytes)
} ethernet_frame_t;

void ne2k_init() {
    // Reset the NE2000 card
    outb(NE2000_BASE_ADDRESS + 0x1F, 0x00);
    while ((inb(NE2000_BASE_ADDRESS + 0x07) & 0x80) == 0);
    outb(NE2000_BASE_ADDRESS + 0x07, 0xFF);
    
    // Set the card into stop mode and setup DMA and memory page
    outb(NE2000_BASE_ADDRESS + COMMAND, 0x22);
    outb(NE2000_BASE_ADDRESS + 0x0C, 0x49);
    outb(NE2000_BASE_ADDRESS + 0x0A, 32);
    
    uint8_t mac_addr[6];
    for (int i = 0; i < 6; i++) {
        mac_addr[i] = inb(NE2000_BASE_ADDRESS + 0x10);
    }
    
    // Set the MAC address
    for (int i = 0; i < 6; i++) {
        outb(NE2000_BASE_ADDRESS + 0x01 + i, mac_addr[i]);
    }
    
    // Enable the card for packet receive (initial state)
    outb(NE2000_BASE_ADDRESS + RCR, 0x0F);  // Set to receive all packets
    outb(NE2000_BASE_ADDRESS + IMR, 0x01);  // Enable receive interrupts
}

void setup_ring_buffer() {
    outb(NE2000_BASE_ADDRESS + PSTART, 0x00);
    outb(NE2000_BASE_ADDRESS + PSTOP, 0xFF);
}

void ne2k_send_data(uint8_t* data, size_t size) {
    // Set command to initiate transmission
    outb(NE2000_BASE_ADDRESS + COMMAND, 0x12);  // Remote Write DMA
    outb(NE2000_BASE_ADDRESS + 0x0B, 0x00);    // Transmit buffer index to start

    // Write the data to NE2000 data FIFO
    for (size_t i = 0; i < size; i++) {
        outb(NE2000_BASE_ADDRESS + FIFO, data[i]);
    }

    // Poll for completion
    while ((inb(NE2000_BASE_ADDRESS + ISR) & 0x02) == 0);  // Check for DMA complete
    outb(NE2000_BASE_ADDRESS + ISR, 0x02);  // Acknowledge interrupt (clear)
}

int ne2k_receive_data(uint8_t* buffer, size_t buffer_size) {
    while (1) {
        if (inb(NE2000_BASE_ADDRESS + ISR) & 0x01) {  // Check receive interrupt
            outb(NE2000_BASE_ADDRESS + ISR, 0x01);  // Acknowledge interrupt
            break;
        }
    }
    
    // Read the packet data from the FIFO
    for (size_t i = 0; i < buffer_size; i++) {
        buffer[i] = inb(NE2000_BASE_ADDRESS + FIFO);
    }

    return buffer_size;  // Return number of bytes read
}
