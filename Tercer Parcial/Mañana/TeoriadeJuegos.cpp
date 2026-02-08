#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// Estructura para representar pagos de dos jugadores (Forma Normal - Pág. 7)
struct Pago {
    double p1; // Jugador 1 (Filas)
    double p2; // Jugador 2 (Columnas)
};

class GameTheoryToolbox {
private:
    vector<vector<Pago>> matriz;
    int filas, columnas;
    vector<string> nombresF, nombresC;

public:
    GameTheoryToolbox(int f, int c) : filas(f), columnas(c) {
        matriz.resize(f, vector<Pago>(c));
        for(int i=0; i<f; ++i) nombresF.push_back("Fila " + to_string(i+1));
        for(int j=0; j<c; ++j) nombresC.push_back("Col " + to_string(j+1));
    }

    void setNombres(vector<string> f, vector<string> c) { nombresF = f; nombresC = c; }
    void setPago(int f, int c, double v1, double v2) { matriz[f][c] = {v1, v2}; }

    // --- 1. EQUILIBRIO DE NASH (Págs. 16-17) ---
    void buscarNash() {
        cout << "\n>>> BUSCANDO EQUILIBRIOS DE NASH (Estrategias Puras):" << endl;
        bool encontrado = false;
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < columnas; ++j) {
                bool mejorP1 = true, mejorP2 = true;
                // P1 busca el máximo en su columna
                for (int k = 0; k < filas; ++k) if (matriz[k][j].p1 > matriz[i][j].p1) mejorP1 = false;
                // P2 busca el máximo en su fila
                for (int k = 0; k < columnas; ++k) if (matriz[i][k].p2 > matriz[i][j].p2) mejorP2 = false;

                if (mejorP1 && mejorP2) {
                    cout << " - Nash detectado en: [" << nombresF[i] << ", " << nombresC[j] 
                         << "] con pagos (" << matriz[i][j].p1 << ", " << matriz[i][j].p2 << ")" << endl;
                    encontrado = true;
                }
            }
        }
        if (!encontrado) cout << " No existen equilibrios de Nash en estrategias puras." << endl;
    }

    // --- 2. ESTRATEGIAS DOMINANTES (Págs. 12-13) ---
    void analizarDominancia() {
        cout << "\n>>> ANALIZANDO ESTRATEGIAS DOMINANTES:" << endl;
        // Dominante P1
        for (int i = 0; i < filas; ++i) {
            bool esDominante = true;
            for (int k = 0; k < filas; ++k) {
                if (i == k) continue;
                for (int j = 0; j < columnas; ++j) if (matriz[i][j].p1 <= matriz[k][j].p1) esDominante = false;
            }
            if (esDominante) cout << " - Jugador 1 tiene estrategia dominante: " << nombresF[i] << endl;
        }
        // Dominante P2
        for (int j = 0; j < columnas; ++j) {
            bool esDominante = true;
            for (int k = 0; k < columnas; ++k) {
                if (j == k) continue;
                for (int i = 0; i < filas; ++i) if (matriz[i][j].p2 <= matriz[i][k].p2) esDominante = false;
            }
            if (esDominante) cout << " - Jugador 2 tiene estrategia dominante: " << nombresC[j] << endl;
        }
    }

    // --- 3. CRITERIOS DE DECISIÓN BAJO INCERTIDUMBRE (Pág. 21) ---
    // Se usa la ganancia de P1 para simular toma de decisiones
    void criteriosDecision(double alfa = 0.5) {
        cout << "\n>>> CRITERIOS DE DECISIÓN (Para Jugador 1):" << endl;
        double maxWald = -1e9, maxMaximax = -1e9, maxHurwicz = -1e9;
        int sWald, sMaximax, sHurwicz;

        for (int i = 0; i < filas; ++i) {
            double minF = matriz[i][0].p1, maxF = matriz[i][0].p1;
            for (int j = 1; j < columnas; ++j) {
                minF = min(minF, matriz[i][j].p1);
                maxF = max(maxF, matriz[i][j].p1);
            }
            if (minF > maxWald) { maxWald = minF; sWald = i; }
            if (maxF > maxMaximax) { maxMaximax = maxF; sMaximax = i; }
            double h = (alfa * maxF) + (1.0 - alfa) * minF;
            if (h > maxHurwicz) { maxHurwicz = h; sHurwicz = i; }
        }
        cout << " 1. Wald (Maximin): " << nombresF[sWald] << " (Gana al menos " << maxWald << ")" << endl;
        cout << " 2. Maximax: " << nombresF[sMaximax] << " (Máximo posible " << maxMaximax << ")" << endl;
        cout << " 3. Hurwicz (a=" << alfa << "): " << nombresF[sHurwicz] << endl;
        
        // Criterio de Savage (Arrepentimiento)
        vector<vector<double>> regret(filas, vector<double>(columnas));
        for (int j = 0; j < columnas; ++j) {
            double maxCol = -1e9;
            for (int i = 0; i < filas; ++i) maxCol = max(maxCol, matriz[i][j].p1);
            for (int i = 0; i < filas; ++i) regret[i][j] = maxCol - matriz[i][j].p1;
        }
        double minRegret = 1e9; int sSavage;
        for (int i = 0; i < filas; ++i) {
            double maxR = *max_element(regret[i].begin(), regret[i].end());
            if (maxR < minRegret) { minRegret = maxR; sSavage = i; }
        }
        cout << " 4. Savage: " << nombresF[sSavage] << " (Minimiza arrepentimiento)" << endl;
    }

    // --- 4. REDUCCIÓN POR ESTRATEGIAS DOMINADAS (Págs. 37-41) ---
    // Útil para el problema de El Cóndor vs Gremio
    void reduccionIterativa() {
        cout << "\n>>> INICIANDO ELIMINACIÓN DE ESTRATEGIAS DOMINADAS:" << endl;
        vector<vector<Pago>> tempM = matriz;
        vector<string> tempF = nombresF, tempC = nombresC;

        bool cambio = true;
        while (cambio && tempM.size() > 1 && tempM[0].size() > 1) {
            cambio = false;
            // Eliminar filas dominadas (P1 maximiza)
            for (size_t i = 0; i < tempM.size(); ++i) {
                for (size_t k = 0; k < tempM.size(); ++k) {
                    if (i == k) continue;
                    bool dominada = true;
                    for (size_t j = 0; j < tempM[0].size(); ++j)
                        if (tempM[i][j].p1 > tempM[k][j].p1) { dominada = false; break; }
                    if (dominada) {
                        cout << " - P1 elimina " << tempF[i] << " (dominada por " << tempF[k] << ")" << endl;
                        tempM.erase(tempM.begin() + i); tempF.erase(tempF.begin() + i);
                        cambio = true; break;
                    }
                }
                if (cambio) break;
            }
            if (cambio) continue;
            // Eliminar columnas dominadas (P2 maximiza sus propios pagos)
            for (size_t j = 0; j < tempM[0].size(); ++j) {
                for (size_t k = 0; k < tempM[0].size(); ++k) {
                    if (j == k) continue;
                    bool dominada = true;
                    for (size_t i = 0; i < tempM.size(); ++i)
                        if (tempM[i][j].p2 > tempM[i][k].p2) { dominada = false; break; }
                    if (dominada) {
                        cout << " - P2 elimina " << tempC[j] << " (dominada por " << tempC[k] << ")" << endl;
                        for (size_t r = 0; r < tempM.size(); ++r) tempM[r].erase(tempM[r].begin() + j);
                        tempC.erase(tempC.begin() + j); cambio = true; break;
                    }
                }
                if (cambio) break;
            }
        }
        cout << " Resultado Final: [" << tempF[0] << ", " << tempC[0] << "] Pago: " << tempM[0][0].p1 << endl;
    }
};

int main() {
    // Ejemplo: Dilema del Prisionero (Pág. 18)
    GameTheoryToolbox dp(2, 2);
    dp.setNombres({"Confesar", "No Confesar"}, {"Confesar", "No Confesar"});
    dp.setPago(0,0, -6, -6); dp.setPago(0,1, -1, -10);
    dp.setPago(1,0, -10, -1); dp.setPago(1,1, -3, -3);
    
    cout << "--- CASO 1: DILEMA DEL PRISIONERO ---";
    dp.analizarDominancia();
    dp.buscarNash();
    dp.criteriosDecision(0.5);

    // Ejemplo final: El Cóndor vs Gremio (Pág. 36)
    // Nota: Como Gremio busca minimizar pérdidas, tratamos sus pagos como negativos de Cóndor
    GameTheoryToolbox condor(3, 4);
    condor.setNombres({"Cafe", "Leche", "Harina"}, {"Cafe", "Leche", "Azucar", "Avena"});
    vector<vector<double>> v = {{50,20,120,-50}, {60,20,70,60}, {-20,0,-40,60}};
    for(int i=0; i<3; i++) for(int j=0; j<4; j++) condor.setPago(i,j, v[i][j], -v[i][j]);

    cout << "\n\n--- CASO 2: EL CÓNDOR VS GREMIO (REDUCCIÓN) ---";
    condor.reduccionIterativa();

    return 0;
}
