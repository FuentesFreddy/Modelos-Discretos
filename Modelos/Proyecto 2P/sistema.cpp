#include "sistema.h"

// --- IMPLEMENTACIÓN DE FUNCIONES LÓGICAS ---

bool P(Proyecto p) { return p.diasRetraso > 10; }
bool Q(Proyecto p) { return p.errores > 3; }
bool R(Proyecto p) { return p.revisionUrgente; }

EstadoProduccion estadoProyecto(const Proyecto& p) {
    bool cumpleMetas = (p.diasRetraso <= 0 && p.errores <= 1);
    bool falloMec = (p.errores > 0);
    bool validado = p.completado; 

    if (cumpleMetas && !falloMec && validado) return NORMAL;
    if (!cumpleMetas && !falloMec && validado) return BAJO_RENDIMIENTO;
    if (falloMec && validado) return FALLA_MECANICA;
    if (!validado && !falloMec) return OBSERVACION;
    return CRITICO;
}

long long comb(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    long long res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - i + 1) / i;
    }
    return res;
}

// --- IMPLEMENTACIÓN DE CLASE LISTAENLAZADA ---

ListaEnlazada::ListaEnlazada() { 
    head = nullptr; 
}

void ListaEnlazada::insertar(Proyecto p) {
    Nodo* nuevo = new Nodo;
    nuevo->data = p;
    nuevo->next = head;
    head = nuevo;
}

bool ListaEnlazada::contiene(string nombre) {
    Nodo* aux = head;
    while (aux) {
        if (aux->data.nombre == nombre)
            return true;
        aux = aux->next;
    }
    return false;
}

void ListaEnlazada::mostrar(string titulo) {
    cout << "\n" << titulo << ":\n";
    Nodo* aux = head;
    if (!aux) {
        cout << "  (vacio)\n";
        return;
    }
    while (aux) {
        cout << "  - " << aux->data.nombre << endl;
        aux = aux->next;
    }
}

ListaEnlazada ListaEnlazada::interseccion(ListaEnlazada B) {
    ListaEnlazada resultado;
    Nodo* a = head;
    while (a) {
        if (B.contiene(a->data.nombre))
            resultado.insertar(a->data);
        a = a->next;
    }
    return resultado;
}

ListaEnlazada ListaEnlazada::unionConjuntos(ListaEnlazada B) {
    ListaEnlazada resultado = *this; 
    Nodo* b = B.head;
    while (b) {
        if (!resultado.contiene(b->data.nombre))
            resultado.insertar(b->data);
        b = b->next;
    }
    return resultado;
}

ListaEnlazada ListaEnlazada::diferencia(ListaEnlazada B) {
    ListaEnlazada resultado;
    Nodo* a = head;
    while (a) {
        if (!B.contiene(a->data.nombre))
            resultado.insertar(a->data);
        a = a->next;
    }
    return resultado;
}

// --- IMPLEMENTACIÓN DE CLASE SISTEMARIESGO ---

void SistemaRiesgo::agregarProyecto() {
    Proyecto p;
    cout << "\n--- Ingresar nuevo proyecto ---\n";
    cin.ignore();
    cout << "Nombre del proyecto: ";
    getline(cin, p.nombre);
    cout << "Dias de retraso: ";
    cin >> p.diasRetraso;
    cout << "Numero de errores criticos: ";
    cin >> p.errores;
    cout << "¿Requiere revision urgente? (1=Si, 0=No): ";
    cin >> p.revisionUrgente;
    cout << "¿Proyecto completado? (1=Si, 0=No): ";
    cin >> p.completado;

    activos.insertar(p);

    if (p.diasRetraso > 0) retraso.insertar(p);
    if (p.completado) completados.insertar(p);

    ofstream file("proyectos.txt", ios::app);
    if (file.is_open()) {
        file << p.nombre << ","
             << p.diasRetraso << ","
             << p.errores << ","
             << p.revisionUrgente << ","
             << p.completado << endl;
        file.close();
    } else {
        cout << "Error al abrir el archivo para guardar.\n";
    }
    cout << "Proyecto agregado exitosamente.\n";
}

void SistemaRiesgo::evaluarProyectos() {
    cout << "\n--- Evaluando proyectos ---\n";
    Nodo* aux = activos.head;
    while (aux) {
        Proyecto p = aux->data;
        cout << "\nProyecto: " << p.nombre << endl;
        cout << "P (retraso>10): " << (P(p) ? "Verdadero" : "Falso") << endl;
        cout << "Q (errores>3): " << (Q(p) ? "Verdadero" : "Falso") << endl;
        cout << "R (revision urgente): " << (R(p) ? "Verdadero" : "Falso") << endl;

        if (P(p)) p.revisionUrgente = true;
        if (Q(p) && P(p))
            cout << " Resultado: Riesgo ALTO\n";
        else if (!P(p) && !Q(p))
            cout << " Resultado: Proyecto ESTABLE\n";
        else if (P(p))
            cout << " Resultado: Requiere REVISION URGENTE\n";
        else
            cout << " Resultado: Riesgo MODERADO\n";

        EstadoProduccion e = estadoProyecto(p);
        cout << " Estado de produccion: ";
        switch (e) {
            case NORMAL: cout << "NORMAL"; break;
            case BAJO_RENDIMIENTO: cout << "BAJO_RENDIMIENTO"; break;
            case FALLA_MECANICA: cout << "FALLA_MECANICA"; break;
            case OBSERVACION: cout << "OBSERVACION"; break;
            case CRITICO: cout << "CRITICO"; break;
        }
        cout << endl;
        aux = aux->next;
    }
}

void SistemaRiesgo::mostrarConjuntos() {
    activos.mostrar("Conjunto A (Activos)");
    retraso.mostrar("Conjunto B (Con retraso)");
    completados.mostrar("Conjunto C (Completados)");

    ListaEnlazada inter = activos.interseccion(retraso);
    ListaEnlazada unionBC = retraso.unionConjuntos(completados);
    ListaEnlazada dif = activos.diferencia(completados);

    inter.mostrar("A n B (Activos con retraso)");
    unionBC.mostrar("B U C (Revision o finalizados)");
    dif.mostrar("A - C (En desarrollo)");
}

void SistemaRiesgo::resumenFinal() {
    int total = 0, riesgos = 0, estables = 0;
    Nodo* aux = activos.head;
    while (aux) {
        total++;
        Proyecto p = aux->data;
        if (Q(p) && P(p)) riesgos++;
        if (!P(p) && !Q(p)) estables++;
        aux = aux->next;
    }
    cout << "\n===== RESUMEN FINAL =====\n";
    cout << "Total de proyectos: " << total << endl;
    cout << "Proyectos en riesgo alto: " << riesgos << endl;
    cout << "Proyectos estables: " << estables << endl;
}

void SistemaRiesgo::probabilidadesEstados() {
    int total = 0;
    int fallo = 0, bajoRend = 0, normal = 0;

    Nodo* aux = activos.head;
    while (aux) {
        total++;
        Proyecto p = aux->data;
        bool esFallo = Q(p) && P(p);      
        bool esNormal = !P(p) && !Q(p);   
        bool esBajo = (!esFallo && !esNormal);

        if (esFallo) fallo++;
        else if (esNormal) normal++;
        else if (esBajo) bajoRend++;
        aux = aux->next;
    }

    if (total == 0) {
        cout << "No hay proyectos para evaluar.\n";
        return;
    }

    cout << "\n=== Probabilidades discretas ===\n";
    cout << "P(Fallo) = " << (double)fallo / total << endl;
    cout << "P(Bajo rendimiento) = " << (double)bajoRend / total << endl;
    cout << "P(Operacion normal) = " << (double)normal / total << endl;
}

void SistemaRiesgo::analizarCombinacionesRiesgo() {
    int k;
    cout << "\nIngrese k (numero de proyectos a seleccionar para revision): ";
    cin >> k;

    vector<Proyecto> riesgoAlto;
    Nodo* aux = activos.head;
    while (aux) {
        Proyecto p = aux->data;
        if (Q(p) && P(p)) riesgoAlto.push_back(p);
        aux = aux->next;
    }

    int n = (int)riesgoAlto.size();
    cout << "Proyectos en riesgo alto: " << n << endl;
    if (k < 0 || k > n) {
        cout << "k fuera de rango.\n";
        return;
    }
    cout << "Formas de seleccionar " << k
         << " proyectos en riesgo (combinaciones, sin orden): "
         << comb(n, k) << endl;
}

void SistemaRiesgo::conteoEstadosProduccion() {
    int total = 0;
    int cNormal = 0, cBajo = 0, cFalla = 0, cObs = 0, cCritico = 0;

    Nodo* aux = activos.head;
    while (aux) {
        total++;
        EstadoProduccion e = estadoProyecto(aux->data);
        switch (e) {
            case NORMAL: cNormal++; break;
            case BAJO_RENDIMIENTO: cBajo++; break;
            case FALLA_MECANICA: cFalla++; break;
            case OBSERVACION: cObs++; break;
            case CRITICO: cCritico++; break;
        }
        aux = aux->next;
    }

    cout << "\n=== Conteo de estados de produccion ===\n";
    cout << "Total proyectos: " << total << endl;
    cout << "NORMAL: " << cNormal << endl;
    cout << "BAJO_RENDIMIENTO: " << cBajo << endl;
    cout << "FALLA_MECANICA: " << cFalla << endl;
    cout << "OBSERVACION: " << cObs << endl;
    cout << "CRITICO: " << cCritico << endl;
    cout << "Use estos niveles para construir el Diagrama de Hasse en el informe.\n";
}

void SistemaRiesgo::cargarProyectos() {
    ifstream file("proyectos.txt");
    if (!file.is_open()) {
        cout << "No se pudo abrir proyectos.txt. Iniciando con lista vacia.\n";
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string nombre, diasStr, erroresStr, revStr, compStr;
        getline(ss, nombre, ',');
        getline(ss, diasStr, ',');
        getline(ss, erroresStr, ',');
        getline(ss, revStr, ',');
        getline(ss, compStr, ',');
        Proyecto p;
        p.nombre = nombre;
        p.diasRetraso = stoi(diasStr);
        p.errores = stoi(erroresStr);
        p.revisionUrgente = stoi(revStr);
        p.completado = stoi(compStr);
        
        activos.insertar(p);
        if (p.diasRetraso > 0) retraso.insertar(p);
        if (p.completado) completados.insertar(p);
    }
    file.close();
    cout << "Proyectos cargados desde proyectos.txt.\n";
}
