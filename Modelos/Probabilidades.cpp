#include <iostream>
#include <vector>
#include <cmath> // Para abs() en comparaciones de float
#include <numeric> // Para algoritmos numéricos

using namespace std;

// 1. Probabilidad Empírica / Frecuentista 
// P(S) = n / N (límite cuando N tiende a infinito)
double probabilidadEmpirica(double frecuenciaAbsoluta, double totalObservaciones) {
    if (totalObservaciones == 0) return 0.0;
    return frecuenciaAbsoluta / totalObservaciones;
}

// 2. Regla de Laplace (Probabilidad Clásica) 
// P(S) = Casos Favorables / Casos Posibles
double reglaLaplace(double casosFavorables, double casosPosibles) {
    if (casosPosibles == 0) return 0.0;
    return casosFavorables / casosPosibles;
}

// 3. Regla del Complemento 
// P(E) + P(No E) = 1  ->  P(E) = 1 - P(No E)
double probabilidadComplemento(double probEventoOpuesto) {
    return 1.0 - probEventoOpuesto;
}

// 4. Regla de la Suma (Unión de Eventos) 
// P(A U B) = P(A) + P(B) - P(A n B)
// Nota: Si son mutuamente excluyentes, la intersección es 0 
double probabilidadUnion(double pA, double pB, double pInterseccion) {
    return pA + pB - pInterseccion;
}

// 5. Probabilidad Condicional 
// P(E|F) = P(E n F) / P(F)
double probabilidadCondicional(double pInterseccion, double pCondicion) {
    if (pCondicion == 0) {
        cerr << "Error: La probabilidad de la condición no puede ser 0." << endl;
        return 0.0;
    }
    return pInterseccion / pCondicion;
}

// 6. Verificación de Independencia 
// Dos eventos son independientes si P(A n B) == P(A) * P(B)
bool sonIndependientes(double pA, double pB, double pInterseccion) {
    double producto = pA * pB;
    // Usamos un margen de error (epsilon) pequeño por ser punto flotante
    return abs(producto - pInterseccion) < 1e-9; 
}

// 7. Combinatoria (Necesaria para ejercicios de conteo
// C(n, k) = n! / (k! * (n-k)!)
unsigned long long factorial(int n) {
    unsigned long long f = 1;
    for (int i = 1; i <= n; ++i) f *= i;
    return f;
}

unsigned long long combinatoria(int n, int k) {
    if (k < 0 || k > n) return 0;
    return factorial(n) / (factorial(k) * factorial(n - k));
}


/* TEOREMA DE BAYES
   Fórmula: P(Ai|B) = (P(Ai) * P(B|Ai)) / Sumatoria Total
*/
double teoremaBayes(int indice, const vector<double>& probA, const vector<double>& condicionales) {
    
    // 1. Calculamos TU camino específico (El numerador)
    // Es simplemente: La probabilidad de tu evento * Su condicional
    double miCamino = probA[indice] * condicionales[indice];

    // 2. Calculamos la Probabilidad Total (El denominador)
    // Es la suma de todos los caminos posibles (Probabilidad Total)
    double total = 0.0;
    for (size_t i = 0; i < probA.size(); ++i) {
        total += probA[i] * condicionales[i];
    }

    // Evitamos división por cero
    if (total == 0) return 0.0;

    // 3. Resultado final
    return miCamino / total;
}

int main() {
    cout << "--- DEMOSTRACION DE FORMULAS ---" << endl;

    // 1. Ejemplo Dados (PDF 1, Pág 16)
    // P(Dobles) = 1/6, P(Suma6) = 5/36, P(Ambos) = 1/36
    double pDobles = 1.0/6.0;
    double pSuma6 = 5.0/36.0;
    double pInterseccion = 1.0/36.0;
    
    double unionRes = probabilidadUnion(pDobles, pSuma6, pInterseccion);
    cout << "1. Union (Dados Pág 16): " << unionRes << " (Esperado: 0.2777... o 5/18)" << endl;

    // 2. Ejemplo Microprocesadores (PDF 1, Pág 14)
    // Usando Combinatoria
    double casosTotales = combinatoria(1000, 5);
    double casosFavorables = combinatoria(980, 5); // 980 buenos
    double probNingunDefectuoso = reglaLaplace(casosFavorables, casosTotales);
    // Complemento: Al menos uno defectuoso
    double probAlMenosUno = probabilidadComplemento(probNingunDefectuoso);
    
    cout << "2. Complemento (Chips Pág 14): " << probAlMenosUno << " (Esperado: ~0.096)" << endl;

    // CASO: ASCENSORES
    // Ascensor 1 (Índice 0): Se usa 45% (0.45) y falla 5% (0.05)
    // Ascensor 2 (Índice 1): Se usa 55% (0.55) y falla 8% (0.08)

    // P(A): Probabilidades de usar cada ascensor
    vector<double> probA = {0.45, 0.55}; 

    // P(B|A): Probabilidades de que falle dado que es ese ascensor
    vector<double> condicionales = {0.05, 0.08};

    // Queremos saber la probabilidad de que sea el Ascensor 2 (Índice 1)
    double resultado = teoremaBayes(1, probA, condicionales);

    cout << "Probabilidad de que sea el Ascensor 2: " << resultado << endl;
    // Resultado esperado: 0.662
    
    return 0;
}
