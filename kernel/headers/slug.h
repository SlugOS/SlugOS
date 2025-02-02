#ifndef SLUG_H
#define SLUG_H

#ifdef __cplusplus
extern "C" {
#endif

// A printf implementation
int printk (const char* format, ...);

// Clear the screen
void clear_screen();

// A logging function which accepts printf like arguments, the following are the levels of log message
// level 0: This is debugging info
// level 1: This is standard logging of OK which you may see if you use a linux machine when you boot
// level 2: This is a warning for stuff like the serial port not working or something
// level 3: This is an error which means that part of the system will not work
// level 4: This is a critical error in which the system will not work like the GDT or IDT not working the system will be halted if this occurs
void logk(int level, const char* fmt, ...);

// Do a stack trace for all functions called
void TraceStackTrace(unsigned int MaxFrames);

// This is for a BSOD like error screen
__attribute__((noreturn))
void crash(unsigned int errorcode);

// Display our logo
void displaylogo();

int snprintf(char *buffer, int size, const char *format, ...);

// Add to the system log
void add_to_log(const char *msg, ...);

#ifdef __cplusplus
}
#endif

#endif // SLUG_H