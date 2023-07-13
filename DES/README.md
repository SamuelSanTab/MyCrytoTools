# Estructura de archivos
## Makefile
El makefile sirve para ejecutar todos los test de la carpeta /test
## /test
La carpeta test contiene pares de archivos .c y .h en los que se testan los módulos que se han creado para el DES (los pares de .c y .h).
## info_des.h
Contiene información relacionada al algoritmo del DES.
## des.c y des.h
Estos archivos contienen primitivas para el funcionamiento del DES, por lo que no tienen una interfaz pensada para cifrar cadenas de bytes ni archivos, para ello están los siguientes archivos. 
# Decisiones de diseño.
## Endianness.
Todos bytes en forma de ```unsigned char``` se leen en formato big-endian y en cada byte, el bit más a la izquierda (el de más valor) se le considera el primer bit del byte.