#pragma once

/*
The equivalent to dos.h in our case it contains the basics that are needed
*/

#include <stdint.h>

// Return status stuff

typedef int slug_status; // similar to EFI_STATUS

/*
SLUG_SUCCESS: When everything is fine and no failures occure this should be returned
SLUG_ERROR: This is a generic error message for when the error is unknown or something else happens
SLUG_UNSUPPORTED: When something like hardware or some other thing is not supported or is just a placeholder
*/
#define SLUG_SUCCESS 0
#define SLUG_ERROR 1
#define SLUG_UNSUPPORTED 2

// Output to terminal

void putchar(char c);
void writestring(const char* data);

void printf(const char *fmt, ...);  // A printf implementation

// Output to ports needed for stuff like a PS/2 driver or interacting with pretty much any hardware

// 8bit data being recivied/sent
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

// 16bit data being recivied/sent
void outw(uint16_t port, uint16_t val);
uint16_t inw(uint16_t port);