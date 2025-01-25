#include <stdint.h>
#include <slug.h>

/* Assume, as is often the case, that EBP is the first thing pushed. If not, we are in trouble. */
struct stackframe {
  struct stackframe* ebp;
  uint32_t eip;
};

void TraceStackTrace(unsigned int MaxFrames) {
    struct stackframe *stk;
    asm ("movl %%ebp,%0" : "=r"(stk) ::);
    
    printk("Stack trace:\n");
    
    for (unsigned int frame = 0; stk && frame < MaxFrames; ++frame) {
        printk("  0x%08x\n", stk->eip);  // Adjust the format to match hexadecimal output
        stk = stk->ebp;
    }
}
