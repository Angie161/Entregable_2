#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <math.h>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

/***************************/
/*** Almacenaje de Datos ***/
/***************************/
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

void ImprimirDatos(Datos usuario)
{
		cout << "Universidad: " << usuario.universidad << endl;
	    cout << "User ID: " << usuario.userId << endl;
	    cout << "User Name: " << usuario.userName << endl;
	    cout << "Number of Tweets: " << usuario.numTweets << endl;
	    cout << "Friends Count: " << usuario.numAmigos << endl;
	    cout << "Followers Count: " << usuario.numSeguidos << endl;
	    cout << "Created At: " << usuario.createdAt<< "\n" << endl;
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

/**********************/
/*** Funciones hash ***/
/**********************/

//Famosa función murmurhash3_32 que permite transformar una clave de tipo string en un entero sin signo de 32 bits de largo (unit32_t) 
uint32_t murmurhash3_32(const uint8_t* key, size_t len, uint32_t seed) {
    uint32_t h = seed;
    if (len > 3) {
        size_t i = len >> 2;
        do {
            uint32_t k = *reinterpret_cast<const uint32_t*>(key);
            key += sizeof(uint32_t);
            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;
            h ^= k;
            h = (h << 13) | (h >> 19);
            h = h * 5 + 0xe6546b64;
        } while (--i);
    }
    if (len & 3) {
        size_t i = len & 3;
        uint32_t k = 0;
        do {
            k <<= 8;
            k |= key[i - 1];
        } while (--i);
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        h ^= k;
    }
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

unsigned int hAuxName(const std::string &key, int n) {
    const uint8_t* keyPtr = reinterpret_cast<const uint8_t*>(key.c_str());
    size_t len = key.length();
    uint32_t seed = 0x9747b28c; // Valor de semilla aleatorio
    return murmurhash3_32(keyPtr, len, seed) % n;
}


// Método de la multiplicación
// k: clave Id
// n: tamaño de la tabla hash
const float A = (sqrt(5) - 1) / 2;
unsigned int hAuxId(double k, int n)
{
    float a = (float)k * A;
    a -= (int)a; // Esta línea implementa la operación módulo 1 (%1)

    return n * a;
}

//Función hash para el User Name. Obtenido de laboratorio.
unsigned int hashName(const string &key, int n)
{
    unsigned int aux = 2166136261;
    unsigned int aux2 = 16777619;
    unsigned int hash = aux;
    for (int i = 0; i < key.length(); ++i)
    {
        hash ^= key[i];
        hash *= aux2;
    }
    return hash % n;
}

//Función hash para el User ID. Basado en el método Mid-Square
unsigned int hashId(double k, int n) 
{
	long long intId = static_cast<long long>(k);
	return intId % n;
}


/****************************************************/
/*** Métodos de Open addressing o hashing cerrado ***/
/****************************************************/

// Linear probing
// datos: struct de donde sacaremos la clave
// n: tamaño de la tabla hash
// i: número del intento
// type: identificador del tipo de tabla hash que tenemos. 1)Clave ID, 2)Clave Name.
int linear_probing(Datos datos, int n, int i, int type)
{
	if(type == 1){
    	return (hashId(datos.userId, n) + i) % n;
	}
	else if(type == 2){
		return (hashName(datos.userName, n) + i) % n;
	}
	else{
		cout << "Error: No se reconoce el tipo de clave para el linear_probing" << endl;
	}
    
}

// Quadratic probing
// datos: struct de donde sacaremos la clave
// n: tamaño de la tabla hash
// i: número del intento
// type: identificador del tipo de tabla hash que tenemos. 1)Clave ID, 2)Clave Name.
int quadratic_probing(Datos datos, int n, int i, int type)
{
	if(type == 1){
    	return (hashId(datos.userId, n) + i + 2*i*i) % n;
	}
	else if(type == 2){
		return (hashName(datos.userName, n) + i + 2*i*i) % n;
	}
	else{
		cout << "Error: No se reconoce el tipo de clave para el quadratic_probing" << endl;
	} 
}

// Double hashing
// datos: struct de donde sacaremos la clave
// n: tamaño de la tabla hash
// i: número del intento
// type: identificador del tipo de tabla hash que tenemos. 1)Clave ID, 2)Clave Name.
int double_hashing(Datos datos, int n, int i, int type) {
    if (type == 1) {
        return (hashId(datos.userId, n) + i * (hAuxId(datos.userId, n) + 1)) % n;
    } else if (type == 2) {
        // Usamos la función hashName y hAuxName para el sondeo doble
        return (hashName(datos.userName, n) + i * hAuxName(datos.userName, n)) % n;
    } else {
        cout << "Error: No se reconoce el tipo de clave para el double_hashing" << endl;
        return -1;
    }
}

/**********************/
/***** Tabla hash *****/
/**********************/
class HashTable
{
public:
    int size;
    Datos *table;
    Datos nulo = {"-1", -1, "-1", -1, -1, -1, "-1"};
    int tipo;
    int (*hashing_method)(Datos, int, int, int);

    HashTable(int size, int type, int (*hashing_method)(Datos, int, int, int)) : size(size), hashing_method(hashing_method)
    {
        table = new Datos[size];
        tipo = type;
        for (int i = 0; i < size; i++)
        {
            table[i] = nulo;
        }
    }

    void insert(Datos datos)
    {
        int i = 0;
        if(tipo == 1){
        	while (table[hashing_method(datos, size, i, tipo)].userId != -1)
	        {
	            i++;
	            
	        }
	        table[hashing_method(datos, size, i, tipo)] = datos;
        }

        else if(tipo == 2){
        	while (table[hashing_method(datos, size, i, tipo)].userName != "-1")
	        {
	            i++;
	        }
	        table[hashing_method(datos, size, i, tipo)] = datos;
        }
        else{
        	cout << "Error al reconocer tipo de dato" << endl;
        }
        
    }

    bool search(Datos datos)
    { 
        int i = 0;
        int index;
        if (tipo == 1) {
        while (true) {
             index = double_hashing(datos, size, i, tipo);
            cout << "Intentando indice: " << index << ", userid: " << table[index].userId << endl;
            if (table[index].userId == datos.userId || table[index].userId == -1) {
                break;
            }
            i++;
            cout << "Se suma uno a la busqueda" << endl;
            if (i >= size) {
                cout << "Busqueda de userId excedio el tamano de la tabla hash" << endl;
                return false;
            }
        }
        return table[index].userId == datos.userId;
        
        }else if (tipo == 2) {
        while (true) {
             index = double_hashing(datos, size, i, tipo);
            cout << "Intentando indice: " << index << ", userName: " << table[index].userName << endl;
            if (table[index].userName == datos.userName || table[index].userName == "-1") {
                break;
            }
            i++;
            cout << "Se suma uno a la busqueda" << endl;
            if (i >= size) {
                cout << "Busqueda de userName excedio el tamano de la tabla hash" << endl;
                return false;
            }
        }
        cout << "Se encontro el valor o -1" << endl;
        return table[index].userName == datos.userName;
	    } 

	    else {
	        cout << "Error al reconocer tipo de dato" << endl;
	    }
	    return false;
	}
};


/**********************/
/******** Main ********/
/**********************/
int main(int argc, char const *argv[]){
	Datos datos;

	//INSERCIONES PARA ID
  	HashTable ht_linear_ID(20, 1, double_hashing);
  	HashTable ht_linear_Name(20, 2, double_hashing);

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
	    ht_linear_ID.insert(datos);
	    auto endID = chrono::high_resolution_clock::now();
	    auto durationID = chrono::duration_cast<chrono::nanoseconds>(endID - startID).count();

	    //Calculo de tiempo de inserción: PRUEBA PARA NAME
        auto startNAME = chrono::high_resolution_clock::now();
	    ht_linear_Name.insert(datos);
	    auto endNAME = chrono::high_resolution_clock::now();
	    auto durationNAME = chrono::duration_cast<chrono::nanoseconds>(endNAME - startNAME).count();

	}
	file.close();
    std::cout << "Datos procesados correctamente" << std::endl;

    //BUSQUEDA PARA ID
    bool guardadoID;
    bool guardadoNAME;
    ifstream fileAux("universities_followers.csv");
	if(!fileAux.is_open()){
		cout<<"Error al abrir el archivo"<<endl;
	}
	getline(fileAux, line);
	for(int i=0; i<20; i++){
		datos= transStruct(fileAux);

        //Calculo de tiempo de busqueda: PARA ID
        auto startID = chrono::high_resolution_clock::now();
        cout<<"Se quiere encontrar el userID: "<< datos.userId <<endl;
	    guardadoID = ht_linear_ID.search(datos);
	    auto endID = chrono::high_resolution_clock::now();
	    auto durationID = chrono::duration_cast<chrono::nanoseconds>(endID - startID).count();

	    if(guardadoID){
	    	cout << "La clave: " << datos.userId <<"sí se encuentra almacenada\n"<< endl;
	    }
	    else{
	    	cout << "El dato no fue encontrado en la tabla id \n"<< endl;
	    }

	    //Calculo de tiempo de busqueda: PARA NAME
        //auto startNAME = chrono::high_resolution_clock::now();
        //cout<<"Se quiere encontrar el userName: "<< datos.userName <<endl;
	    //guardadoNAME = ht_linear_Name.search(datos);
	    //auto endNAME = chrono::high_resolution_clock::now();
	    //auto durationNAME = chrono::duration_cast<chrono::nanoseconds>(endNAME - startNAME).count();

	    //if(guardadoNAME){
	    //	cout << "La clave: " << datos.userName <<"sí se encuentra almacenada\n"<< endl;
	    //}
	    //else{
	    //	cout << "El dato no fue encontrado en la tabla name \n"<< endl;
	    //}
	}
	fileAux.close();




	return 0;
}
