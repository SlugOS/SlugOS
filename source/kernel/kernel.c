/*
This code is derived and modified from the posix-uefi project. Full license for this code is available at:
https://gitlab.com/bztsrc/posix-uefi/-/blob/master/LICENSE
*/

#include "headers/bootparam.h"

bootparam_t *bootp;

void print(char* s);

/* A simple kernel */
void _kernel(bootparam_t *bootpar) {
    int i;
    bootp = bootpar;
    for(i = 0; i < bootp->width * bootp->height; i++)
        bootp->framebuffer[i] = 0x000008;

    print("Hello from SlugOS UEFI kernel");
    while(1);
}
