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

void insercionID(unordered_map<double,Datos>& tabla, Datos datos){
	tabla[datos.userId]=datos;
}
void insercionName(unordered_map<string,Datos>& tabla, Datos datos){
	tabla[datos.userName]=datos;
}

auto busquedaID(unordered_map<double,Datos>& tabla, Datos datos){
	return tabla.find(datos.userId);
}
auto busquedaName(unordered_map<string,Datos>& tabla, Datos datos){
	return tabla.find(datos.userName);
}

Datos transStruct(ifstream& archivo){
	Datos datos;
	string line;
	getline(archivo, line);
	istringstream ss(line);
	string token;

	try {
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

    } catch (const std::invalid_argument& e) {
    	std::cerr << "Error: argumento inválido al convertir a número en la línea: " << line << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: número fuera de rango en la línea: " << line << std::endl;
    }

    return datos;
}

int main(int argc, char const *argv[])
{
	unordered_map<double,Datos> tablaId(40000);
	unordered_map<string,Datos> tablaName(40000);
	Datos datos;

	/****************************************************/
	/****Apertura y tranformación de datos a structs*****/
	/****************************************************/
	ifstream file("universities_followers.csv");
	if(!file.is_open()){
		cout<<"Error al abrir el archivo"<<endl;
	}

	string line;
	getline(file, line);

	for(int i=0; i<10; i++){
		datos=transStruct(file);

        //Calculo de tiempo de inserción: PRUEBA PARA ID
        auto startID = chrono::high_resolution_clock::now();
	    insercionID(tablaId, datos);
	    auto endID = chrono::high_resolution_clock::now();
	    auto durationID = chrono::duration_cast<chrono::nanoseconds>(endID - startID).count();

	    //Calculo de tiempo de inserción: PRUEBA PARA NAME
        auto startNAME = chrono::high_resolution_clock::now();
	    insercionID(tablaId, datos);
	    auto endNAME = chrono::high_resolution_clock::now();
	    auto durationNAME = chrono::duration_cast<chrono::nanoseconds>(endNAME - startNAME).count();

	}
	file.close();
    std::cout << "Datos procesados correctamente" << std::endl;


	/****************************************************/
	/**********Busqueda de datos para claves ID**********/
	/****************************************************/
    ifstream fileAux("universities_followers.csv");
	if(!fileAux.is_open()){
		cout<<"Error al abrir el archivo"<<endl;
	}

	getline(fileAux, line);
	for(int i=0; i<20; i++){
		datos= transStruct(fileAux);

        //Calculo de tiempo de busqueda: PARA ID
        auto start = chrono::high_resolution_clock::now();
	    auto iterador= busquedaID(tablaId, datos);
	    auto end = chrono::high_resolution_clock::now();
	    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

	    if(iterador!= tablaId.end()){
	    	cout << "La clave: " << iterador ->first <<"sí se encuentra almacenada"<< endl;
	    }
	    else{
	    	cout << "El dato no fue encontrado"<< endl;
	    }
	}
	fileAux.close();

	return 0;
}