void user_main() {
    asm("int $0x80"); // Anything interrupt will cause a GPF due to exceptions being kernel level only
    while(1);
}