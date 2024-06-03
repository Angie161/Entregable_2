#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <math.h>
#include <cmath>
#include <chrono>
using namespace std;

#ifndef funcionesHash_H
#define funcionesHash_H


//Función hash para el User ID, entrega un entero positivo.
// k: clave Id.
// n: tamaño de la tabla hash.
unsigned int hashId(double key, int n) 
{
	long long intId = static_cast<long long>(key);
	return intId % n;
}

// Función hash auxiliar para el userId, emplea el método de la multiplicación para obtener el indice no negativo de guardado para la tabla hash.
// k: clave Id.
// n: tamaño de la tabla hash.
const float A = (sqrt(5) - 1) / 2;
unsigned int hAuxId(double key, int n)
{
    float a = (float)key * A;
    a -= (int)a;

    return n * a;
}

//Función hash para el User Name, entrega un entero positivo.
// key: clave Name.
// n: tamaño de la tabla hash.
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

//Función hash auxiliar para el userName, permite adaptar el tamaño del int obtenido con la función murmurhash3_32 al tamaño de la tabla.
//key: Clave string.
//n: Tamño de la tabla hash.
unsigned int hAuxName(const std::string &key, int n) {
    const uint8_t* keyPtr = reinterpret_cast<const uint8_t*>(key.c_str());
    size_t len = key.length();
    uint32_t seed = 0x9747b28c; // Valor de semilla aleatorio
    return murmurhash3_32(keyPtr, len, seed) % n;
}

#endif