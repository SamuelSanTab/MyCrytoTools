/**
 * @file des.c
 * @author Samuel Sánchez Tabernero
 * @date 13 Jul 2023
 * @brief Implementación de las funciones del archivo des.h.
 * @version 1.0
 */

#include "des.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define DES_BITS_IN_PC1 56
#define DES_ROUNDS 16
#define DES_BITS_IN_PC2 48
#define DES_BITS_IN_IP 64
#define DES_BITS_IN_E 48
#define DES_NUM_S_BOXES 8
#define DES_ROWS_PER_SBOX 4
#define DES_COLUMNS_PER_SBOX 16
#define DES_BITS_IN_P 32

static const unsigned short PC1[DES_BITS_IN_PC1] = {
    56, 48, 40, 32, 24, 16, 8,  0,  57, 49, 41, 33, 25, 17, 9,  1,  58, 50, 42,
    34, 26, 18, 10, 2,  59, 51, 43, 35, 62, 54, 46, 38, 30, 22, 14, 6,  61, 53,
    45, 37, 29, 21, 13, 5,  60, 52, 44, 36, 28, 20, 12, 4,  27, 19, 11, 3};

static const unsigned short ROUND_SHIFTS[DES_ROUNDS] = {1, 1, 2, 2, 2, 2, 2, 2,
                                                        1, 2, 2, 2, 2, 2, 2, 1};

static const unsigned short PC2[DES_BITS_IN_PC2] = {
    13, 16, 10, 23, 0,  4,  2,  27, 14, 5,  20, 9,  22, 18, 11, 3,
    25, 7,  15, 6,  26, 19, 12, 1,  40, 51, 30, 36, 46, 54, 29, 39,
    50, 44, 32, 47, 43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31};

static const unsigned short IP[DES_BITS_IN_IP] = {
    57, 49, 41, 33, 25, 17, 9,  1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7,
    56, 48, 40, 32, 24, 16, 8,  0, 58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6};

static const unsigned short IP_INV[DES_BITS_IN_IP] = {
    39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9,  49, 17, 57, 25, 32, 0, 40, 8,  48, 16, 56, 24};

static const unsigned short E[DES_BITS_IN_E] = {
    31, 0,  1,  2,  3,  4,  3,  4,  5,  6,  7,  8,  7,  8,  9,  10,
    11, 12, 11, 12, 13, 14, 15, 16, 15, 16, 17, 18, 19, 20, 19, 20,
    21, 22, 23, 24, 23, 24, 25, 26, 27, 28, 27, 28, 29, 30, 31, 0};

static const unsigned short
    S_BOXES[DES_NUM_S_BOXES][DES_ROWS_PER_SBOX][DES_COLUMNS_PER_SBOX] = {
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

static const unsigned short P[DES_BITS_IN_P] = {
    15, 6, 19, 20, 28, 11, 27, 16, 0,  14, 22, 25, 4,  17, 30, 9,
    1,  7, 23, 13, 31, 26, 2,  8,  18, 12, 29, 5,  21, 10, 3,  24};

unsigned char* DES_block_cipher(const unsigned char* block,
                                const unsigned char** keys, bool mode) {
    unsigned char perm_block[] = {0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00};
    unsigned char swap_block[] = {0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00};
    unsigned char* enc_block = NULL;
    unsigned char* half_left = NULL;
    unsigned char* half_right = NULL;
    unsigned char* half_aux = NULL;
    char mask;
    short index;
    int i, j;

    /* Se hace la comprobación de argumentos. */
    if (keys == NULL) {
        fprintf(stderr, "Error. Claves vacía.\n");
        return NULL;
    } else if (block == NULL) {
        fprintf(stderr, "Error. Bloque vacío.\n");
        return NULL;
    }
    /* Se realiza la permutación IP */
    for (i = 0, index = IP[i]; i < DES_BITS_IN_IP; i++, index = IP[i]) {
        mask = 1 << (7 - (index % 8));
        if (block[index / 8] & mask) {
            perm_block[i / 8] |= 1 << (7 - (i % 8));
        }
    }

    half_left = &perm_block[0];
    half_right = &perm_block[4];
    /*Se realizan las 16 rondas*/
    if (mode == true) {
        for (i = 0; i < DES_ROUNDS; i++) {
            /*Se realiza la función F*/
            half_aux = DES_F_function(half_right, keys[i]);
            if (half_aux == NULL) {
                fprintf(stderr, "Error. Fallo en la función F en la ronda %d\n",
                        i);
                return NULL;
            }

            /*Se hace un XOR*/
            for (j = 0; j < 4; j++) {
                half_left[j] ^= half_aux[j];
            }
            free(half_aux);
            /* Swap  */
            half_aux = half_left;
            half_left = half_right;
            half_right = half_aux;
        }
    } else {
        for (i = DES_ROUNDS; i > 0; i--) {
            /*Se realiza la función F*/
            half_aux = DES_F_function(half_right, keys[i - 1]);
            if (half_aux == NULL) {
                fprintf(stderr, "Error. Fallo en la función F en la ronda %d\n",
                        i);
                return NULL;
            }

            /*Se hace un XOR*/
            for (j = 0; j < 4; j++) {
                half_left[j] ^= half_aux[j];
            }
            free(half_aux);
            /* Swap  */
            half_aux = half_left;
            half_left = half_right;
            half_right = half_aux;
        }
    }

    /*Swap final.*/
    for (i = 0; i < 4; i++) {
        swap_block[i] = half_right[i];
        swap_block[i + 4] = half_left[i];
    }

    /*Se reserva memoria para el bloque cifrado.*/
    enc_block = (unsigned char*)calloc(8, sizeof(char));
    if (enc_block == NULL) {
        fprintf(stderr, "Error. Fallo al reservar memoria.");
        return NULL;
    }

    /* Se realiza la permutación IP_INV */
    for (i = 0, index = IP_INV[i]; i < DES_BITS_IN_IP; i++, index = IP_INV[i]) {
        mask = 1 << (7 - (index % 8));
        if (swap_block[index / 8] & mask) {
            enc_block[i / 8] |= 1 << (7 - (i % 8));
        }
    }

    return enc_block;
}

unsigned char* DES_F_function(const unsigned char* right,
                              const unsigned char* key) {
    unsigned char exp_right[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char pre_half_block[] = {0x00, 0x00, 0x00, 0x00};
    unsigned char mask;
    unsigned short index, x_index, y_index;
    unsigned short S_index[DES_NUM_S_BOXES] = {0x00, 0x00, 0x00, 0x00,
                                               0x00, 0x00, 0x00, 0x00};
    unsigned char* half_block = NULL;
    int i;

    /* Se hace la comprobación de argumentos. */
    if (key == NULL) {
        fprintf(stderr, "Error. Clave vacía.\n");
        return NULL;
    } else if (right == NULL) {
        fprintf(stderr, "Error. Medio bloque vacío.\n");
        return NULL;
    }

    /*Se expande el medio bloque.*/
    for (i = 0, index = E[i]; i < DES_BITS_IN_E; i++, index = E[i]) {
        mask = 1 << (7 - (index % 8));
        if (right[index / 8] & mask) {
            exp_right[i / 8] |= 1 << (7 - (i % 8));
        }
    }

    /*Se realizar un XOR entre el medio bloque y la clave*/
    for (i = 0; i < 6; i++) {
        exp_right[i] ^= key[i];
    }

    /*Se obtienen los indices para las SBox*/
    S_index[0] = (exp_right[0] & 0b11111100) >> 2;
    S_index[1] = (exp_right[0] & 0b00000011) << 4 | (exp_right[1] & 0xF0) >> 4;
    S_index[2] = (exp_right[1] & 0x0F) << 2 | (exp_right[2] & 0b11000000) >> 6;
    S_index[3] = exp_right[2] & 0b00111111;
    S_index[4] = (exp_right[3] & 0b11111100) >> 2;
    S_index[5] = (exp_right[3] & 0b00000011) << 4 | (exp_right[4] & 0xF0) >> 4;
    S_index[6] = (exp_right[4] & 0x0F) << 2 | (exp_right[5] & 0b11000000) >> 6;
    S_index[7] = exp_right[5] & 0b00111111;

    /*Se pasa el resultado por las cajas de sustitución*/
    for (i = 0, index = S_index[i]; i < DES_NUM_S_BOXES;
         i++, index = S_index[i]) {
        y_index = (index & 0b011110) >> 1;
        x_index = (index & 0b100000) >> 4 | (index & 0b000001);

        pre_half_block[i / 2] |= S_BOXES[i][x_index][y_index];
        if (i % 2 == 0) {
            pre_half_block[i / 2] <<= 4;
        }
    }

    /*Se reserva memoria para el medio bloque resultante.*/
    half_block = (unsigned char*)calloc(4, sizeof(char));
    if (half_block == NULL) {
        fprintf(stderr, "Error. Fallo al reservar memoria.\n");
        return NULL;
    }

    /*Se realiza la permutación final.*/
    for (i = 0, index = P[i]; i < DES_BITS_IN_P; i++, index = P[i]) {
        mask = 1 << (7 - (index % 8));
        if (pre_half_block[index / 8] & mask) {
            half_block[i / 8] |= 1 << (7 - (i % 8));
        }
    }
    return half_block;
}

unsigned char** DES_generate_subkeys(const unsigned char* key) {
    unsigned char key_gen[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char* sub_keys = NULL;
    unsigned char** key_index = NULL;
    unsigned char mask_1, mask_2;
    unsigned short index, shift, subkey_size;
    int i, j;

    /* Se comprueba que la clave no sea nula. */
    if (key == NULL) {
        fprintf(stderr, "Error. Clave vacía.\n");
        return NULL;
    }

    /* Se realiza la permutación PC1 */
    for (i = 0, index = PC1[i]; i < DES_BITS_IN_PC1; i++, index = PC1[i]) {
        mask_1 = 0b1 << (7 - (index % 8));
        if (key[index / 8] & mask_1) {
            key_gen[i / 8] |= 0b1 << (7 - (i % 8));
        }
    }

    /* Se reserva memoria para las 16 sub-claves y para el indice que apunte
     * a las mismas. */
    subkey_size = DES_BITS_IN_PC2 / 8;
    sub_keys = (unsigned char*)calloc(DES_ROUNDS, sizeof(char) * subkey_size);
    if (sub_keys == NULL) {
        fprintf(stderr, "Error. Fallo al reservar memoria.\n");
        return NULL;
    }
    key_index = (unsigned char**)calloc(DES_ROUNDS, sizeof(char*));
    if (key_index == NULL) {
        free(sub_keys);
        fprintf(stderr, "Error. Fallo al reservar memoria.\n");
        return NULL;
    }

    /* Se asignan los punteros al indice. */
    for (i = 0; i < DES_ROUNDS; i++) {
        key_index[i] = &sub_keys[subkey_size * i];
    }

    /*Se generan las 16 sub-claves*/
    for (i = 0, shift = ROUND_SHIFTS[i]; i < DES_ROUNDS;
         i++, shift = ROUND_SHIFTS[i]) {
        /* Se obtienen las mascaras para los shift de 1 y 3 bits*/
        if (shift == 1) {
            mask_1 = 0b10000000;
        } else if (shift == 2) {
            mask_1 = 0b11000000;
        } else {
            fprintf(stderr, "Error. El tamaño de máscara no es válido.\n");
            free(key_index);
            free(sub_keys);
            return NULL;
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
                     ((key_gen[3] << shift) & 0x0F) | key_gen[4] >> (8 - shift);
        key_gen[4] = key_gen[4] << shift | key_gen[5] >> (8 - shift);
        key_gen[5] = key_gen[5] << shift | key_gen[6] >> (8 - shift);
        key_gen[6] = key_gen[6] << shift | mask_2 >> (4 - shift);

        /*Se realiza la permutación PC2*/
        for (j = 0, index = PC2[j]; j < DES_BITS_IN_PC2; j++, index = PC2[j]) {
            mask_1 = 0b1 << (7 - (index % 8));
            if (key_gen[index / 8] & mask_1) {
                key_index[i][j / 8] |= 0b1 << (7 - (j % 8));
            }
        }
    }

    return key_index;
}

int DES_check_odd_parity(const unsigned char* data, int length) {
    unsigned char byte;
    int i, parity, count = 0;

    /* Se comprueba que el tamaño no es menor que 1 y data es nulo.  */
    if (length < 1) {
        fprintf(stderr, "Error. Tamaño de bloque menor que 1.\n");
        return 1;
    } else if (data == NULL) {
        fprintf(stderr, "Error. Argumento vacío.\n");
        return 1;
    }

    /* Recorre cada byte en la cadena para comprobarlo. */
    for (i = 0; i < length; i++, count = 0) {
        /* Se obtienen por separado los 7 bits de contenido y el de paridad.
         */
        byte = data[i] >> 1;
        parity = data[i] & 1;

        /* Mientras el byte no sea 0 se cuenta si el bit de menor valor
         * es 1. */
        while (byte) {
            count += byte & 1;
            byte >>= 1;
        }

        /* Si la paridad indica par, el bit de paridad tiene que ser 1; o si
         * la paridad indica impar, el bit de paridad tiene que ser 0. */
        if ((count % 2 == 0 && parity == 0) ||
            (count % 2 != 0 && parity != 0)) {
            return 1;
        }
    }

    return 0;
}

unsigned char* DES_add_padding(const unsigned char* block, int length,
                               enum DES_padding padding) {
    unsigned char* padding_block = NULL;
    int i, padding_size = DES_BLOCK_LENGTH - length;

    /* Se comprueba que el tamaño del padding no sea mayor que el de bloque.
     */
    if (length < 0 || length > DES_BLOCK_LENGTH) {
        fprintf(stderr, "Error. Tamaño de bloque fuera de rango.\n");
        return NULL;
    }

    /* En caso de que el bloque este vació, el tamaño tiene que corresponder a
     * 0. */
    if (block == NULL && length != 0) {
        fprintf(stderr,
                "Error. Si el bloque esta vació su tamaño tiene que ser 0.\n");
        return NULL;
    }

    /* Se crea un bloque al que añadirle el padding. */
    padding_block = (unsigned char*)calloc(DES_BLOCK_LENGTH, sizeof(char));

    /* Se copian los datos. */
    for (i = 0; i < length; i++) {
        padding_block[i] = block[i];
    }

    /*Se rellena el resto del bloque. */
    switch (padding) {
        /* El padding PKCS#5 consiste en rellenar con el tamaño de padding.
         */
        case PKCS_5:
            for (i = length; i < DES_BLOCK_LENGTH; i++) {
                padding_block[i] = padding_size;
            }
            break;
        /* El padding ANSIX9_23 consiste en rellenar con 0s salvo el último,
         * que se rellena con el tamaño de padding. */
        case ANSIX9_23:
            for (i = length; i < (DES_BLOCK_LENGTH - 1); i++) {
                padding_block[i] = 0;
            }
            padding_block[DES_BLOCK_LENGTH - 1] = padding_size;
            break;
        /* El padding ISO_10126 consiste en rellenar con números aleatorios
         * salvo el último, que se rellena con el tamaño de padding. */
        case ISO_10126:
            srand(time(NULL));
            for (i = length; i < (DES_BLOCK_LENGTH - 1); i++) {
                padding_block[i] = rand() % 256;
            }
            padding_block[DES_BLOCK_LENGTH - 1] = padding_size;
            break;
        /*Si el método no esta soportado, se da error. */
        default:
            fprintf(stderr, "Error. Tipo de padding no soportado.\n");
            free(padding_block);
            return NULL;
            break;
    }
    return padding_block;
}