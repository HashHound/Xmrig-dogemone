#include "dogemone.h"
#include <string.h>
#include <stdbool.h>
#include "stdio.h"
#include "cpu.h"
#include "crypto/yespower.h"

// do one memcopy to get a fresh context
//static __thread sph_blake512_context     blake_mid;

void printArray(const uint8_t* array, size_t length) {
    printf("input:");
    for (size_t i = 0; i < length; i++) {
        printf("%02x", array[i]);  // Printing each element of the array in hexadecimal format.
    }
    printf("\n");
}

#define nullptr ((void*)0)

inline void y_slow_hash(const void* data, size_t length, const uint8_t* input, uint8_t* output) {
yespower_params_t yespower_params = {
        2048,
        32,
        nullptr,
        0
};

if (yespower_tls((unsigned char *)data, length, &yespower_params, (yespower_binary_t *)output)) {

}
}



void dogemonehash(const char* input, char* output, uint32_t len)
{
    uint8_t hash[32];
    uint8_t hashA[32];

    y_slow_hash(input, len,  hash, hashA);
    memcpy(output, hashA, 32);
}



