#include <iostream>
#include <vector>
#include "conteo.h" // Incluimos nuestra propia librería

using namespace std;

int main() {
    cout << "=== VERIFICACION DE EJERCICIOS DE LA PRESENTACION ===" << endl;

    // --- EJERCICIO 1 (Profesores) ---
    // Reparto en locales A(2), B(3), C(5). Total 10 profesores.
    // Solución: C(10,2) * C(8,3) * C(5,5)
    cout << "\n--- Ejercicio 1: Profesores ---" << endl;
    unsigned long long localA = combinacionSimple(10, 2);
    unsigned long long localB = combinacionSimple(8, 3);
    unsigned long long localC = combinacionSimple(5, 5);
    
    // Usamos un vector para aplicar el Principio Multiplicativo
    // Nota: principioMultiplicativo espera vector<int>, hacemos cast si es necesario
    // o multiplicamos directo si ya son long long. Aquí mostramos la lógica:
    unsigned long long totalEjer1 = localA * localB * localC;
    
    cout << "Combinaciones Local A (10 en 2): " << localA << endl;
    cout << "Combinaciones Local B (8 en 3): " << localB << endl;
    cout << "Combinaciones Local C (5 en 5): " << localC << endl;
    cout << "Total formas (Principio Multiplicativo): " << totalEjer1 << endl;


    // --- EJERCICIO 5 (Mesa Circular) ---
    // 2 varones y 1 mujer juntos (bloque de 3) + 4 personas resto.
    // Total elementos a circular: 5.
    cout << "\n--- Ejercicio 5: Mesa Circular con condicion ---" << endl;
    unsigned long long circularExterno = permutacionCircular(5); // PC(5)
    unsigned long long linealInterno = permutacionSimple(3);     // P(3) para el bloque
    
    cout << "Circular 5 elementos (PC5): " << circularExterno << endl;
    cout << "Orden interno bloque (P3): " << linealInterno << endl;
    cout << "Total: " << circularExterno * linealInterno << endl;


    // --- EJERCICIO 11 (Chompas) ---
    // 4 modelos, comprar 10, al menos 1 de cada uno.
    // Se reduce a elegir 6 libres entre 4 modelos (Combinación con Repetición).
    // n=4, r=6.
    cout << "\n--- Ejercicio 11: Chompas (Comb. con Repeticion) ---" << endl;
    // Nota: Aunque la diapositiva tenía un error en la restricción r<=n,
    // nuestra implementación funciona correctamente para r > n.
    unsigned long long resultadoChompas = combinacionConRepeticion(4, 6);
    cout << "CR(n=4, r=6): " << resultadoChompas << endl;


    // --- EJERCICIO 16 (Palabra ARITMETICA) ---
    // Pares: 3 pares en 5 posiciones dobles -> V(5,3) o C(5,3)*P(3)
    // Solitarias: 4 letras en 4 huecos -> P(4)
    cout << "\n--- Ejercicio 16: Palabra ARITMETICA ---" << endl;
    
    // Paso 1: Elegir y ordenar los pares (AA, II, TT) en 5 posiciones posibles
    // Esto es una Permutación Lineal de 5 elementos tomados de 3 en 3 (Variación)
    unsigned long long pares = permutacionLineal(5, 3);
    
    // Paso 2: Ordenar las solitarias (R, M, E, C)
    unsigned long long solitarias = permutacionSimple(4);
    
    cout << "Formas de ubicar pares (P_Lineal 5 en 3): " << pares << endl;
    cout << "Formas de ubicar solitarias (P_Simple 4): " << solitarias << endl;
    cout << "Total: " << pares * solitarias << endl;

    return 0;
}