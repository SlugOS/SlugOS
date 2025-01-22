#ifndef NO-PCI

#ifndef NO-NE2K

#include "ne2k.h"
#include "../generic.h"
#include <string.h>
#include <stdint.h>

// Define the Ethernet frame structure
typedef struct {
    uint8_t dest_mac[6];  // Destination MAC address
    uint8_t src_mac[6];   // Source MAC address
    uint16_t ethertype;   // EtherType (e.g., 0x0800 for IP)
    uint8_t data[1500];   // Payload data (max 1500 bytes for Ethernet)
} ethernet_frame_t;

// Define EtherType for IPv4 (ETH_P_IP)
#define ETH_P_IP 0x0800

// Send IP data over Ethernet (now accepts an ethernet_frame_t directly)
void network_send_ip(ethernet_frame_t frame) {
    // Send the entire Ethernet frame
    size_t frame_size = sizeof(frame.ethertype) + sizeof(frame.data) + sizeof(frame.dest_mac) + sizeof(frame.src_mac);
    ne2k_send_data((uint8_t*)&frame, frame_size);
}

// Receive IP packets over Ethernet
uint8_t* network_receive_ip(int* ip_data_size_out) {
    uint8_t buffer[1500];  // Buffer large enough to store an Ethernet frame
    int size = ne2k_receive_data(buffer, sizeof(buffer));

    if (size > 0) {
        // Parse the received Ethernet frame
        ethernet_frame_t* frame = (ethernet_frame_t*)buffer;

        // Check if the EtherType indicates it's an IP packet
        if (frame->ethertype == ETH_P_IP) {
            // Extract the IP data from the frame's payload
            *ip_data_size_out = size - sizeof(ethernet_frame_t);
            uint8_t* ip_data = frame->data;
            return ip_data;  // Return the IP data (to be used by the caller)
        }
    }

    // Return NULL if no valid IP packet is received
    *ip_data_size_out = 0;
    return NULL;
}

#endif

#endif
