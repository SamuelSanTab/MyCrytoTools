#ifndef DES
#define DES

#include <stdbool.h>

enum DES_padding { PKCS_5, ANSIX9_23, ISO_10126 };

/*Modo de operación ECB*/
unsigned char* DES_ECB_encrypt_data(const unsigned char* plain_text,
                                    enum DES_padding padding_mode);

unsigned char* DES_ECB_decrypt_data(const unsigned char* encrypted_text);

int DES_ECB_encrypt_file(const char* plain_file, const char* output_file,
                         enum DES_padding padding_mode);

int DES_ECB_decrypt_file(const char* encrypted_file);

/*Modo de operación CBC*/
unsigned char* DES_CBC_encrypt_data(const unsigned char* plaintext,
                                    const unsigned char* iv,
                                    enum DES_padding padding_mode);

unsigned char* DES_CBC_decrypt_data(const unsigned char* encrypted_text,
                                    const unsigned char* iv);

int DES_CBC_encrypt_file(const char* plain_file, const char* output_file,
                         const unsigned char* iv,
                         enum DES_padding padding_mode);

int DES_CBC_decrypt_file(const char* encrypted_file, const unsigned char* iv);

/*Modo de operación PCBC*/
unsigned char* DES_PCBC_encrypt_data(const unsigned char* plaintext,
                                     const unsigned char* iv,
                                     enum DES_padding padding_mode);

unsigned char* DES_PCBC_decrypt_data(const unsigned char* encrypted_text,
                                     const unsigned char* iv);

int DES_PCBC_encrypt_file(const char* plain_file, const char* output_file,
                          const unsigned char* iv,
                          enum DES_padding padding_mode);

int DES_PCBC_decrypt_file(const char* encrypted_file, const unsigned char* iv);

/*Modo de operación CFB*/
unsigned char* DES_CFB_encrypt_data(const unsigned char* plaintext,
                                    const unsigned char* iv, int block_size,
                                    enum DES_padding padding_mode);

unsigned char* DES_CFB_decrypt_data(const unsigned char* encrypted_text,
                                    const unsigned char* iv, int block_size);

int DES_CFB_encrypt_file(const char* plain_file, const char* output_file,
                         const unsigned char* iv, int block_size,
                         enum DES_padding padding_mode);

int DES_CFB_decrypt_file(const char* encrypted_file, const unsigned char* iv,
                         int block_size);

/*Modo de operación OFB*/
unsigned char* DES_OFB_data(const unsigned char* input_text,
                            const unsigned char* iv, int block_size,
                            enum DES_padding padding_mode);

int DES_OFB_file(const char* input_file, const char* output_file,
                 const unsigned char* iv, int block_size,
                 enum DES_padding padding_mode);

/*Modo de operación CTR*/
unsigned char* DES_CTR_data(const unsigned char* input_text,
                            const unsigned char* counter, int block_size,
                            enum DES_padding padding_mode);

int DES_CTR_file(const char* input_file, const char* output_file,
                 const unsigned char* counter, int block_size,
                 enum DES_padding padding_mode);

/*Funciones internas para el funcionamiento del DES.*/

/**
 * La función reserva memoria y cifra/descifra un bloque con DES.
 * @param block: Bloque de 64 bits que cifrar.
 * @param keys: Las 16 claves que cifrar.
 * @param mode: true para cifrar y false para descifrar.
 * @return: Bloque de 64 bits cifrado o NULL en caso de error.
 */
unsigned char* DES_block_cipher(const unsigned char* block,
                                const unsigned char** keys, bool mode);

/**
 * La función realiza la función F de Feistel.
 * @param right: Mitad derecha del bloque que se esta cifrando.
 * @param key: Clave que se esta usando.
 * @return Puntero al medio bloque con generado o NULL en caso de error.
 * */
unsigned char* DES_F_function(const unsigned char* right,
                              const unsigned char* key);

/**
 * La función genera las 16 sub-claves de DES.
 * @param key: Clave de 64 bits con la generar el resto de sub-claves.
 * @return En caso de éxito un array de 16 punteros a cada una de las sub-claves
 * (un único array de 96 bytes) o NULL en caso de error.
 */
unsigned char** DES_generate_subKeys(const unsigned char* key);

/**
 * La función comprueba que la paridad impar del cada byte es correcta (Esta
 * indicado en el bit de menor valor de cada byte).
 * @param data Array con los bytes que comprobar.
 * @param length Tamaño del array [data].
 * @return 0 En caso de que la paridad sea correcta o 1 en caso de que en
 * cualquiera de los bytes este mal.
 *
 */
int checkOddParity(const unsigned char* data, int length);

#endif