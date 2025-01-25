; x86 crti.s

global _init
global _fini

_init:
    ; gcc will nicely put the contents of crtbegin.o's .init section here.
    ret

_fini:
    ; gcc will nicely put the contents of crtbegin.o's .fini section here.
    ret
