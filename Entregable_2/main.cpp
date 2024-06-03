#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
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

class HashTable
{
private:
    vector<list<Datos>> table;
    int tableSize;

    int hashUserName(const std::string &userName)
    {
        unsigned int aux = 2166136261;
        unsigned int aux2 = 16777619;
        unsigned int hash = aux;
        for (int i = 0; i < userName.length(); ++i)
        {
            hash ^= userName[i];
            hash *= aux2;
        }
        return hash % tableSize;
    }

    int hashUserId(double userId)
    {
        long long intId = static_cast<long long>(userId);
        intId = ((intId >> 16) ^ intId) * 0x45d9f3b;
        intId = ((intId >> 16) ^ intId) * 0x45d9f3b;
        intId = (intId >> 16) ^ intId;
        return intId % tableSize;
    }

public:
    HashTable(int size) : tableSize(size)
    {
        table.resize(tableSize);
    }
    void insertUserId(const Datos &data)
    {
        int index = hashUserId(data.userId);
        table[index].push_back(data);
    }
    void insertUsername(const Datos &data)
    {
        int index = hashUserName(data.userName);
        table[index].push_back(data);
    }

    // SEARCH USER ID
    bool searchUserId(double userId)
    {
        int index = hashUserId(userId);
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
        int index = hashUserName(userName);
        for (const auto &data : table[index])
        {
            if (data.userName == userName)
            {
                return true;
            }
        }
        return false;
    }
};

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <cantidad_elementos>" << endl;
        exit(1);
    }
    int cantElem = atoi(argv[1]);

    HashTable ht_ID(27071);
    HashTable ht_Name(27071);

    ifstream file("universities_followers.csv");
    if (!file.is_open())
    {
        cerr << "Error al abrir el archivo" << endl;
        return 1;
    }

    string line;
    getline(file, line);

    for (int experimento = 1; experimento <= 20; ++experimento)
    {
        file.clear();  // Limpia el estado del flujo
        file.seekg(0); // Mueve el puntero al inicio del archivo
        getline(file, line);
        if (!getline(file, line))
        {
            cerr << "Error: Se ha alcanzado el final del archivo o la línea está vacía." << endl;
            break; // Salir del bucle si no hay más líneas para leer
        }
        for (int i = 0; i < cantElem; i++)
        {
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
                // Medir el tiempo de inserción por ID
                auto startId = high_resolution_clock::now();
                ht_ID.insertUserId(datos);
                auto endId = high_resolution_clock::now();
                auto durationId = duration_cast<nanoseconds>(endId - startId);
                cout << experimento << ";universities_followers;Hash_abierto;ID;" << cantElem << ";" << durationId.count() << endl;

                // Medir el tiempo de inserción por Nombre
                auto startName = high_resolution_clock::now();
                ht_Name.insertUsername(datos);
                auto endName = high_resolution_clock::now();
                auto durationName = duration_cast<nanoseconds>(endName - startName);
                cout << experimento << ";universities_followers;Hash_abierto;Name;" << cantElem << ";" << durationName.count() << endl;
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
    }
    file.close();
    return 0;
}
