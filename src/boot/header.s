/* Multiboot2 header setup with framebuffer flag */

.set MAGIC,    0xE85250D6     /* Magic number */
.set ARCH,     0              /* Architecture 0 for i386 */
.set LEN,      (header_end - header_start)    /* Length of the header */
.set CHECKSUM, 0x100000000 - (MAGIC + ARCH + LEN)  /* Checksum */

.section .multiboot2
header_start:
.align 4
.long MAGIC                /* Magic number */
.long ARCH                 /* Architecture */
.long LEN                  /* Header length */
.long CHECKSUM             /* Header checksum */

.long 0                    /* End tag, No specific tag to set here */
.long 8                    /* End tag length for alignment */
header_end:
