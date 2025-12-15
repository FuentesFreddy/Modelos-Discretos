#ifndef SISTEMA_H
#define SISTEMA_H

#include <iostream>
#include <list>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

// --- ESTRUCTURAS ---

struct Proyecto {
    string nombre;
    int diasRetraso;
    int errores;
    bool revisionUrgente;
    bool completado;
};

struct Nodo {
    Proyecto data;  // Información del proyecto
    Nodo* next;     // Puntero al siguiente nodo
};

// --- ENUMS ---

enum EstadoProduccion {
    NORMAL,
    BAJO_RENDIMIENTO,
    FALLA_MECANICA,
    OBSERVACION,
    CRITICO
};

// --- PROTOTIPOS DE FUNCIONES LÓGICAS ---
bool P(Proyecto p);
bool Q(Proyecto p);
bool R(Proyecto p);
EstadoProduccion estadoProyecto(const Proyecto& p);
long long comb(int n, int k);

// --- CLASES ---

class ListaEnlazada {
public:
    Nodo* head; 

    ListaEnlazada(); // Constructor
    void insertar(Proyecto p);
    bool contiene(string nombre);
    void mostrar(string titulo);

    // Operaciones de conjuntos
    ListaEnlazada interseccion(ListaEnlazada B);
    ListaEnlazada unionConjuntos(ListaEnlazada B);
    ListaEnlazada diferencia(ListaEnlazada B);
};

class SistemaRiesgo {
public:
    ListaEnlazada activos;      // A
    ListaEnlazada retraso;      // B
    ListaEnlazada completados;  // C

    void agregarProyecto();
    void evaluarProyectos();
    void mostrarConjuntos();
    void resumenFinal();
    
    // Funciones del Proyecto 4
    void probabilidadesEstados();
    void analizarCombinacionesRiesgo();
    void conteoEstadosProduccion();
    
    // Gestión de archivos
    void cargarProyectos();
};

#endif
