#include <drivers/io.h>

#define PORT 0x3f8          // COM1

int init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(PORT + 0) != 0xAE) {
      return 1;
   }

   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(PORT + 4, 0x0F);
   return 0;
}

static int serial_received() {
   return inb(PORT + 5) & 1;
}

static char read_serial() {
   while (serial_received() == 0);

   return inb(PORT);
}

static int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}

void putchar_serial(char a) {
   while (is_transmit_empty() == 0);

   outb(PORT,a);
}

void puts_serial(const char* str) {
   while (*str) {
      putchar_serial(*str);
      str++;
   }
}

char getchar_serial() {
   while (serial_received() == 0);
   return inb(PORT);
}

char* getstring_serial() {
   int index = 0;
   char* size_input;
   index = 0;

   while (1) {
       char c = getchar_serial();
       if (c == '\r' || c == '\n') {
           size_input[index] = '\0';
           puts_serial("\r\n");
           break;
       } else if (c == '\b' && index > 0) {
           index--;
           puts_serial("\b \b");
       } else if (index < 255) {
           size_input[index++] = c;
           putchar_serial(c);
       }
   }
   return size_input;
}