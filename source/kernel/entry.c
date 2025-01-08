/*
This code is derived and modified from the posix-uefi project. Full license for this code is available at:
https://gitlab.com/bztsrc/posix-uefi/-/blob/master/LICENSE
*/

#include <uefi.h>
#include "headers/bootparam.h"

void _kernel(bootparam_t *bootpar);

int main(int argc, char **argv) {
    uintptr_t entry;
    bootparam_t bootp;
    efi_status_t status;
    efi_guid_t gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    efi_gop_t *gop = NULL;
    int i;

    /* set up boot parameters passed to the "kernel" */
    memset(&bootp, 0, sizeof(bootparam_t));
    status = BS->LocateProtocol(&gopGuid, NULL, (void**)&gop);
    if(!EFI_ERROR(status) && gop) {
        status = gop->SetMode(gop, 0);
        ST->ConOut->Reset(ST->ConOut, 0);
        ST->StdErr->Reset(ST->StdErr, 0);
        if(EFI_ERROR(status)) {
            fprintf(stderr, "unable to set video mode\n");
            return 0;
        }
        bootp.framebuffer = (unsigned int*)gop->Mode->FrameBufferBase;
        bootp.width = gop->Mode->Information->HorizontalResolution;
        bootp.height = gop->Mode->Information->VerticalResolution;
        bootp.pitch = sizeof(unsigned int) * gop->Mode->Information->PixelsPerScanLine;
    } else {
        fprintf(stderr, "unable to get graphics output protocol\n");
        return 0;
    }
    if(argc > 1) {
        bootp.argc = argc - 1;
        bootp.argv = (char**)malloc(argc * sizeof(char*));
        if(bootp.argv) {
            for(i = 0; i < bootp.argc; i++)
                if((bootp.argv[i] = (char*)malloc(strlen(argv[i + 1]) + 1)))
                    strcpy(bootp.argv[i], argv[i + 1]);
            bootp.argv[i] = NULL;
        }
    }

    /* exit UEFI boot services */
    if(exit_bs()) {
        fprintf(stderr,
            "Ph'nglui mglw'nafh Chtulu R'lyeh wgah'nagl fhtagn\n"
            "(Hastur has a hold on us and won't let us go)\n");
        return 0;
    }

    /* execute the kernel */
    _kernel(&bootp);
    /* failsafe, should never return just in case */
    while(1);

    return 0;
}
