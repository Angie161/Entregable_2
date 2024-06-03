#!/bin/bash

# Crear archivos CSV con cabeceras
echo "numero_experimento;dataset;estructura_de_datos;clave;cantidad_consultas;tiempo_ejecucion"

for n in 100 500 #1000 1500 2000
do
    ./a $n
    ./b $n
    ./c $n
done
