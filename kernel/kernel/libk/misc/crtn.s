section .init
    ; gcc will nicely put the contents of crtend.o's .init section here.
    ret

section .fini
    ; gcc will nicely put the contents of crtend.o's .fini section here.
    ret
