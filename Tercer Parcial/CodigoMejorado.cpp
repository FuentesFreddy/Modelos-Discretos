#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Definimos un tipo para facilitar la lectura de las matrices
typedef vector<vector<int>> Matriz;

// --- FUNCIONES DE REPRESENTACIÓN ---

void mostrarAdyacencia(int n, const Matriz& ady) {
    cout << "\nMATRIZ DE ADYACENCIA:\n";
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) cout << ady[i][j] << " ";
        cout << endl;
    }
}

void mostrarIncidencia(int n, int e, const Matriz& inc) {
    cout << "\nMATRIZ DE INCIDENCIA:\n";
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < e; j++) cout << inc[i][j] << " ";
        cout << endl;
    }
}

// --- FUNCIONES SOLICITADAS (Basadas en PDFs T1 y T2) ---

// Calcula el grado de cada vértice (Teoría T1: Definiciones)

void calcularGrados(int n, const Matriz& ady) {
    cout << "\nGRADOS DE LOS VERTICES:\n";
    for (int i = 1; i <= n; i++) {
        int grado = 0;
        for (int j = 1; j <= n; j++) {
            grado += ady[i][j];
        }
        cout << "Grado(v" << i << ") = " << grado << endl;
    }
}

// Determina si el grafo es bipartido usando coloreo (Teoría T2)

bool esBipartito(int n, const Matriz& ady) {
    vector<int> color(n + 1, -1); // -1 significa sin color

    for (int i = 1; i <= n; i++) {
        if (color[i] == -1) {
            queue<int> q;
            q.push(i);
            color[i] = 1; // Primer color

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int v = 1; v <= n; v++) {
                    if (ady[u][v] == 1) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u]; // Color opuesto
                            q.push(v);
                        } else if (color[v] == color[u]) {
                            return false; // Mismo color en nodos adyacentes
                        }
                    }
                }
            }
        }
    }
    return true;
}

// --- FUNCIÓN PRINCIPAL ---

int main() {
    int n, e;
    cout << "Numero de nodos y aristas: ";
    cin >> n >> e;

    // Inicialización de matrices dinámicas
    Matriz ady(n + 1, vector<int>(n + 1, 0));
    Matriz inc(n + 1, vector<int>(e, 0));

    cout << "Ingresa los pares de la lista de aristas (u v):\n";
    for (int i = 0; i < e; i++) {
        int u, v;
        cin >> u >> v;

        // Llenado de Adyacencia
        ady[u][v] = 1;
        ady[v][u] = 1;

        // Llenado de Incidencia
        inc[u][i] = 1;
        inc[v][i] = 1;
    }

    // Ejecución de funciones
    mostrarAdyacencia(n, ady);
    mostrarIncidencia(n, e, inc);
    calcularGrados(n, ady);

    if (esBipartito(n, ady)) {
        cout << "\nRESULTADO: El grafo es Bipartido.\n";
    } else {
        cout << "\nRESULTADO: El grafo NO es Bipartido (tiene ciclos impares).\n";
    }

    return 0;
}
