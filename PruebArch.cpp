#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <math.h>

using namespace std;

/***************************/
/*** Almacenaje de Datos ***/
/***************************/
struct Datos{
	string universidad;
	double userId;
	string userName;
	int numTweets;
	int numAmigos;
	int numSeguidos;
	string createdAt;
};

void ImprimirDatos(Datos usuario){
		cout << "Universidad: " << usuario.universidad << endl;
	    cout << "User ID: " << usuario.userId << endl;
	    cout << "User Name: " << usuario.userName << endl;
	    cout << "Number of Tweets: " << usuario.numTweets << endl;
	    cout << "Friends Count: " << usuario.numAmigos << endl;
	    cout << "Followers Count: " << usuario.numSeguidos << endl;
	    cout << "Created At: " << usuario.createdAt<< "\n" << endl;
}

/**********************/
/*** Funciones hash ***/
/**********************/
const float A = (sqrt(5) - 1) / 2;

// Método de la división
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
int h1(int k, int n)
{
    return k % n;
}

// Método de la multiplicación
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
int h2(int k, int n)
{
    float a = (float)k * A;
    a -= (int)a; // Esta línea implementa la operación módulo 1 (%1)

    return n * a;
}

/****************************************************/
/*** Métodos de Open addressing o hashing cerrado ***/
/****************************************************/

// Linear probing
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
// i: número del intento
int linear_probing(int k, int n, int i)
{
    // Utilizando el método de la division
    return (h1(k, n) + i) % n;
}

// Quadratic probing
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
// i: número del intento
int quadratic_probing(int k, int n, int i)
{
    // Utilizando el método de la division
    return (h1(k, n) + i + 2*i*i) % n;
}

// Double hashing
// k: clave a la cual aplicaremos la función hash
// n: tamaño de la tabla hash
// i: número del intento
int double_hashing(int k, int n, int i)
{
    // Utilizando como primer método el método de la division y luego el
    // método de la multiplicacion
    return (h1(k, n) + i * (h2(k, n) + 1)) % n;
}

class HashTable{
public:
    int size;
    int *table;
    int (*hashing_method)(int, int, int);

    HashTable(int size, int (*hashing_method)(int, int, int)) : size(size), hashing_method(hashing_method)
    {
        table = new int[size];
        for (int i = 0; i < size; i++)
        {
            table[i] = -1;
        }
    }

    void insert(int key)
    {
        int i = 0;
        while (table[hashing_method(key, size, i)] != -1)
        {
            i++;
        }
        table[hashing_method(key, size, i)] = key;
    }

    bool search(int key)
    { 
        int i = 0;
        while (table[hashing_method(key, size, i)] != key && table[hashing_method(key, size, i)] != -1)
        {
            i++;
        }
        return table[hashing_method(key, size, i)] == key;
    }
};

int main(int argc, char const *argv[]){

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
		getline(file, line);
		istringstream ss(line);
    	string token;

    	Datos datos;
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

        ImprimirDatos(datos);
        } catch (const std::invalid_argument& e) {
        	std::cerr << "Error: argumento inválido al convertir a número en la línea: " << line << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: número fuera de rango en la línea: " << line << std::endl;
        }
	}
	file.close();
    std::cout << "Datos procesados correctamente" << std::endl;

	return 0;
}
