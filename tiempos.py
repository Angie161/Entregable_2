# -*- coding: utf-8 -*-
import csv
import numpy as np

# Nombre del archivo CSV
archivo_csv = 'results.csv'

# Lista para almacenar los tiempos de ejecución
tiempos_ejecucion = []

# Leer el archivo CSV y extraer los tiempos de ejecución
with open(archivo_csv, mode='r') as archivo:
    lector_csv = csv.DictReader(archivo, delimiter=';')
    for fila in lector_csv:
        tiempos_ejecucion.append(int(fila['tiempo_ejecucion']))

# Calcular el promedio y la desviación estándar
promedio = np.mean(tiempos_ejecucion)
desviacion_estandar = np.std(tiempos_ejecucion, ddof=1)

# Imprimir los resultados usando el método format() para Python 2
print('Promedio del tiempo de ejecución: {}'.format(promedio))
print('Desviación estándar del tiempo de ejecución: {}'.format(desviacion_estandar))
