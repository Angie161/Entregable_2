#!/bin/bash

# Imprimimos una cabecera para nuestro archivo CSV
echo "numero_experimento;dataset;estructura_de_datos;cantidad_consultas;tiempo_ejecucion"

for n in 1000 5000 10000 15000 20000
do
    # Ejecuta el programa ./a con el tamaño de entrada $n y guarda la salida en la variable resultado
   ./a $n
done
