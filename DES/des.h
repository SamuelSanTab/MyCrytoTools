/**
 * @file des.h
 * @author Samuel Sánchez Tabernero
 * @date 13 Jul 2023
 * @brief Funciones básicas para el operar el DES.
 * @version 1.0
 *
 * @section DESCRIPCIÓN
 * Las siguientes funciones sirven para cifrar un bloque, realizar la función F
 * de Feistel del DES, generar las sub-claves, comprobar la paridad de una clave
 * del DES y añadir padding a un bloque.
 */

#ifndef _DES_H
#define _DES_H

#include <stdbool.h>

#include "info_des.h"

/**
 * La función reserva memoria y cifra/descifra un bloque con DES.
 * @param block: Bloque de 64 bits que cifrar.
 * @param keys: Las 16 claves que cifrar.
 * @param mode: true para cifrar y false para descifrar.
 * @return: Bloque de 64 bits cifrado o NULL en caso de error.
 */
unsigned char *DES_block_cipher(const unsigned char *block,
                                const unsigned char **keys, bool mode);

/**
 * La función realiza la función F de Feistel del DES.
 * @param right: Mitad derecha del bloque que se esta cifrando.
 * @param key: Clave que se esta usando.
 * @return Puntero al medio bloque con generado o NULL en caso de error.
 * */
unsigned char *DES_F_function(const unsigned char *right,
                              const unsigned char *key);

/**
 * La función genera las 16 sub-claves de DES.
 * @param key: Clave de 64 bits con la generar el resto de sub-claves.
 * @return En caso de éxito un array de 16 punteros a cada una de las
 * sub-claves (un único array de 96 bytes) o NULL en caso de error.
 */
unsigned char **DES_generate_subkeys(const unsigned char *key);

/**
 * La función comprueba que la paridad impar del cada byte es correcta.
 *
 * La función cuenta el número de bit's a 1 en cada byte y comprueba que
 * corresponda con lo indicado en el bit de paridad impar (el de menos valor).
 *
 * @param data Array con los bytes que comprobar.
 * @param length Tamaño del array [data].
 * @return 0 En caso de que la paridad sea correcta o 1 en caso de que en
 * cualquiera de los bytes este mal.
 */
int DES_check_odd_parity(const unsigned char *data, int length);

/**
 * La función añade padding a un bloque.
 *
 * La función reserva memoria para un nuevo bloque, copia el contenido del
 * bloque [block] y rellena el espacio vació con el tipo de padding indicado.
 *
 * @param block Bloque al que añadir padding. Puede ser NULL si es un bloque
 * únicamente para padding.
 * @param length Tamaño de [block] en bytes.
 * @param padding Tipo de padding que añadir.
 * @return Bloque reservado con el padding o NULL en caso de error.
 */
unsigned char *DES_add_padding(const unsigned char *block, int length,
                           enum DES_padding padding);

#endif