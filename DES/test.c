#include <stdio.h>
#include <stdlib.h>

#include "des.h"

void printBytesAsBits(const unsigned char* data, size_t length);
void printArrayAsHex(const unsigned char* array, size_t length);
void test_DES_generate_subKeys();

int main(int argc, char const* argv[]) {
    unsigned char key[] = {0b00010011, 0b00110100, 0b01010111, 0b01111001,
                           0b10011011, 0b10111100, 0b11011111, 0b11110001};
    unsigned char message[] = {0b00000001, 0b00100011, 0b01000101, 0b01100111,
                               0b10001001, 0b10101011, 0b11001101, 0b11001101, 0b11001101};

    printBytesAsBits(message, 9);
    printArrayAsHex(message, 9);

    unsigned char* result;
    unsigned char* result2;
    long int size;
    long int size2;
    if (DES_ECB_encrypt_data(message, 9, &result, &size, key, PKCS_5) != 0) {
        return 0;
    };

    printBytesAsBits(result, size);
    printArrayAsHex(result, size);

    if (DES_ECB_decrypt_data(result, size, &result2, &size2, key) != 0) {
        free(result);
        return 0;
    };
    printBytesAsBits(result2, size2);
    printArrayAsHex(result2, size2);

    free(result);
    free(result2);
    return 0;
}

void printBytesAsBits(const unsigned char* data, size_t length) {
    int i, j;
    for (i = 0; i < length; i++) {
        unsigned char byte = data[i];

        // Imprime cada bit del byte en orden inverso (de derecha a izquierda)
        for (j = 7; j >= 0; j--) {
            printf("%d", (byte >> j) & 1);
        }

        printf(" ");  // Agrega un espacio entre bytes
    }

    printf("\n");  // Salto de línea al final
}

void printArrayAsHex(const unsigned char* array, size_t length) {
    int i;
    for (i = 0; i < length; i++) {
        printf("%02X ", array[i]);
    }
    printf("\n");
}
