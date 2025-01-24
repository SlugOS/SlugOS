void user_main() {
    asm("int $0x80"); // Call the current test API
    while(1);
}