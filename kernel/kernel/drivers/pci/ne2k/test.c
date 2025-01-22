#include "network.h"
#include <string.h>  // For memcpy
#include <stdint.h>  // For uint8_t
#include <stddef.h>  // For size_t
#include <slug.h>

void test_ne2k() {
    // Test data as "Hello, world!" message
    uint8_t test_ip_data[] = "Hello, world!";  // Payload for IP packet
    size_t test_ip_data_size = sizeof(test_ip_data) - 1;  // Exclude the null-terminator

    // Set up test Destination MAC address (Example address)
    uint8_t test_dest_mac[6] = { 0x00, 0x1A, 0x11, 0x33, 0x55, 0x99 };

    // Set up Ethernet frame for sending
    ethernet_frame_t frame;

    // Fill the frame with the Destination MAC address
    memcpy(frame.dest_mac, test_dest_mac, 6);

    // Fill with a hardcoded Source MAC address (change this to your system MAC or leave it static)
    uint8_t my_mac[6] = { 0x00, 0x0A, 0x35, 0x58, 0xF4, 0x41 };
    memcpy(frame.src_mac, my_mac, 6);

    // Set EtherType (IPv4)
    frame.ethertype = ETH_P_IP;

    // Copy the IP payload into the Ethernet frame
    memcpy(frame.data, test_ip_data, test_ip_data_size);

    // Send the Ethernet frame (Using your `network_send_ip`)
    network_send_ip(frame);

    // =========================
    // Simulate receiving the packet (or actually receive it if it's set up on SlugOS)
    uint8_t* received_data = NULL;
    int received_data_size = 0;

    // Assuming network_receive_ip would return the received data and its size
    received_data = network_receive_ip(&received_data_size);

    // Check if data was successfully received
    if (received_data != NULL && received_data_size > 0) {
        // Process the received data as needed, print the size or handle it
        printk("Successfully received %d bytes of IP data: %s\n", received_data_size, received_data);
    } else {
        printk("Failed to receive any data.\n");
    }
}
