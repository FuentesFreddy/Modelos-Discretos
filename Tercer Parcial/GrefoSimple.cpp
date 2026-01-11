#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int nodos, aristas;
    cout << "Cantidad de Nodos y Aristas: "; cin >> nodos >> aristas;

    // Lista de adyacencia: un arreglo de vectores para guardar vecinos
    vector<int> adyacencia[101]; 
    cout << "Ingresa las " << aristas << " aristas (u v):\n";
    for (int i = 0, u, v; i < aristas; i++) {
        cin >> u >> v;
        adyacencia[u].push_back(v);
        adyacencia[v].push_back(u);
    }

    int origen, destino;
    cout << "Nodo Origen y Destino: "; cin >> origen >> destino;

    // BFS: Busqueda en anchura para encontrar el camino mas corto
    vector<int> distancia(101, -1);
    queue<int> cola;

    distancia[origen] = 0;
    cola.push(origen);

    

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();

        for (int vecino : adyacencia[actual]) {
            if (distancia[vecino] == -1) { // Si el vecino no ha sido visitado
                distancia[vecino] = distancia[actual] + 1;
                cola.push(vecino);
            }
        }
    }

    cout << "Resultado: ";
    if (distancia[destino] == -1) cout << "No existe conexion" << endl;
    else cout << "La distancia es de " << distancia[destino] << " pasos." << endl;

    return 0;
}
