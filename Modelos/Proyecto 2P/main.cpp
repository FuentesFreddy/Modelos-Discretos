#include "sistema.h"

int main() {
    SistemaRiesgo sistema; // Objeto principal
    sistema.cargarProyectos();
    int opcion;

    do {
        cout << "\n=========================================\n";
        cout << "  SISTEMA DE DIAGNOSTICO DE RIESGOS\n";
        cout << "=========================================\n";
        cout << "1. Ingresar proyecto\n";
        cout << "2. Evaluar proyectos\n";
        cout << "3. Mostrar conjuntos (A, B, C)\n";
        cout << "4. Mostrar resumen final\n";
        cout << "5. Probabilidades discretas de estados\n";
        cout << "6. Analizar combinaciones en proyectos de riesgo\n";
        cout << "7. Conteo de estados (para Hasse)\n";
        cout << "8. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1: sistema.agregarProyecto(); break;
        case 2: sistema.evaluarProyectos(); break;
        case 3: sistema.mostrarConjuntos(); break;
        case 4: sistema.resumenFinal(); break;
        case 5: sistema.probabilidadesEstados(); break;
        case 6: sistema.analizarCombinacionesRiesgo(); break;
        case 7: sistema.conteoEstadosProduccion(); break;
        case 8: cout << "Saliendo del sistema...\n"; break;
        default: cout << "Opcion invalida.\n"; break;
        }

    } while (opcion != 8);

    return 0;
}
