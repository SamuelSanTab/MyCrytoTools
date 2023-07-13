/**
 * @file des_test.h
 * @author Samuel Sánchez Tabernero
 * @date 13 Jul 2023
 * @brief Funciones de test para el archivo des.h
 * @version 1.0
 *
 * @section DESCRIPCIÓN
 * Las funciones se dividen en diferentes grupos, según la función de la que son
 * test.
 * - test_DES_add_padding_* -> Testan DES_add_padding.
 * - test_DES_check_odd_parity_* -> Testan DES_check_odd_parity.
 * - test_DES_generate_subkeys_* -> Testan DES_generate_subkeys.
 * - test_DES_F_function_* -> Testan DES_F_function.
 * - test_DES_block_cipher_* -> Testan DES_block_cipher.
 */

#ifndef _DES_TEST_H
#define _DES_TEST_H

/*
    TEST DES_add_padding
*/

/**
 * Comprueba que la función reacciona correctamente cuando el tamaño de bloque
 * no es válido.
 *
 * El padding es invalido cuando:
 *  - Su tamaño es menor que 0.
 *  - Su tamaño es mayor que el tamaño de bloque.
 *  - Su tamaño es diferente de 0 cuando el bloque esta en blanco.
 */
void test_DES_add_padding_length_limits();
/**
 * Comprueba que la función reacciona correctamente cuando el tipo de padding no
 * esta soportado.
 */
void test_DES_add_padding_unkown_padding();
/**
 * Comprueba que la función genera correctamente el padding PKCS#5.
 */
void test_DES_add_padding_PKCS_5_padding();
/**
 * Comprueba que la función genera correctamente el padding ANSIX9.23.
 */
void test_DES_add_padding_ANSIX9_23_padding();
/**
 * Comprueba que la función genera correctamente el padding ISO 10126.
 */
void test_DES_add_padding_ISO_10126_padding();

/*
    TEST DES_check_odd_parity
*/

/**
 * Comprueba que la función reacciona correctamente con un array vacío.
 */
void test_DES_check_odd_parity_empty_data();

/**
 * Comprueba que la función reacciona correctamente con un tamaño menor que 1.
 */
void test_DES_check_odd_parity_invalid_length();
/**
 * Comprueba que la función detecta correctamente cuando un byte tiene paridad
 * par.
 */
void test_DES_check_odd_parity_with_even_parity();
/**
 * Comprueba que la función reacciona correctamente cuando todos los bytes
 * tienen paridad impar.
 */
void test_DES_check_odd_parity_with_odd_parity();

/*
    TEST DES_generate_subkeys
*/

/**
 * Comprueba que la función reacciona correctamente cuando no hay clave.
 */
void test_DES_generate_subkeys_empty_key();
/**
 * Comprueba que la función genera las sub-claves correctas (se prueban con las
 * claves débiles del DES).
 */
void test_DES_generate_subkeys_check_subkeys();

/*
    TEST DES_F_function
*/

/**
 * Comprueba que la función falla correctamente cuando el argumento [right] es
 * nulo.
 */
void test_DES_F_function_empty_right();
/**
 * Comprueba que la función falla correctamente cuando el argumento [key] es
 * nulo.
 */
void test_DES_F_function_empty_key();
/**
 * Comprueba que la función genera el sub-bloque correctamente.
 * @return 0 Si se pueden probar las funciones que la usan o cualquier otro número en caso contrario.
 */
int test_DES_F_function_check_block();


/*
    TEST DES_block_cipher
*/

/**
 * Comprueba que la función falla correctamente cuando el argumento [block] es
 * nulo.
 */
void test_DES_block_cipher_empty_block();
/**
 * Comprueba que la función falla correctamente cuando el argumento [keys] es
 * nulo.
 */
void test_DES_block_cipher_empty_keys();
/**
 * Comprueba que la función cifra un bloque correctamente.
 */
void test_DES_block_cipher_encrypt_block();
/**
 * Comprueba que la función descifra un bloque correctamente.
 */
void test_DES_block_cipher_decrypt_block();

#endif