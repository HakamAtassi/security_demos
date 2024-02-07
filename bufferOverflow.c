#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "util.h"

void victim(char* str) // ?? bytes
{
    char buf[16]; // ?? bytes
    char* ch = buf; // ?? bytes

    strcpy(buf, str);

    return;
}

void steal_password()
{
    printf("You called?\n");
    printf("Also, I have your password now. Sorry.\n");
    exit(-1);
}

void append_address(char* buf, int i, uint64_t addr)
{
    // This function inserts a 64 bit uint to a buffer starting at address i
    // Address is stored as little endian
    // Note: this function does not do any bounds checking

    buf[i + 0] = (char)((addr >> 0) & 0xFF);
    buf[i + 1] = (char)((addr >> 8) & 0xFF);
    buf[i + 2] = (char)((addr >> 16) & 0xFF);
    buf[i + 3] = (char)((addr >> 24) & 0xFF);
    buf[i + 4] = (char)((addr >> 32) & 0xFF);
    buf[i + 5] = (char)((addr >> 40) & 0xFF);
    buf[i + 6] = (char)((addr >> 48) & 0xFF);
    buf[i + 7] = (char)((addr >> 56) & 0xFF);
    buf[i + 8] = '\0';
    return;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Error: Please pass test size!\n");
        printf("Ex: bash bufferOverflow.sh <addr offset in bytes>\n");
        return -1;
    }
    int address_start_byte = atoi(argv[1]);

    /*Extract evil function address*/
    uint64_t function_addr = (uint64_t)(steal_password);
    printf("Target Function Ptr: 0x%lx\n", function_addr);

    char safe_str[128] = "0123456789ABCDE";
    char evil_str[128];

    for (int j = 0; j < address_start_byte; j++) { // Fill everything up to starting target addr with 0x41 ("A")
        // fill buffer with random value
        evil_str[j] = 0x41;
    }

    victim(safe_str); // Call victim with typical input string
    printf("Returned from safe_str call\n");

    append_address(evil_str, address_start_byte, function_addr);
    // hexdump_arr(evil_str);   // Uncomment for debug
    victim(evil_str);
    printf("Returned from evil_str call\n");
    printf("Try again :(\n");

    return 1;
}