import pandas as pd
import matplotlib.pyplot as plt

# Leer los datos desde el archivo CSV
datos = pd.read_csv('results.csv', delimiter=';')

# Calcular el promedio y la desviación estándar por estructura de datos
promedio = datos.groupby(['estructura_de_datos', 'cantidad_consultas'])['tiempo_ejecucion'].mean()
desviacion = datos.groupby(['estructura_de_datos', 'cantidad_consultas'])['tiempo_ejecucion'].std()

# Crear un gráfico de barras con los resultados
fig, ax = plt.subplots(figsize=(10, 6))
for clave in promedio.index.levels[0]:
    subset = promedio[clave]
    error_subset = desviacion[clave]
    ax.errorbar(subset.index, subset, yerr=error_subset, marker='o', label=clave)

# Personalizar el gráfico
plt.title('Tiempo de Ejecución por Cantidad de Consultas y Estructura de Datos')
plt.xlabel('Cantidad de Consultas')
plt.ylabel('Tiempo de Ejecución')
plt.grid(True)
plt.legend(title='Estructura de Datos')
plt.show()
