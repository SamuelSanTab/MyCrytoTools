/**
 * @file des_test.h
 * @author Samuel Sánchez Tabernero
 * @date 13 Jul 2023
 * @brief Implementación del archivo des_test.h
 * @version 1.0
 * @section DESCRIPCIÓN
 *
 * Las funciones del grupo test_DES_block_cipher_* solo se ejecutan si se
 * verifica que DES_F_function devuelve un resultado correcto con argumentos
 * válidos.
 */

#include "../des.h"

#include <stdio.h>
#include <stdlib.h>

#include "des_test.h"
#define ANSI_COLOR_RED \
    "\x1b[31m" /**< String para cambiar el color de la terminal a rojo. */
#define ANSI_COLOR_GREEN \
    "\x1b[32m" /**< String para cambiar el color de la terminal a verde. */
#define ANSI_COLOR_YELLOW \
    "\x1b[33m" /**< String para cambiar el color de la terminal a amarillo. */
#define ANSI_COLOR_RESET                                                      \
    "\x1b[0m" /**< String para cambiar el color de la terminal a por defecto. \
               */

/* Posibles estados de ejecución de los test. */
enum STATUS {
    SUCCESS, /**< Número de bytes de una clave del DES. */
    FAILURE, /**< Número de bytes de una clave del DES. */
    SKIPPED  /**< Número de bytes de una clave del DES. */
};
#define ROUNDS 16
#define SUBKEY_LENGTH 6

const unsigned char keys[ROUNDS][SUBKEY_LENGTH] = {
    {0x1B, 0x02, 0xEF, 0xFC, 0x70, 0x72}, {0x79, 0xAE, 0xD9, 0xDB, 0xC9, 0xE5},
    {0x55, 0xFC, 0x8A, 0x42, 0xCF, 0x99}, {0x72, 0xAD, 0xD6, 0xDB, 0x35, 0x1D},
    {0x7C, 0xEC, 0x07, 0xEB, 0x53, 0xA8}, {0x63, 0xA5, 0x3E, 0x50, 0x7B, 0x2F},
    {0xEC, 0x84, 0xB7, 0xF6, 0x18, 0xBC}, {0xF7, 0x8A, 0x3A, 0xC1, 0x3B, 0xFB},
    {0xE0, 0xDB, 0xEB, 0xED, 0xE7, 0x81}, {0xB1, 0xF3, 0x47, 0xBA, 0x46, 0x4F},
    {0x21, 0x5F, 0xD3, 0xDE, 0xD3, 0x86}, {0x75, 0x71, 0xF5, 0x94, 0x67, 0xE9},
    {0x97, 0xC5, 0xD1, 0xFA, 0xBA, 0x41}, {0x5F, 0x43, 0xB7, 0xF2, 0xE7, 0x3A},
    {0xBF, 0x91, 0x8D, 0x3D, 0x3F, 0x0A}, {0xCB, 0x3D, 0x8B, 0x0E, 0x17, 0xF5}};

/**
 * La función imprime detalles de ejecución por pantalla.
 *
 * Según el tipo de estatus, los datos se imprimen en:
 * - SUCCESS-> Verde
 * - FAILURE-> Rojo
 * - SKIPPED-> Amarillo
 *
 * @param status: Resultado de la operación.
 * @param data: Información que imprimir.
 */
void print_message(enum STATUS status, char* data) {
    switch (status) {
        case SUCCESS:
            printf("\t" ANSI_COLOR_GREEN "OK:   %s" ANSI_COLOR_RESET, data);
            break;
        case FAILURE:
            printf("\t" ANSI_COLOR_RED "FAIL: %s" ANSI_COLOR_RESET, data);
            break;
        case SKIPPED:
            printf("\t" ANSI_COLOR_YELLOW "SKIP: %s" ANSI_COLOR_RESET, data);
            break;
        default:
            break;
    }
}

/**
 * La función generan unas sub-claves para las pruebas que las requieran. La
 * clave de la que se sacan es 0x133457799BBCDFF1
 * @return Un indice a un array con las sub-claves o NULL en caso de fallar al
 * reservar memoria.
 */
unsigned char** generate_subkeys_for_test() {
    unsigned char* sub_keys = NULL;
    unsigned char** key_index = NULL;
    int i, j;

    sub_keys = (unsigned char*)calloc(ROUNDS, sizeof(char) * SUBKEY_LENGTH);
    if (sub_keys == NULL) {
        return NULL;
    }
    key_index = (unsigned char**)calloc(ROUNDS, sizeof(char*));
    if (key_index == NULL) {
        free(sub_keys);
        return NULL;
    }

    for (i = 0; i < ROUNDS; i++) {
        key_index[i] = &sub_keys[SUBKEY_LENGTH * i];
        for (j = 0; j < SUBKEY_LENGTH; j++) {
            sub_keys[SUBKEY_LENGTH * i + j] = keys[i][j];
        }
    }
    return key_index;
}

int main(void) {
    int check_DES_block_cipher = 1;
    fclose(stderr);

    printf("Test de DES_add_padding\n");
    test_DES_add_padding_length_limits();
    test_DES_add_padding_unkown_padding();
    test_DES_add_padding_PKCS_5_padding();
    test_DES_add_padding_ANSIX9_23_padding();
    test_DES_add_padding_ISO_10126_padding();

    printf("Test de DES_check_odd_parity\n");
    test_DES_check_odd_parity_empty_data();
    test_DES_check_odd_parity_invalid_length();
    test_DES_check_odd_parity_with_even_parity();
    test_DES_check_odd_parity_with_odd_parity();

    printf("Test de DES_generate_subkeys\n");
    test_DES_generate_subkeys_empty_key();
    test_DES_generate_subkeys_check_subkeys();

    printf("Test de DES_F_function\n");
    test_DES_F_function_empty_right();
    test_DES_F_function_empty_key();
    check_DES_block_cipher = test_DES_F_function_check_block();

    printf("Test de DES_block_cipher\n");
    if (check_DES_block_cipher == 0) {
        test_DES_block_cipher_empty_block();
        test_DES_block_cipher_empty_keys();
        test_DES_block_cipher_encrypt_block();
        test_DES_block_cipher_decrypt_block();
    } else {
        print_message(
            SKIPPED,
            "Es necesario que DES_F_function funcione correctamente.\n");
    }

    return 0;
}

void test_DES_add_padding_length_limits() {
    if (DES_add_padding(NULL, -1, PKCS_5) == NULL) {
        print_message(SUCCESS, "DES_add_padding: Check length under 0\n");
    } else {
        print_message(FAILURE, "DES_add_padding: Check length under 0\n");
    }

    if (DES_add_padding(NULL, DES_BLOCK_LENGTH + 1, PKCS_5) == NULL) {
        print_message(SUCCESS, "DES_add_padding: Check length over max.\n");
    } else {
        print_message(FAILURE, "DES_add_padding: Check length over max.\n");
    }

    if (DES_add_padding(NULL, 1, PKCS_5) == NULL) {
        print_message(
            SUCCESS,
            "DES_add_padding: Check the length with an empty block.\n");
    } else {
        print_message(
            FAILURE,
            "DES_add_padding: Check the length with an empty block.\n");
    }
}

void test_DES_add_padding_unkown_padding() {
    if (DES_add_padding(NULL, 0, -1) == NULL) {
        print_message(SUCCESS, "DES_add_padding: Check unkown padding.\n");
    } else {
        print_message(FAILURE, "DES_add_padding: Check unkown padding.\n");
    }
}

void test_DES_add_padding_PKCS_5_padding() {
    const unsigned char block[3] = {0x00, 0x0F, 0xFF};
    unsigned char* padding_block = NULL;

    padding_block = DES_add_padding(block, 3, PKCS_5);
    if (padding_block == NULL) {
        print_message(FAILURE,
                      "DES_add_padding: Check PKCS#5 padding [Empty Block]\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        if (block[i] != padding_block[i]) {
            print_message(FAILURE,
                          "DES_add_padding: Check PKCS#5 padding [Bad Copy]\n");
            free(padding_block);
            return;
        }
    }
    for (int i = 3; i < 8; i++) {
        if (padding_block[i] != 0x05) {
            print_message(
                FAILURE,
                "DES_add_padding: Check PKCS#5 padding [Bad length]\n");
            free(padding_block);
            return;
        }
    }
    free(padding_block);
    print_message(SUCCESS, "DES_add_padding: Check PKCS#5 padding.\n");
    return;
}

void test_DES_add_padding_ANSIX9_23_padding() {
    const unsigned char block[3] = {0x00, 0x0F, 0xFF};
    unsigned char* padding_block = NULL;

    padding_block = DES_add_padding(block, 3, ANSIX9_23);
    if (padding_block == NULL) {
        print_message(
            FAILURE,
            "DES_add_padding: Check ANSI X9.23 padding [Empty Block].\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        if (block[i] != padding_block[i]) {
            print_message(
                FAILURE,
                "DES_add_padding: Check ANSI X9.23 padding [Bad Copy].\n");
            free(padding_block);
            return;
        }
    }
    for (int i = 3; i < 7; i++) {
        if (padding_block[i] != 0x00) {
            print_message(
                FAILURE,
                "DES_add_padding: Check ANSI X9.23 padding [Bad Padding].\n");
            free(padding_block);
            return;
        }
    }
    if (padding_block[7] != 0x05) {
        print_message(
            FAILURE,
            "DES_add_padding: Check ANSI X9.23 padding [Bad length].\n");
        free(padding_block);
        return;
    }
    free(padding_block);
    print_message(SUCCESS, "DES_add_padding: Check ANSI X9.23 padding.\n");
    return;
}

void test_DES_add_padding_ISO_10126_padding() {
    const unsigned char block[3] = {0x00, 0x0F, 0xFF};
    unsigned char* padding_block = NULL;

    padding_block = DES_add_padding(block, 3, ISO_10126);
    if (padding_block == NULL) {
        print_message(
            FAILURE,
            "DES_add_padding: Check ISO 10126 padding [Empty Block].\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        if (block[i] != padding_block[i]) {
            print_message(
                FAILURE,
                "DES_add_padding: Check ISO 10126 padding [Bad Copy].\n");
            free(padding_block);
            return;
        }
    }
    if (padding_block[7] != 0x05) {
        print_message(
            FAILURE,
            "DES_add_padding: Check ISO 10126 padding [Bad length].\n");
        free(padding_block);
        return;
    }
    free(padding_block);
    print_message(SUCCESS, "DES_add_padding: Check ISO 10126 padding.\n");
    return;
}

void test_DES_check_odd_parity_empty_data() {
    if (DES_check_odd_parity(NULL, 2) != 1) {
        print_message(FAILURE, "DES_check_odd_parity: Check Empty Array.\n");
    } else {
        print_message(SUCCESS, "DES_check_odd_parity: Check Empty Array.\n");
    }
}

void test_DES_check_odd_parity_invalid_length() {
    const unsigned char block[3] = {0x00, 0x0F, 0xFF};

    if (DES_check_odd_parity(block, -1) != 1) {
        print_message(FAILURE, "DES_check_odd_parity: Check invalid length.\n");
    } else {
        print_message(SUCCESS, "DES_check_odd_parity: Check invalid length.\n");
    }
}

void test_DES_check_odd_parity_with_even_parity() {
    const unsigned char block[3] = {0b00000001, 0b10100011, 0b11111110};

    if (DES_check_odd_parity(block, 3) != 1) {
        print_message(
            FAILURE,
            "DES_check_odd_parity: Check with even parity [False Negative].\n");
    } else {
        print_message(SUCCESS,
                      "DES_check_odd_parity: Check with even parity.\n");
    }
}

void test_DES_check_odd_parity_with_odd_parity() {
    const unsigned char block[4] = {0b00000001, 0b10100010, 0b11010011,
                                    0b11111110};

    if (DES_check_odd_parity(block, 3) == 1) {
        print_message(
            FAILURE,
            "DES_check_odd_parity: Check odd parity [False Positive].\n");
    } else {
        print_message(SUCCESS, "DES_check_odd_parity: Check odd parity.\n");
    }
}

void test_DES_generate_subkeys_empty_key() {
    if (DES_generate_subkeys(NULL) == NULL) {
        print_message(SUCCESS, "DES_generate_subkeys: Check Empty Key.\n");
    } else {
        print_message(FAILURE, "DES_generate_subkeys: Check Empty Key.\n");
    }
}

void test_DES_generate_subkeys_check_subkeys() {
    const unsigned char key[4][8] = {
        {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
        {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE},
        {0xE0, 0xE0, 0xE0, 0xE0, 0xF1, 0xF1, 0xF1, 0xF1},
        {0x1F, 0x1F, 0x1F, 0x1F, 0x0E, 0x0E, 0x0E, 0x0E}};
    const unsigned char subkey[4][6] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                                        {0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00},
                                        {0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF}};
    unsigned char** temp_subkey = NULL;

    for (int i = 0; i < 4; i++) {
        temp_subkey = DES_generate_subkeys(key[i]);
        if (temp_subkey == NULL) {
            print_message(
                FAILURE,
                "DES_generate_subkeys: Check subkeys [Empty Subkey].\n");
            return;
        }
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 6; k++) {
                if (temp_subkey[j][k] != subkey[i][k]) {
                    print_message(FAILURE,
                                  "DES_generate_subkeys: Check subkeys "
                                  "[Invalid Subkey].\n");
                    free(*temp_subkey);
                    free(temp_subkey);
                    return;
                }
            }
        }

        free(*temp_subkey);
        free(temp_subkey);
    }

    print_message(SUCCESS, "DES_generate_subkeys: Check subkeys.\n");
}

void test_DES_F_function_empty_right() {
    const unsigned char key[] = {0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF};

    if (DES_F_function(NULL, key) == NULL) {
        print_message(SUCCESS, "test_DES_F_function: Empty right.\n");
    } else {
        print_message(FAILURE, "test_DES_F_function: Empty right.\n");
    }
}

void test_DES_F_function_empty_key() {
    const unsigned char right[] = {0x00, 0x00, 0x00, 0x00};

    if (DES_F_function(right, NULL) == NULL) {
        print_message(SUCCESS, "test_DES_F_function: Empty key.\n");
    } else {
        print_message(FAILURE, "test_DES_F_function: Empty key.\n");
    }
}

int test_DES_F_function_check_block() {
    const unsigned char right[] = {0xF0, 0xAA, 0xF0, 0xAA};
    const unsigned char key[] = {0x1B, 0x02, 0xEF, 0xFC, 0x70, 0x72};
    unsigned char right_block[] = {0x23, 0x4A, 0xA9, 0xBB};

    unsigned char* new_right = NULL;

    new_right = DES_F_function(right, key);
    if (new_right == NULL) {
        print_message(FAILURE,
                      "test_DES_F_function: Check block [Empty Block].\n");
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        if (new_right[i] != right_block[i]) {
            free(new_right);
            print_message(
                FAILURE, "test_DES_F_function: Check block [Invalid Block].\n");
            return 1;
        }
    }

    free(new_right);
    print_message(SUCCESS, "test_DES_F_function: Check block.\n");
    return 0;
}

void test_DES_block_cipher_empty_block() {
    unsigned char** keys = generate_subkeys_for_test();
    if (keys == NULL) {
        print_message(SKIPPED,
                      "test_DES_F_function: Empty right [Not keys].\n");
        return;
    }

    if (DES_block_cipher(NULL, (const unsigned char**)keys, true) == NULL) {
        print_message(SUCCESS, "test_DES_F_function: Empty right.\n");
    } else {
        print_message(FAILURE, "test_DES_F_function: Empty right.\n");
    }
    free(*keys);
    free(keys);
}

void test_DES_block_cipher_empty_keys() {
    const unsigned char plain_block[] = {0x01, 0x23, 0x45, 0x67,
                                         0x89, 0xAB, 0xCD, 0xEF};

    if (DES_block_cipher(plain_block, NULL, true) == NULL) {
        print_message(SUCCESS, "test_DES_F_function: Empty keys.\n");
    } else {
        print_message(FAILURE, "test_DES_F_function: Empty keys.\n");
    }
}

void test_DES_block_cipher_encrypt_block() {
    const unsigned char plain_block[] = {0x01, 0x23, 0x45, 0x67,
                                         0x89, 0xAB, 0xCD, 0xEF};
    const unsigned char encrypt_block[] = {0x85, 0xE8, 0x13, 0x54,
                                           0x0F, 0x0A, 0xB4, 0x05};
    unsigned char* block = NULL;
    unsigned char** keys = generate_subkeys_for_test();
    if (keys == NULL) {
        print_message(SKIPPED,
                      "test_DES_F_function: Encrypt block [Not keys].\n");
        return;
    }

    block = DES_block_cipher(plain_block, (const unsigned char**)keys, true);
    free(*keys);
    free(keys);
    if (block == NULL) {
        print_message(FAILURE,
                      "test_DES_F_function: Encrypt block [Empty Block].\n");
        return;
    }

    for (int i = 0; i < 8; i++) {
        if (block[i] != encrypt_block[i]) {
            free(block);
            print_message(
                FAILURE,
                "test_DES_F_function: Encrypt block [Invalid Block].\n");
            return;
        }
    }

    free(block);
    print_message(SUCCESS, "test_DES_block_cipher: Encrypt block.\n");
}

void test_DES_block_cipher_decrypt_block() {
    const unsigned char plain_block[] = {0x01, 0x23, 0x45, 0x67,
                                         0x89, 0xAB, 0xCD, 0xEF};
    const unsigned char encrypt_block[] = {0x85, 0xE8, 0x13, 0x54,
                                           0x0F, 0x0A, 0xB4, 0x05};
    unsigned char* block = NULL;
    unsigned char** keys = generate_subkeys_for_test();
    if (keys == NULL) {
        print_message(SKIPPED,
                      "test_DES_F_function: Decrypt block [Not keys].\n");
        return;
    }

    block = DES_block_cipher(encrypt_block, (const unsigned char**)keys, false);
    free(*keys);
    free(keys);
    if (block == NULL) {
        print_message(FAILURE,
                      "test_DES_F_function: Decrypt block [Empty Block].\n");
        return;
    }

    for (int i = 0; i < 8; i++) {
        if (block[i] != plain_block[i]) {
            free(block);
            print_message(
                FAILURE,
                "test_DES_F_function: Decrypt block [Invalid Block].\n");
            return;
        }
    }

    free(block);
    print_message(SUCCESS, "test_DES_block_cipher: Decrypt block.\n");
}
