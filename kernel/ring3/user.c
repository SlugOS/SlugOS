void user_main() {
    // This calls our test kernel API
    asm("int $0x80");
    while(1);
}