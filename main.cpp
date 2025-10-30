#include <iostream>
#include <string>
using namespace std;
#include "Genoma.h" 

// ============================
// Funciones para ayuda
// ============================
void ayudaCargar()            { cout << "Uso: cargar archivo\n"; }
void ayudaListarSecuencias()  { cout << "Uso: listar_secuencias\n"; }
void ayudaHistograma()        { cout << "Uso: histograma descripcion\n"; }
void ayudaEsSubsecuencia()    { cout << "Uso: es_subsecuencia subsecuencia\n"; }
void ayudaEnmascarar()        { cout << "Uso: enmascarar subsecuencia\n"; }
void ayudaGuardar()           { cout << "Uso: guardar nombre_archivo\n"; }
void ayudaCodificar()         { cout << "Uso: codificar archivo.fabin\n"; }
void ayudaDecodificar()       { cout << "Uso: decodificar archivo.fabin\n"; }
void ayudaSalir()             { cout << "Uso: salir\n"; }

// ============================
// Función para separar palabras (tokens por espacios)
// ============================
int separarPalabras(string linea, string palabras[], int max) {
    int n = 0;
    string palabra = "";
    for (size_t i = 0; i < linea.length(); i++) {
        if (linea[i] == ' ') {
            if (palabra != "") {
                palabras[n++] = palabra;
                palabra = "";
                if (n == max) break;
            }
        } else {
            palabra += linea[i];
        }
    }
    if (palabra != "" && n < max) {
        palabras[n++] = palabra;
    }
    return n;
}

// ============================
// Programa principal
// ============================
int main() {
    Genoma g;                       
    string linea, palabras[10];
    int cantidad;
    string comando = "";

    while (comando.compare("salir") != 0) {
        cout << "$ ";
        if (!getline(cin, linea)) break;

        cantidad = separarPalabras(linea, palabras, 10);
        if (cantidad == 0) continue;

        comando = palabras[0];

        // ---------------- salir ----------------
        if (comando.compare("salir") == 0) {
            if (cantidad != 1) ayudaSalir();
            else break;
        }

        // ---------------- ayuda ----------------
        else if (comando.compare("ayuda") == 0) {
            if (cantidad == 1) {
                cout << "Comandos disponibles:\n";
                cout << " - cargar archivo\n";
                cout << " - listar_secuencias\n";
                cout << " - histograma desc\n";
                cout << " - es_subsecuencia sub\n";
                cout << " - enmascarar sub\n";
                cout << " - guardar archivo\n";
                cout << " - codificar archivo\n";
                cout << " - decodificar archivo\n";
                cout << " - salir\n";
                cout << "Uso: ayuda [comando]\n";
            } else if (cantidad == 2) {
                string subcomando = palabras[1];
                if      (subcomando.compare("cargar") == 0)            ayudaCargar();
                else if (subcomando.compare("listar_secuencias") == 0) ayudaListarSecuencias();
                else if (subcomando.compare("histograma") == 0)        ayudaHistograma();
                else if (subcomando.compare("es_subsecuencia") == 0)   ayudaEsSubsecuencia();
                else if (subcomando.compare("enmascarar") == 0)        ayudaEnmascarar();
                else if (subcomando.compare("guardar") == 0)           ayudaGuardar();
                else if (subcomando.compare("codificar") == 0)         ayudaCodificar();
                else if (subcomando.compare("decodificar") == 0)       ayudaDecodificar();
                else if (subcomando.compare("salir") == 0)             ayudaSalir();
                else                                                   cout << "Comando no reconocido. Use 'ayuda'.\n";
            } else {
                cout << "Uso: ayuda [comando]\n";
            }
        }

        // ---------------- cargar <archivo> ----------------
        else if (comando.compare("cargar") == 0) {
            if (cantidad != 2) { ayudaCargar(); continue; }
            g.Cargar(palabras[1]);
        }

        // ---------------- listar_secuencias ----------------
        else if (comando.compare("listar_secuencias") == 0) {
            if (cantidad != 1) { ayudaListarSecuencias(); continue; }
            g.ListarSecuencias();
        }

        // ---------------- histograma <descripcion> ----------------
        else if (comando.compare("histograma") == 0) {
            if (cantidad != 2) { ayudaHistograma(); continue; }
            string desc = palabras[1];
            if (!desc.empty() && desc[0] == '>') desc = desc.substr(1); // por si escriben >S1
            g.HistogramaDe(desc);
        }

        // ---------------- es_subsecuencia <sub> ----------------
        else if (comando.compare("es_subsecuencia") == 0) {
            if (cantidad != 2) { ayudaEsSubsecuencia(); continue; }
            g.EsSubsecuencia(palabras[1]);
        }

        // ---------------- enmascarar <sub> ----------------
        else if (comando.compare("enmascarar") == 0) {
            if (cantidad != 2) { ayudaEnmascarar(); continue; }
            if (palabras[1] == "\"\"" || palabras[1] == "''") g.Enmascarar(string());
            else g.Enmascarar(palabras[1]);
        }

        // ---------------- guardar <archivo> ----------------
        else if (comando.compare("guardar") == 0) {
            if (cantidad != 2) { ayudaGuardar(); continue; }
            g.Guardar(palabras[1]);
        }

        // ----- comandos 2da entrega -----
        else if (comando.compare("codificar") == 0)
        {
            if (cantidad != 2)
            {
                ayudaCodificar();
                continue;
            }
            g.Codificar(palabras[1]);
        }
        else if (comando.compare("decodificar") == 0)
        {
            if (cantidad != 2)
            {
                ayudaDecodificar();
                continue;
            }
            g.Decodificar(palabras[1]);
        }

        // ---------------- no reconocido ----------------
        else {
            cout << "Comando no reconocido. Use 'ayuda'.\n";
        }
    }

    return 0;
}
