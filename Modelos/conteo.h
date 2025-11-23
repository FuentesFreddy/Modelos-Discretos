/**
 * conteo.h
 * Librería basada en las diapositivas de "Principios Fundamentales de Conteo".
 */

#ifndef CONTEO_H
#define CONTEO_H

// --- CORRECCIÓN IMPORTANTE: Incluir vector aquí ---
#include <vector>
#include <iostream>
// --------------------------------------------------

// Función auxiliar para calcular el Factorial (n!)
// Necesario para las fórmulas de Permutaciones y Combinaciones.
unsigned long long factorial(int n);

// --- PRINCIPIOS FUNDAMENTALES ---

// Principio de Multiplicación (Regla del Producto)
// Fuente: Diapositiva 6 - "Si un evento A ocurre de m formas... entonces m*n"
unsigned long long principioMultiplicativo(const std::vector<int>& eventos);

// Principio de Adición (Regla de la Suma)
// Fuente: Diapositiva 8 - "Si un evento A... o el evento B... m+n formas"
unsigned long long principioAditivo(const std::vector<int>& eventos);


// --- PERMUTACIONES (Importa el orden) ---

// Teorema 1: Permutación Lineal (Variación) de n elementos tomados de r en r.
// Fuente: Diapositiva 15
// Formula: n! / (n-r)!
unsigned long long permutacionLineal(int n, int r);

// Corolario: Permutación de n elementos (Todos los elementos)
// Fuente: Diapositiva 15
// Formula: Pn = n!
unsigned long long permutacionSimple(int n);

// Teorema 2: Permutación Circular
// Fuente: Diapositiva 17
// Formula: Pc = (n-1)!
unsigned long long permutacionCircular(int n);

// Teorema 3: Permutación con Elementos Repetidos
// Fuente: Diapositiva 18
// Formula: n! / (n1! * n2! * ... * nk!)
unsigned long long permutacionConRepeticion(int n, const std::vector<int>& repetidos);


// --- COMBINACIONES (No importa el orden) ---

// Teorema 4: Combinación Simple (Sin repetición)
// Fuente: Diapositiva 22
// Formula: n! / ((n-r)! * r!)
unsigned long long combinacionSimple(int n, int r);

// Teorema 5: Combinación con Repetición
// Fuente: Diapositiva 23
// Formula: (n + r - 1)! / ((n - 1)! * r!)
unsigned long long combinacionConRepeticion(int n, int r);

#endif