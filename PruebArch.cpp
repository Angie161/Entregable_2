#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

using namespace std;

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

int main(int argc, char const *argv[]){

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
