#ifndef NETWORK_H
#define NETWORK_H

#ifndef NO-PCI

#ifndef NO-NE2K

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

// Send IP data over Ethernet
void network_send_ip(ethernet_frame_t frame);// Receive IP packets over Ethernet
uint8_t* network_receive_ip(int* ip_data_size_out);

#endif

#endif


#endif