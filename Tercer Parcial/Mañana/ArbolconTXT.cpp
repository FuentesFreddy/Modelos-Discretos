#include <iostream>
#include <vector>
#include <algorithm> 
#include <fstream> // Librería para manejo de archivos

using namespace std;

struct Nodo {
    int dato;
    Nodo* izq;
    Nodo* dere;
    Nodo* padre;
};

// --- PROTOTIPOS ---
Nodo* crearNodo(int, Nodo*);
void insertar(Nodo*&, int, Nodo*);
void mostrar(Nodo*, int);
bool buscar(Nodo*, int);
void preOrden(Nodo*);
void inOrden(Nodo*);
void posOrden(Nodo*);
Nodo* minimo(Nodo*);
void destruirNodo(Nodo*);
void eliminarNodo(Nodo*);
void eliminar(Nodo*&, int);
void guardarEnVector(Nodo*, vector<int>&);
Nodo* construirBalanceado(vector<int>&, int, int);
Nodo* balancearArbol(Nodo*);

// Funciones basadas en el PDF
int obtenerAltura(Nodo*);
void clasificarNodos(Nodo*, Nodo*);

// --- NUEVAS FUNCIONES PARA ARCHIVOS ---
void leerDesdeArchivo(Nodo*&, string);
void escribirEnArchivo(Nodo*, ofstream&); // Auxiliar recursiva
void guardarEnArchivo(Nodo*, string);

int main() {
    Nodo* arbol = nullptr;
    int opcion;
    int dato;
    int contador = 0;
    int n;
    string nombreArchivo;

    do {
        cout << "\n--- MENU ARBOL BINARIO ---" << endl;
        cout << "1. Ingresar datos al arbol\n";
        cout << "2. Mostrar datos al arbol\n";
        cout << "3. Buscar elemento en el arbol\n";
        cout << "4. Imprimir datos en pre orden\n";
        cout << "5. Imprimir datos en in orden\n";
        cout << "6. Imprimir datos en pos orden\n";
        cout << "7. Eliminar nodo del arbol \n";
        cout << "8. Balancear arbol\n";
        cout << "10. Ver altura del arbol\n";
        cout << "11. Clasificar nodos (Hoja/Interno)\n";
        cout << "12. Leer datos desde archivo .txt\n";
        cout << "13. Guardar arbol en archivo .txt\n"; // NUEVA OPCIÓN
        cout << "9. Salir\n";
        cout << "Seleccione: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            int parada;
            do {
                cout << "Ingresar un dato: ";
                cin >> dato;
                insertar(arbol, dato, nullptr);
                cout << "Ingrese 1 para seguir ingresando 0 para salir: ";
                cin >> parada;
            } while (parada != 0);
            break;
        case 2:
            mostrar(arbol, contador);
            break;
        case 3:
            cout << "Ingrese el numero a buscar: ";
            cin >> n;
            if (buscar(arbol, n) == true) cout << "Elemento encontrado\n";
            else cout << "No encontrado\n";
            break;
        case 4: preOrden(arbol); break;
        case 5: inOrden(arbol); break;
        case 6: posOrden(arbol); break;
        case 7:
            int numero;
            cout << "Digite el numero a eliminar: ";
            cin >> numero;
            eliminar(arbol, numero);
            break;
        case 8:
            arbol = balancearArbol(arbol); 
            cout << "Arbol balanceado.\n";
            break;
        case 10:
            cout << "La altura del arbol es: " << obtenerAltura(arbol) << endl;
            break;
        case 11:
            cout << "\n--- CLASIFICACION DE NODOS ---" << endl;
            clasificarNodos(arbol, arbol);
            break;
        case 12:
            cout << "Ingrese el nombre del archivo para LEER (ej: datos.txt): ";
            cin >> nombreArchivo;
            leerDesdeArchivo(arbol, nombreArchivo);
            break;
        case 13:
            cout << "Ingrese el nombre del archivo para GUARDAR (ej: respaldo.txt): ";
            cin >> nombreArchivo;
            guardarEnArchivo(arbol, nombreArchivo);
            break;
        case 9:
            return 0;
        default:
            cout << "Opcion no valida.\n";
            break;
        }
    } while (opcion != 9);
    return 0;
}

// --- IMPLEMENTACIÓN DE FUNCIONES ORIGINALES ---

Nodo* crearNodo(int n, Nodo* padre) {
    Nodo* nuevo = new Nodo();
    nuevo->dato = n;
    nuevo->dere = nullptr;
    nuevo->izq = nullptr;
    nuevo->padre = padre;
    return nuevo;
}

void insertar(Nodo*& arbol, int n, Nodo* padre) {
    if (arbol == nullptr) {
        Nodo* nuevo = crearNodo(n, padre);
        arbol = nuevo;
    } else {
        int valorRaiz = arbol->dato;
        if (n < valorRaiz) insertar(arbol->izq, n, arbol);
        else insertar(arbol->dere, n, arbol);
    }
}

void mostrar(Nodo* arbol, int contador) {
    if (arbol == nullptr) return;
    mostrar(arbol->dere, contador + 1);
    for (int i = 0; i < contador; i++) cout << "    ";
    cout << arbol->dato << endl;
    mostrar(arbol->izq, contador + 1);
}

bool buscar(Nodo* arbol, int n) {
    if (arbol == nullptr) return false;
    else if (arbol->dato == n) return true;
    else if (n < arbol->dato) return buscar(arbol->izq, n);
    else return buscar(arbol->dere, n);
}

void preOrden(Nodo* arbol) {
    if (arbol == nullptr) return;
    cout << arbol->dato << "->";
    preOrden(arbol->izq);
    preOrden(arbol->dere);
}

void inOrden(Nodo* arbol) {
    if (arbol == nullptr) return;
    inOrden(arbol->izq);
    cout << arbol->dato << "->";
    inOrden(arbol->dere);
}

void posOrden(Nodo* arbol) {
    if (arbol == nullptr) return;
    posOrden(arbol->izq);
    posOrden(arbol->dere);
    cout << arbol->dato << "->";
}

Nodo* minimo(Nodo* arbol) {
    if (arbol == nullptr) return nullptr;
    else if (arbol->izq) return minimo(arbol->izq);
    else return arbol;
}

void remplazar(Nodo* arbol, Nodo* nuevoNodo) {
    if (arbol->padre) {
        if (arbol->dato == arbol->padre->izq->dato) arbol->padre->izq = nuevoNodo;
        else if (arbol->dato == arbol->padre->dere->dato) arbol->padre->dere = nuevoNodo;
    }
    if (nuevoNodo) nuevoNodo->padre = arbol->padre;
}

void destruirNodo(Nodo* nodo) {
    nodo->izq = nullptr;
    nodo->dere = nullptr;
    delete nodo;
}

void eliminarNodo(Nodo* nodoEliminar) {
    if (nodoEliminar->izq && nodoEliminar->dere) {
        Nodo* menor = minimo(nodoEliminar->dere);
        nodoEliminar->dato = menor->dato;
        eliminarNodo(menor);
    } else if (nodoEliminar->izq) {
        remplazar(nodoEliminar, nodoEliminar->izq);
        destruirNodo(nodoEliminar);
    } else if (nodoEliminar->dere) {
        remplazar(nodoEliminar, nodoEliminar->dere);
        destruirNodo(nodoEliminar);
    } else {
        remplazar(nodoEliminar, nullptr);
        destruirNodo(nodoEliminar);
    }
}

void eliminar(Nodo*& arbol, int n) {
    if (arbol == nullptr) return;
    else if (n < arbol->dato) eliminar(arbol->izq, n);
    else if (n > arbol->dato) eliminar(arbol->dere, n);
    else eliminarNodo(arbol);
}

void guardarEnVector(Nodo* raiz, vector<int>& nodos) {
    if (raiz == nullptr) return;
    guardarEnVector(raiz->izq, nodos);
    nodos.push_back(raiz->dato);
    guardarEnVector(raiz->dere, nodos);
}

Nodo* construirBalanceado(vector<int>& nodos, int inicio, int fin) {
    if (inicio > fin) return nullptr;
    int medio = (inicio + fin) / 2;
    Nodo* nuevo = crearNodo(nodos[medio], nullptr);
    nuevo->izq = construirBalanceado(nodos, inicio, medio - 1);
    nuevo->dere = construirBalanceado(nodos, medio + 1, fin);
    return nuevo;
}

Nodo* balancearArbol(Nodo* raiz) {
    vector<int> nodos;
    guardarEnVector(raiz, nodos);
    int n = nodos.size();
    return construirBalanceado(nodos, 0, n - 1);
}

int obtenerAltura(Nodo* arbol) {
    if (arbol == nullptr) return -1; 
    else {
        int altIzq = obtenerAltura(arbol->izq);
        int altDere = obtenerAltura(arbol->dere);
        return 1 + max(altIzq, altDere);
    }
}

void clasificarNodos(Nodo* arbol, Nodo* raizOriginal) {
    if (arbol == nullptr) return;

    cout << "Nodo [" << arbol->dato << "]: ";
    if (arbol == raizOriginal) cout << "RAIZ";
    else if (arbol->izq == nullptr && arbol->dere == nullptr) cout << "HOJA";
    else cout << "NODO INTERNO";
    cout << endl;

    clasificarNodos(arbol->izq, raizOriginal);
    clasificarNodos(arbol->dere, raizOriginal);
}

// --- IMPLEMENTACIÓN DE LAS FUNCIONES DE ARCHIVO ---

void leerDesdeArchivo(Nodo*& arbol, string nombreArchivo) {
    ifstream archivo(nombreArchivo);
    int valor;
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo." << endl;
        return;
    }
    while (archivo >> valor) {
        insertar(arbol, valor, nullptr);
    }
    archivo.close();
    cout << "Datos cargados correctamente." << endl;
}

void escribirEnArchivo(Nodo* arbol, ofstream& archivo) {
    if (arbol == nullptr) return;
    escribirEnArchivo(arbol->izq, archivo); // In-Orden
    archivo << arbol->dato << " ";
    escribirEnArchivo(arbol->dere, archivo);
}

void guardarEnArchivo(Nodo* arbol, string nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo crear el archivo." << endl;
        return;
    }
    escribirEnArchivo(arbol, archivo);
    archivo.close();
    cout << "Arbol guardado exitosamente en " << nombreArchivo << endl;
}
