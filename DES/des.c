#include "des.h"

#include <stdio.h>
#include <stdlib.h>

#define BITS_IN_PC1 56
#define ROUNDS 16
#define BITS_IN_PC2 48
#define BITS_IN_IP 64
#define BITS_IN_E 48
#define NUM_S_BOXES 8
#define ROWS_PER_SBOX 4
#define COLUMNS_PER_SBOX 16
#define BITS_IN_P 32

/* Se ha modificado para que la que el resultado sean 8 bytes.*/
static const unsigned short PC1[BITS_IN_PC1] = {
    56, 48, 40, 32, 24, 16, 8,  0,  57, 49, 41, 33, 25, 17, 9,  1,  58, 50, 42,
    34, 26, 18, 10, 2,  59, 51, 43, 35, 62, 54, 46, 38, 30, 22, 14, 6,  61, 53,
    45, 37, 29, 21, 13, 5,  60, 52, 44, 36, 28, 20, 12, 4,  27, 19, 11, 3};

static const unsigned short ROUND_SHIFTS[ROUNDS] = {1, 1, 2, 2, 2, 2, 2, 2,
                                                    1, 2, 2, 2, 2, 2, 2, 1};

static const unsigned short PC2[BITS_IN_PC2] = {
    13, 16, 10, 23, 0,  4,  2,  27, 14, 5,  20, 9,  22, 18, 11, 3,
    25, 7,  15, 6,  26, 19, 12, 1,  40, 51, 30, 36, 46, 54, 29, 39,
    50, 44, 32, 47, 43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31};

static const unsigned short IP[BITS_IN_IP] = {
    57, 49, 41, 33, 25, 17, 9,  1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7,
    56, 48, 40, 32, 24, 16, 8,  0, 58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6};

static const unsigned short IP_INV[BITS_IN_IP] = {
    39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9,  49, 17, 57, 25, 32, 0, 40, 8,  48, 16, 56, 24};

static const unsigned short E[BITS_IN_E] = {
    31, 0,  1,  2,  3,  4,  3,  4,  5,  6,  7,  8,  7,  8,  9,  10,
    11, 12, 11, 12, 13, 14, 15, 16, 15, 16, 17, 18, 19, 20, 19, 20,
    21, 22, 23, 24, 23, 24, 25, 26, 27, 28, 27, 28, 29, 30, 31, 0};

static const unsigned short
    S_BOXES[NUM_S_BOXES][ROWS_PER_SBOX][COLUMNS_PER_SBOX] = {
        {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
         {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
         {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
         {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
        {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
         {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
         {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
         {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
        {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
         {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
         {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
         {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
        {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
         {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
         {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
         {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
        {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
         {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
         {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
         {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
        {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
         {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
         {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
         {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
        {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
         {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
         {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
         {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
        {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
         {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
         {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
         {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

static const unsigned short P[BITS_IN_P] = {
    15, 6, 19, 20, 28, 11, 27, 16, 0,  14, 22, 25, 4,  17, 30, 9,
    1,  7, 23, 13, 31, 26, 2,  8,  18, 12, 29, 5,  21, 10, 3,  24};

unsigned char** DES_generate_subKeys(const unsigned char* key) {
    unsigned char key_gen[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char* sub_keys = NULL;
    unsigned char** key_index = NULL;
    unsigned char mask_1, mask_2;
    unsigned short index, shift;
    int i, j;

    /* Se hace la comprobación de argumentos. */
    if (key == NULL) {
        fprintf(stderr, "Argumento NULO en DES_generate_subKeys.\n");
        return NULL;
    }

    /* Se comprueba la paridad de la clave. */
    if (checkOddParity(key, 8) != 0) {
        fprintf(stderr, "La clave no cumple la paridad indicada.\n");
        return NULL;
    }

    /* Se realiza la permutación PC1 */
    for (i = 0, index = PC1[i]; i < BITS_IN_PC1; i++, index = PC1[i]) {
        mask_1 = 0b1 << (7 - (index % 8));
        if (key[index / 8] & mask_1) {
            key_gen[i / 8] |= 0b1 << (7 - (i % 8));
        }
    }

    /* Reservar memoria */
    sub_keys = (char*)calloc(16, sizeof(char) * 6);
    if (sub_keys == NULL) {
        fprintf(stderr, "Error al reservar memoria.\n");
        return NULL;
    }
    key_index = (unsigned char**)calloc(16, sizeof(char*));
    if (key_index == NULL) {
        free(sub_keys);
        fprintf(stderr, "Error al reservar memoria.\n");
        return NULL;
    }

    /*Se generan las 16 sub-claves*/
        for (i = 0, shift = ROUND_SHIFTS[i]; i < ROUNDS;
         i++, shift = ROUND_SHIFTS[i]) {

        /*Se obtienen las mascaras para los shift de 1 y 3 bits*/
        if (shift == 1) {
            mask_1 = 0b10000000;
        } else {
            mask_1 = 0b11000000;
        }
        mask_2 = mask_1 >> 4;

        /*Se obtienen las los bits necesarios antes de perderlos*/
        mask_1 &= key_gen[0];
        mask_2 &= key_gen[3];
        /*Se desplazan los bits*/
        key_gen[0] = key_gen[0] << shift | key_gen[1] >> (8 - shift);
        key_gen[1] = key_gen[1] << shift | key_gen[2] >> (8 - shift);
        key_gen[2] = key_gen[2] << shift | key_gen[3] >> (8 - shift);
        key_gen[3] = (key_gen[3] & 0xF0) << shift | mask_1 >> (4 - shift) |
                     (key_gen[3] << shift) & 0x0F | key_gen[4] >> (8 - shift);
        key_gen[4] = key_gen[4] << shift | key_gen[5] >> (8 - shift);
        key_gen[5] = key_gen[5] << shift | key_gen[6] >> (8 - shift);
        key_gen[6] = key_gen[6] << shift | mask_2 >> (4 - shift);

        /*Se realiza la permutación PC2*/
        key_index[i] = &sub_keys[i * 6];
        for (j = 0, index = PC2[j]; j < BITS_IN_PC2; j++, index = PC2[j]) {
            mask_1 = 0b1 << (7 - (index % 8));
            if (key_gen[index / 8] & mask_1) {
                sub_keys[(i * 6) + (j / 8)] |= 0b1 << (7 - (j % 8));
            }
        }
    }

    return key_index;
}

int checkOddParity(const unsigned char* data, int length) {
    int i, parity, count = 0;

    /* Recorre cada byte en la cadena*/
    for (i = 0; i < length; i++) {
        unsigned char byte = data[i];
        parity = data[i] & 1;
        count = 0;

        /* Cuenta el número de bits establecidos en 1*/
        byte >>= 1;
        while (byte) {
            count += byte & 1;
            byte >>= 1;
        }
        /*Paridad par y el bit de paridad indicando impar. */
        if ((count % 2 == 0) && (parity == 0)) {
            return 1;
        /*Paridad impar y el bit de paridad indicando par. */
        } else if ((count % 2 != 0) && (parity != 0)) {
            return 1; // Paridad par
        }
    }

    return 0;
}