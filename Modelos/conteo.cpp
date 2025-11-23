#include "conteo.h"

// Implementación del Factorial
unsigned long long factorial(int n) {
    if (n < 0) return 0; // Manejo de error para negativos
    if (n == 0 || n == 1) return 1;
    unsigned long long resultado = 1;
    for (int i = 2; i <= n; ++i) {
        resultado *= i;
    }
    return resultado;
}

// Principio Multiplicativo [cite: 89]
unsigned long long principioMultiplicativo(const std::vector<int>& eventos) {
    unsigned long long total = 1;
    for (int e : eventos) {
        total *= e;
    }
    return total;
}

// Principio Aditivo [cite: 106]
unsigned long long principioAditivo(const std::vector<int>& eventos) {
    unsigned long long total = 0;
    for (int e : eventos) {
        total += e;
    }
    return total;
}

// Teorema 1: Permutación Lineal (nPr) [cite: 194]
unsigned long long permutacionLineal(int n, int r) {
    if (r > n) return 0; // No se pueden tomar más elementos de los que hay
    // Formula: n! / (n-r)!
    return factorial(n) / factorial(n - r);
}

// Corolario: Permutación Simple (Pn) [cite: 195]
unsigned long long permutacionSimple(int n) {
    // Formula: n!
    return factorial(n);
}

// Teorema 2: Permutación Circular [cite: 214]
unsigned long long permutacionCircular(int n) {
    if (n <= 0) return 0;
    // Formula: (n-1)!
    return factorial(n - 1);
}

// Teorema 3: Permutación con Repetición [cite: 224]
unsigned long long permutacionConRepeticion(int n, const std::vector<int>& repetidos) {
    unsigned long long numerador = factorial(n);
    unsigned long long denominador = 1;
    
    int suma_elementos = 0;
    for (int k : repetidos) {
        denominador *= factorial(k);
        suma_elementos += k;
    }
    
    // Validación: La suma de los subgrupos no puede exceder el total n
    if (suma_elementos > n) return 0; 

    return numerador / denominador;
}

// Teorema 4: Combinación Simple (nCr) [cite: 303]
unsigned long long combinacionSimple(int n, int r) {
    if (r > n) return 0;
    // Formula: n! / ((n-r)! * r!)
    return factorial(n) / (factorial(n - r) * factorial(r));
}

// Teorema 5: Combinación con Repetición [cite: 313]
unsigned long long combinacionConRepeticion(int n, int r) {
    // Formula original: (n + r - 1)! / ((n - 1)! * r!)
    // Esto es matemáticamente equivalente a una combinación simple de (n+r-1) en r
    int n_nuevo = n + r - 1;
    return combinacionSimple(n_nuevo, r);
}