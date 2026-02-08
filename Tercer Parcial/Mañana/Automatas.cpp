#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

// --- ESTRUCTURA PARA DFA (T7) ---
struct DFA {
    int estadoInicial;
    set<int> estadosFinales;
    map<pair<int, char>, int> transiciones;

    bool procesar(string input) {
        int estadoActual = estadoInicial;
        for (char c : input) {
            if (transiciones.count({estadoActual, c})) {
                estadoActual = transiciones[{estadoActual, c}];
            } else {
                return false; // Transición no definida
            }
        }
        return estadosFinales.count(estadoActual);
    }
};

// --- ESTRUCTURA PARA NFA (T8) ---
struct NFA {
    int estadoInicial;
    set<int> estadosFinales;
    map<pair<int, char>, set<int>> transiciones;

    bool procesar(string input) {
        set<int> estadosActuales;
        estadosActuales.insert(estadoInicial);

        for (char c : input) {
            set<int> proccesing;
            for (int s : estadosActuales) {
                if (transiciones.count({s, c})) {
                    for (int sig : transiciones[{s, c}]) proccesing.insert(sig);
                }
            }
            estadosActuales = proccesing;
        }

        for (int s : estadosActuales) {
            if (estadosFinales.count(s)) return true;
        }
        return false;
    }
};

// --- MÁQUINA DE MOORE (T8: Output depende del estado) ---
struct Moore {
    int estadoInicial;
    map<pair<int, char>, int> transiciones;
    map<int, string> outputEstado;

    void ejecutar(string input) {
        int s = estadoInicial;
        cout << "Moore Output: " << outputEstado[s];
        for (char c : input) {
            s = transiciones[{s, c}];
            cout << outputEstado[s];
        }
        cout << endl;
    }
};

// --- MÁQUINA DE MEALY (T8: Output depende de transición) ---
struct Mealy {
    int estadoInicial;
    map<pair<int, char>, pair<int, string>> transiciones; // {s, c} -> {sigEstado, output}

    void ejecutar(string input) {
        int s = estadoInicial;
        cout << "Mealy Output: ";
        for (char c : input) {
            string out = transiciones[{s, c}].second;
            s = transiciones[{s, c}].first;
            cout << out;
        }
        cout << endl;
    }
};

int main() {
    // --- EJEMPLO DFA: Acepta cadenas que terminan en '1' (Alfabeto {0,1}) ---
    DFA miDfa;
    miDfa.estadoInicial = 0;
    miDfa.estadosFinales = {1};
    miDfa.transiciones = {
        {{0, '0'}, 0}, {{0, '1'}, 1},
        {{1, '0'}, 0}, {{1, '1'}, 1}
    };

    cout << "DFA (Termina en 1) '0101': " << (miDfa.procesar("0101") ? "Aceptada" : "Rechazada") << endl;

    // --- EJEMPLO MOORE: Contador de ocurrencias de 'ab' ---
    Moore miMoore;
    miMoore.estadoInicial = 0;
    miMoore.outputEstado = {{0, "0"}, {1, "0"}, {2, "1"}}; // q2 indica que se encontró 'ab'
    miMoore.transiciones = {
        {{0, 'a'}, 1}, {{0, 'b'}, 0},
        {{1, 'a'}, 1}, {{1, 'b'}, 2},
        {{2, 'a'}, 1}, {{2, 'b'}, 0}
    };
    miMoore.ejecutar("abab");

    // --- EJEMPLO MEALY: Complemento a 1 (Invierte bits) ---
    Mealy miMealy;
    miMealy.estadoInicial = 0;
    miMealy.transiciones = {
        {{0, '0'}, {0, "1"}},
        {{0, '1'}, {0, "0"}}
    };
    miMealy.ejecutar("1011");

    return 0;
}
