#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <chrono>
#include <vector>
#include <list>

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

void ImprimirDatos(Datos usuario)
{
    cout << "Universidad: " << usuario.universidad << endl;
    cout << "User ID: " << usuario.userId << endl;
    cout << "User Name: " << usuario.userName << endl;
    cout << "Number of Tweets: " << usuario.numTweets << endl;
    cout << "Friends Count: " << usuario.numAmigos << endl;
    cout << "Followers Count: " << usuario.numSeguidos << endl;
    cout << "Created At: " << usuario.createdAt << "\n"
         << endl;
}

class HashTable
{
private:
    vector<list<Datos>> table;
    int tableSize;

    // Hash
    int hashName(const std::string &key)
    {
        unsigned int aux = 2166136261;
        unsigned int aux2 = 16777619;
        unsigned int hash = aux;
        for (int i = 0; i < key.length(); ++i)
        {
            hash ^= key[i];
            hash *= aux2;
        }
        return hash % tableSize;
    }

    int hashId(double userId)
    {
        long long intId = static_cast<long long>(userId);
        return intId % tableSize;
    }

public:
    HashTable(int size) : tableSize(size)
    {
        table.resize(tableSize);
    }
    // INSERT USER ID
    void insertUserId(const Datos &data)
    {
        int index = hashId(data.userId);
        table[index].push_back(data);
    }
    // INSERT USER NAME
    void insertUsername(const Datos &data)
    {
        int index = hashName(data.userName);
        table[index].push_back(data);
    }

    // SEARCH USER ID
    bool searchUserId(double userId)
    {
        int index = hashId(userId);
        for (const auto &data : table[index])
        {
            if (data.userId == userId)
            {
                return true;
            }
        }
        return false;
    }
    // SEARCH USER NAME
    bool searchUserName(std::string userName)
    {
        int index = hashName(userName);
        for (const auto &data : table[index])
        {
            if (data.userName == userName)
            {
                return true;
            }
        }
        return false;
    }

    // PARA REVISAR LAS INSERSIONES
    // void displayHashTable() {
    //     for (int i = 0; i < tableSize; ++i) {
    //         cout << "Puesto " << i << ": ";
    //         for (const auto& usuario : table[i]) {
    //             cout << usuario.userName << " -> ";
    //         }
    //         cout << "NULL" << endl;
    //     }
    // }
};

int main(int argc, char const *argv[])
{
    HashTable ht(20000);

    // Variables para almacenar los resultados
    ofstream resultsFile("results.csv", ios::out);
    if (!resultsFile.is_open())
    {
        cerr << "Error al abrir el archivo de resultados." << endl;
        return 1;
    }
    // Cabecera del archivo
    resultsFile << "número_experimento;dataset;estructura_de_datos;cantidad_consultas;tiempo_ejecucion\n";

    ifstream file("universities_followers.csv");
    if (!file.is_open())
    {
        cout << "Error al abrir el archivo" << endl;
    }

    string line;
    getline(file, line);
    // Bucle para realizar 20 experimentos
    for (int experimento = 1; experimento <= 20; ++experimento)
    {
        auto start = high_resolution_clock::now();
        for (int i = 0; i < 20000; i++)
        {
            file.clear();  // Limpia el estado del flujo
            file.seekg(0); // Mueve el puntero al inicio del archivo
            getline(file, line);

            if (!getline(file, line))
            {
                cerr << "Error: Se ha alcanzado el final del archivo o la línea está vacía." << endl;
                break; // Salir del bucle si no hay más líneas para leer
            }
            istringstream ss(line);
            string token;
            Datos datos;
            try
            {
                getline(ss, token, ',');
                datos.universidad = token;
                getline(ss, token, ',');
                datos.userId = std::stod(token);
                getline(ss, token, ',');
                datos.userName = token;
                getline(ss, token, ',');
                datos.numTweets = std::stoi(token);
                getline(ss, token, ',');
                datos.numAmigos = std::stoi(token);
                getline(ss, token, ',');
                datos.numSeguidos = std::stoi(token);
                getline(ss, token, ',');
                datos.createdAt = token;
                // ImprimirDatos(datos);
                ht.insertUserId(datos);
                // ht.insertUsername(datos);
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Error: argumento inválido al convertir a número en la línea: " << line << std::endl;
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Error: número fuera de rango en la línea: " << line << std::endl;
            }
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        resultsFile << experimento << ";universities_followers;" << "Hash_abierto;" << ";" << duration.count() << "\n";
    }
    std::cout << "Datos procesados correctamente" << std::endl;

    // ht.displayHashTable();

    // std::string uName = "ntejos";
    // if (ht.searchUserName(uName)) {
    //     std::cout << "El usuario con Name " << uName << " está en la tabla hash." << std::endl;
    // } else {
    //     std::cout << "El usuario con Name " << uName << " no está en la tabla hash." << std::endl;
    // }

    // double uID = 682213121;
    // if (ht.searchUserId(uID)) {
    //     std::cout << "El usuario con ID " << uID << " está en la tabla hash." << std::endl;
    // } else {
    //     std::cout << "El usuario con ID " << uID << " NO está en la tabla hash." << std::endl;
    // }

    resultsFile.close();
    file.close();

    return 0;
}
