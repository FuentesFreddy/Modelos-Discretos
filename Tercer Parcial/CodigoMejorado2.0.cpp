#include <iostream>
#include <vector>
#include <queue>
#include <numeric>

using namespace std;

const int INF = 1e9; // Definimos un valor infinito para distancias

// Definimos un tipo para facilitar la lectura de las matrices
typedef vector<vector<int>> Matriz;

// --- FUNCIONES EXISTENTES ---

void mostrarAdyacencia(int n, const Matriz& ady) {
    cout << "\nMATRIZ DE ADYACENCIA (VALORES = PESOS):\n";
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

void calcularGrados(int n, const Matriz& ady) {
    cout << "\nGRADOS DE LOS VERTICES:\n";
    for (int i = 1; i <= n; i++) {
        int grado = 0;
        for (int j = 1; j <= n; j++) {
            if (ady[i][j] > 0) grado++;
        }
        cout << "Grado(v" << i << ") = " << grado << endl;
    }
}

bool esBipartito(int n, const Matriz& ady) {
    vector<int> color(n + 1, -1); 
    for (int i = 1; i <= n; i++) {
        if (color[i] == -1) {
            queue<int> q;
            q.push(i);
            color[i] = 1; 
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v = 1; v <= n; v++) {
                    if (ady[u][v] > 0) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u];
                            q.push(v);
                        } else if (color[v] == color[u]) return false;
                    }
                }
            }
        }
    }
    return true;
}

void DFS(int v, vector<bool>& visitado, int n, const Matriz& ady) {
    visitado[v] = true;
    for (int i = 1; i <= n; i++) {
        if (ady[v][i] > 0 && !visitado[i]) DFS(i, visitado, n, ady);
    }
}

bool tieneCaminoEuleriano(int n, const Matriz& ady) {
    int impares = 0, nodosConAristas = 0, inicio = -1;
    for (int i = 1; i <= n; i++) {
        int grado = 0;
        for (int j = 1; j <= n; j++) if (ady[i][j] > 0) grado++;
        if (grado > 0) {
            nodosConAristas++;
            if (inicio == -1) inicio = i;
            if (grado % 2 != 0) impares++;
        }
    }
    if (nodosConAristas == 0) return true;
    vector<bool> visitado(n + 1, false);
    DFS(inicio, visitado, n, ady);
    for (int i = 1; i <= n; i++) {
        int g = 0;
        for (int j = 1; j <= n; j++) if (ady[i][j] > 0) g++;
        if (g > 0 && !visitado[i]) return false;
    }
    return (impares == 0 || impares == 2);
}

bool resolverHamiltoniano(int n, const Matriz& ady, vector<int>& camino, int pos) {
    if (pos == n) return (ady[camino[pos - 1]][camino[0]] > 0);
    for (int v = 2; v <= n; v++) {
        bool yaEnCamino = false;
        for (int i = 0; i < pos; i++) if (camino[i] == v) { yaEnCamino = true; break; }
        if (!yaEnCamino && ady[camino[pos - 1]][v] > 0) {
            camino[pos] = v;
            if (resolverHamiltoniano(n, ady, camino, pos + 1)) return true;
            camino[pos] = -1;
        }
    }
    return false;
}

bool tieneCicloHamiltoniano(int n, const Matriz& ady) {
    vector<int> camino(n, -1);
    camino[0] = 1; 
    return resolverHamiltoniano(n, ady, camino, 1);
}

// --- NUEVAS FUNCIONES PARA GRAFOS PONDERADOS ---

/* Función para poner ponderaciones:
   Sustituye el valor '1' en la matriz de adyacencia por un peso real.
   Refleja lo visto en la Pág. 34 de la T1 sobre "Grafo Ponderado".
*/
void ingresarPonderaciones(int n, Matriz& ady) {
    cout << "\n--- ASIGNACION DE PESOS ---" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            if (ady[i][j] > 0) {
                int peso;
                cout << "Ingrese el peso para la arista (" << i << " - " << j << "): ";
                cin >> peso;
                ady[i][j] = peso;
                ady[j][i] = peso;
            }
        }
    }
}

/* Función para encontrar la distancia mínima (Dijkstra):
   Calcula el costo total del camino más corto entre dos nodos.
   Basado en el problema de "Camino más corto" de la Pág. 2 de la T1.
*/

void encontrarDistancia(int n, const Matriz& ady) {
    int inicio, fin;
    cout << "\nNodo origen: "; cin >> inicio;
    cout << "Nodo destino: "; cin >> fin;

    if (inicio < 1 || inicio > n || fin < 1 || fin > n) {
        cout << "Nodos fuera de rango." << endl; return;
    }

    vector<int> dist(n + 1, INF);
    dist[inicio] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, inicio});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (int v = 1; v <= n; v++) {
            if (ady[u][v] > 0) {
                if (dist[u] + ady[u][v] < dist[v]) {
                    dist[v] = dist[u] + ady[u][v];
                    pq.push({dist[v], v});
                }
            }
        }
    }

    if (dist[fin] == INF) cout << "No existe camino entre " << inicio << " y " << fin << endl;
    else cout << "La distancia minima (costo total) es: " << dist[fin] << endl;
}

// --- MAIN CON MENU ACTUALIZADO ---

int main() {
    int n, e;
    cout << "Numero de nodos y aristas: ";
    cin >> n >> e;

    Matriz ady(n + 1, vector<int>(n + 1, 0));
    Matriz inc(n + 1, vector<int>(e, 0));

    cout << "Ingresa los pares (u v):\n";
    for (int i = 0; i < e; i++) {
        int u, v; cin >> u >> v;
        ady[u][v] = 1; ady[v][u] = 1;
        inc[u][i] = 1; inc[v][i] = 1;
    }

    int opcion;
    do {
        cout << "\n--- MENU ---" << endl;
        cout << "1. Ver Matrices\n2. Grados\n3. Bipartido\n4. Euleriano\n5. Hamiltoniano";
        cout << "\n6. Asignar Pesos (Ponderar)\n7. Calcular Distancia Minima\n0. Salir\nOpcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: mostrarAdyacencia(n, ady); mostrarIncidencia(n, e, inc); break;
            case 2: calcularGrados(n, ady); break;
            case 3: cout << (esBipartito(n, ady) ? "Es Bipartido" : "No es Bipartido") << endl; break;
            case 4: cout << (tieneCaminoEuleriano(n, ady) ? "Tiene Camino Euleriano" : "No tiene") << endl; break;
            case 5: cout << (tieneCicloHamiltoniano(n, ady) ? "Tiene Ciclo Hamiltoniano" : "No tiene") << endl; break;
            case 6: ingresarPonderaciones(n, ady); break;
            case 7: encontrarDistancia(n, ady); break;
        }
    } while (opcion != 0);

    return 0;
}
