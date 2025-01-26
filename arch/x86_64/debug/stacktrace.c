#include <stdint.h>
#include <slug.h>

/* Assume, as is often the case, that EBP is the first thing pushed. If not, we are in trouble. */
struct stackframe {
  struct stackframe* rbp;
  uint32_t rip;
};

void TraceStackTrace(unsigned int MaxFrames) {
    struct stackframe *stk;
    asm ("mov %%rbp,%0" : "=r"(stk) ::);
    
    printk("Stack trace:\n");
    
    for (unsigned int frame = 0; stk && frame < MaxFrames; ++frame) {
        printk("  0x%08x\n", stk->rip);  // Adjust the format to match hexadecimal output
        stk = stk->rbp;
    }
}
