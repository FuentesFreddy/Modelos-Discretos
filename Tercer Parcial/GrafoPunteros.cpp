#include <iostream>
using namespace std;

// Estructura para representar una conexión (Nodo de lista enlazada)
struct Arista {
    int destino;
    Arista* siguiente;
};

// Variables globales
int totalNodos, totalAristas;
Arista* listaAdyacencia[100]; // Arreglo de punteros (cabezas de lista)
int rutaHamiltoniana[100];
int nodoVisitado[100];

// Función para añadir una arista a la lista (Insertar al inicio)
void agregarArista(int u, int v) {
    Arista* nueva = new Arista();
    nueva->destino = v;
    nueva->siguiente = listaAdyacencia[u];
    listaAdyacencia[u] = nueva;
}

// Función auxiliar para verificar si existe una conexión directa entre dos nodos
bool existeConexion(int u, int v) {
    Arista* temporal = listaAdyacencia[u];
    while (temporal != NULL) {
        if (temporal->destino == v) return true;
        temporal = temporal->siguiente;
    }
    return false;
}

// --- CICLO HAMILTONIANO (Backtracking) ---
bool encontrarCicloHamiltoniano(int posicionActual) {
    if (posicionActual == totalNodos) {
        return existeConexion(rutaHamiltoniana[posicionActual - 1], rutaHamiltoniana[0]);
    }

    for (int vecino = 1; vecino <= totalNodos; vecino++) {
        if (existeConexion(rutaHamiltoniana[posicionActual - 1], vecino) && !nodoVisitado[vecino]) {
            nodoVisitado[vecino] = 1;
            rutaHamiltoniana[posicionActual] = vecino;
            if (encontrarCicloHamiltoniano(posicionActual + 1)) return true;
            nodoVisitado[vecino] = 0;
        }
    }
    return false;
}

int main() {
    cout << "Ingrese Numero de Nodos y Aristas: ";
    cin >> totalNodos >> totalAristas;

    // Inicializar listas en NULL
    for (int i = 1; i <= totalNodos; i++) listaAdyacencia[i] = NULL;

    cout << "Ingrese las aristas (u v):\n";
    for (int i = 0, u, v; i < totalAristas; i++) {
        cin >> u >> v;
        agregarArista(u, v);
        agregarArista(v, u); // Grafo no dirigido
    }

    // --- 1. CAMINO MÁS CORTO (BFS) ---
    int origen, destino;
    int colaBFS[100], distancia[100], frente = 0, final = 0;
    cout << "\nOrigen y Destino para distancia: "; cin >> origen >> destino;

    for (int i = 1; i <= totalNodos; i++) distancia[i] = -1;
    colaBFS[final++] = origen;
    distancia[origen] = 0;

    while (frente < final) {
        int u = colaBFS[frente++];
        for (Arista* temp = listaAdyacencia[u]; temp != NULL; temp = temp->siguiente) {
            if (distancia[temp->destino] == -1) {
                distancia[temp->destino] = distancia[u] + 1;
                colaBFS[final++] = temp->destino;
            }
        }
    }
    cout << "Distancia mas corta: " << distancia[destino] << endl;

    // --- 2. CICLO EULERIANO (Grados pares) ---
    bool esEuleriano = true;
    for (int i = 1; i <= totalNodos; i++) {
        int grado = 0;
        for (Arista* temp = listaAdyacencia[i]; temp != NULL; temp = temp->siguiente) grado++;
        if (grado % 2 != 0 || grado == 0) esEuleriano = false;
    }
    cout << (esEuleriano ? "Tiene Ciclo Euleriano" : "No tiene Ciclo Euleriano") << endl;

    // --- 3. CICLO HAMILTONIANO ---
    rutaHamiltoniana[0] = 1;
    nodoVisitado[1] = 1;
    cout << (encontrarCicloHamiltoniano(1) ? "Tiene Ciclo Hamiltoniano" : "No tiene Ciclo Hamiltoniano") << endl;

    return 0;
}
