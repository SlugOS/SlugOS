#define KSTACKSTART 0x2000
#define KSTACKEXC   0x4000

void __attribute__((naked)) entry() {
	asm("mov sp, %[ps]" : : [ps]"i" (KSTACKSTART));
	/* send to serial output */
	asm("mov r1, #0x16000000");
	asm("mov r2, #65");
	asm("str r2, [r1]");
	/* call main kernel function */
	asm("bl	start");
}