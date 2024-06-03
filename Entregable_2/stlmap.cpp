#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <math.h>
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

struct Datos
{
	string universidad;
	double userId;
	string userName;
	int numTweets;
	int numAmigos;
	int numSeguidos;
	string createdAt;
};

void insercionID(unordered_map<double, Datos> &tabla, Datos datos)
{
	tabla[datos.userId] = datos;
}
void insercionName(unordered_map<string, Datos> &tabla, Datos datos)
{
	tabla[datos.userName] = datos;
}

auto busquedaID(unordered_map<double, Datos> &tabla, Datos datos)
{
	return tabla.find(datos.userId);
}
auto busquedaName(unordered_map<string, Datos> &tabla, Datos datos)
{
	return tabla.find(datos.userName);
}

Datos transStruct(ifstream &archivo)
{
	Datos datos;
	string line;
	getline(archivo, line);
	istringstream ss(line);
	string token;

	try
	{
		getline(ss, token, ',');
		datos.universidad = token;
		getline(ss, token, ',');
		datos.userId = stod(token);
		getline(ss, token, ',');
		datos.userName = token;
		getline(ss, token, ',');
		datos.numTweets = stoi(token);
		getline(ss, token, ',');
		datos.numAmigos = stoi(token);
		getline(ss, token, ',');
		datos.numSeguidos = stoi(token);
		getline(ss, token, ',');
		datos.createdAt = token;
	}
	catch (const std::invalid_argument &e)
	{
		std::cerr << "Error: argumento inválido al convertir a número en la línea: " << line << std::endl;
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << "Error: número fuera de rango en la línea: " << line << std::endl;
	}

	return datos;
}

int main(int argc, char const *argv[])
{
	unordered_map<double, Datos> tablaId(40000);
	unordered_map<string, Datos> tablaName(40000);
	Datos datos;
	if (argc < 2)
	{
		cerr << "Usage: " << argv[0] << " <cantidad_elementos>" << endl;
		exit(1);
	}
	int cantElem = atoi(argv[1]);
	/****************************************************/
	/****Apertura y tranformación de datos a structs*****/
	/****************************************************/
	ifstream file("universities_followers.csv");
	if (!file.is_open())
	{
		cout << "Error al abrir el archivo" << endl;
	}

	string line;
	getline(file, line);
	for (int experimento = 1; experimento <= 20; ++experimento)
	{
		file.clear();  // Limpia el estado del flujo
		file.seekg(0); // Mueve el puntero al inicio del archivo
		getline(file, line);
		for (int i = 0; i < cantElem; i++)
		{

			if (!getline(file, line))
			{
				cerr << "Error: Se ha alcanzado el final del archivo o la línea está vacía." << endl;
				break; // Salir del bucle si no hay más líneas para leer
			}
			datos = transStruct(file);

			// Calculo de tiempo de inserción: PRUEBA PARA ID
			auto startID = chrono::high_resolution_clock::now();
			insercionID(tablaId, datos);
			auto endID = chrono::high_resolution_clock::now();
			auto durationID = chrono::duration_cast<chrono::nanoseconds>(endID - startID);
			cout << experimento << ";universities_followers;unordered_map;ID;" << cantElem << ";" << durationID.count() << endl;

			// Calculo de tiempo de inserción: PRUEBA PARA NAME
			auto startNAME = chrono::high_resolution_clock::now();
			insercionID(tablaId, datos);
			auto endNAME = chrono::high_resolution_clock::now();
			auto durationNAME = chrono::duration_cast<chrono::nanoseconds>(endNAME - startNAME);
			cout << experimento << ";universities_followers;unordered_map;Name;" << cantElem << ";" << durationNAME.count() << endl;
		}
	}
	file.close();
	return 0;
}