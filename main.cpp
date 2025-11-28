#include <iostream>
#include <sstream>
#include <string>

#include "Genoma.h"

using namespace std;

// ============================
// Funciones de ayuda
// ============================
void ayudaCargar()            { cout << "Uso: cargar archivo\n"; }
void ayudaListarSecuencias()  { cout << "Uso: listar_secuencias\n"; }
void ayudaHistograma()        { cout << "Uso: histograma descripcion_secuencia\n"; }
void ayudaEsSubsecuencia()    { cout << "Uso: es_subsecuencia subsecuencia\n"; }
void ayudaEnmascarar()        { cout << "Uso: enmascarar subsecuencia\n"; }
void ayudaGuardar()           { cout << "Uso: guardar nombre_archivo\n"; }
void ayudaCodificar()         { cout << "Uso: codificar archivo.fabin\n"; }
void ayudaDecodificar()       { cout << "Uso: decodificar archivo.fabin\n"; }
void ayudaRutaMasCorta()      { cout << "Uso: ruta_mas_corta descripcion_secuencia i j x y\n"; }
void ayudaBaseRemota()        { cout << "Uso: base_remota descripcion_secuencia i j\n"; }
void ayudaSalir()             { cout << "Uso: salir\n"; }

// ============================
// Programa principal
// ============================
int main() {
    Genoma g;
    string linea;

    while (true) {
        cout << "$ ";
        if (!std::getline(cin, linea)) {
            break;
        }
        if (linea.empty()) continue;

        std::istringstream iss(linea);
        string comando;
        iss >> comando;
        if (comando.empty()) continue;

        // ---------------- salir ----------------
        if (comando == "salir") {
            string extra;
            if (iss >> extra) {
                ayudaSalir();
                continue;
            }
            break;
        }

        // ---------------- ayuda ----------------
        else if (comando == "ayuda") {
            string sub;
            if (!(iss >> sub)) {
                cout << "Comandos disponibles:\n"
                     << "  cargar archivo\n"
                     << "  listar_secuencias\n"
                     << "  histograma descripcion_secuencia\n"
                     << "  es_subsecuencia subsecuencia\n"
                     << "  enmascarar subsecuencia\n"
                     << "  guardar nombre_archivo\n"
                     << "  codificar archivo.fabin\n"
                     << "  decodificar archivo.fabin\n"
                     << "  ruta_mas_corta descripcion_secuencia i j x y\n"
                     << "  base_remota descripcion_secuencia i j\n"
                     << "  salir\n";
            } else if (sub == "cargar") ayudaCargar();
            else if (sub == "listar_secuencias") ayudaListarSecuencias();
            else if (sub == "histograma") ayudaHistograma();
            else if (sub == "es_subsecuencia") ayudaEsSubsecuencia();
            else if (sub == "enmascarar") ayudaEnmascarar();
            else if (sub == "guardar") ayudaGuardar();
            else if (sub == "codificar") ayudaCodificar();
            else if (sub == "decodificar") ayudaDecodificar();
            else if (sub == "ruta_mas_corta") ayudaRutaMasCorta();
            else if (sub == "base_remota") ayudaBaseRemota();
            else if (sub == "salir") ayudaSalir();
            else cout << "Comando no reconocido. Use 'ayuda'.\n";
        }

        // ---------------- cargar <archivo> ----------------
        else if (comando == "cargar") {
            string archivo;
            if (!(iss >> archivo)) {
                ayudaCargar();
                continue;
            }
            g.Cargar(archivo);
        }

        // ---------------- listar_secuencias ----------------
        else if (comando == "listar_secuencias") {
            string extra;
            if (iss >> extra) {
                ayudaListarSecuencias();
                continue;
            }
            g.ListarSecuencias();
        }

        // ---------------- histograma descripcion ----------------
        else if (comando == "histograma") {
            string desc;
            if (!(iss >> desc)) {
                ayudaHistograma();
                continue;
            }
            g.HistogramaDe(desc);
        }

        // ---------------- es_subsecuencia subsecuencia ----------------
        else if (comando == "es_subsecuencia") {
            string sub;
            if (!(iss >> sub)) {
                ayudaEsSubsecuencia();
                continue;
            }
            g.EsSubsecuencia(sub);
        }

        // ---------------- enmascarar subsecuencia ----------------
        else if (comando == "enmascarar") {
            string sub;
            if (!(iss >> sub)) {
                ayudaEnmascarar();
                continue;
            }
            g.Enmascarar(sub);
        }

        // ---------------- guardar archivo ----------------
        else if (comando == "guardar") {
            string archivo;
            if (!(iss >> archivo)) {
                ayudaGuardar();
                continue;
            }
            g.Guardar(archivo);
        }

        // ---------------- codificar archivo.fabin ----------------
        else if (comando == "codificar") {
            string archivo;
            if (!(iss >> archivo)) {
                ayudaCodificar();
                continue;
            }
            g.Codificar(archivo);
        }

        // ---------------- decodificar archivo.fabin ----------------
        else if (comando == "decodificar") {
            string archivo;
            if (!(iss >> archivo)) {
                ayudaDecodificar();
                continue;
            }
            g.Decodificar(archivo);
        }

        // ---------------- ruta_mas_corta descripcion i j x y ----------------
        else if (comando == "ruta_mas_corta") {
            string desc;
            int i, j, x, y;
            if (!(iss >> desc >> i >> j >> x >> y)) {
                ayudaRutaMasCorta();
                continue;
            }
            g.RutaMasCorta(desc, i, j, x, y);
        }

        // ---------------- base_remota descripcion i j ----------------
        else if (comando == "base_remota") {
            string desc;
            int i, j;
            if (!(iss >> desc >> i >> j)) {
                ayudaBaseRemota();
                continue;
            }
            g.BaseRemota(desc, i, j);
        }

        // ---------------- no reconocido ----------------
        else {
            cout << "Comando no reconocido. Use 'ayuda'.\n";
        }
    }

    return 0;
}