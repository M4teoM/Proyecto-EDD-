#ifndef __GENOMA__HXX__
#define __GENOMA__HXX__

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <list>
#include <vector>
//para dijsktra
#include <queue>
#include <limits>
#include <cmath>
#include "Genoma.h"
#include "util.h"



// ----------------------
// Constructores / Estado
// ----------------------
Genoma::Genoma() : secuencias_() {}
Genoma::~Genoma() {}

bool Genoma::EstaVacio() const {
  return secuencias_.empty();
}
size_t Genoma::Tam() const { 
  return secuencias_.size();
}

// ----------------------
// Cargar
// ----------------------

void Genoma::Cargar(const std::string& ruta) {
  std::ifstream f(ruta.c_str());
  if (!f) {
    std::cout << ruta << " no se encuentra o no puede leerse." << std::endl;
    return;
  }

  std::list<Secuencia> nuevo;
  std::string line, desc, seq;
  unsigned short ancho = 0;
  bool enSec = false;
  bool invalida = false; // marca si la secuencia actual tiene símbolos no permitidos

  while (std::getline(f, line)) {
    if (!line.empty() && line[0] == '>') {
      if (!desc.empty()) {
        if (!invalida && !seq.empty()) {
          nuevo.push_back(Secuencia(desc, seq, ancho));
        }
      }
      // Iniciar nueva secuencia
      desc = line.substr(1);
      seq.clear();
      ancho = 0;
      enSec = true;
      invalida = false;
    } else if (enSec) {
      bool linea_valida = true;
      std::string agregada;
      for (std::size_t i = 0; i < line.size(); ++i) {
        unsigned char cc = static_cast<unsigned char>(line[i]);
        if (std::isspace(cc)) continue;
        char u = static_cast<char>(std::toupper(cc));
        if (ORDEN_FASTA.find(u) == std::string::npos) {
          linea_valida = false;
          break;
        }
        agregada.push_back(u);
      }
      if (!linea_valida) {
        invalida = true;
        continue;
      }
      if (ancho == 0) {
        ancho = static_cast<unsigned short>(agregada.size()); // ancho = largo de la primera línea válida
      }
      seq += agregada;
    }
  }

  // Cerrar última secuencia al EOF
  if (!desc.empty()) {
    if (!invalida && !seq.empty()) {
      nuevo.push_back(Secuencia(desc, seq, ancho));
    }
  }

  if (nuevo.empty()) {
    std::cout << ruta << " no contiene ninguna secuencia." << std::endl;
  } else {
    secuencias_.swap(nuevo);  // sobrescribe el contenido anterior
    size_t n = secuencias_.size();
    std::cout << n << " secuencia" << (n > 1 ? "s" : "") << " cargada" << (n > 1 ? "s" : "")
              << " correctamente desde " << ruta << "." << std::endl;
  }
}
// ----------------------
// ListarSecuencias
// ----------------------
void Genoma::ListarSecuencias() const {
  if (EstaVacio()) {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }

  std::cout << "Hay " << Tam() << " secuencias cargadas en memoria:" << std::endl;

  std::list<Secuencia>::const_iterator it = secuencias_.begin();
  for (; it != secuencias_.end(); ++it) {
    size_t b = it->ContarBases();
    if (it->EsCompleta()) {
      std::cout << "Secuencia " << it->ObtenerDescripcion() << " contiene " << b << " bases." << std::endl;
    } else {
      std::cout << "Secuencia " << it->ObtenerDescripcion() << " contiene al menos " << b << " bases." << std::endl;
    }
  }
}

// ----------------------
// HistogramaDe
// ----------------------
void Genoma::HistogramaDe(const std::string& descripcion) const {
  if (EstaVacio()) {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }

  // Buscar por descripción
  std::list<Secuencia>::const_iterator it = secuencias_.begin();
  while (it != secuencias_.end() && it->ObtenerDescripcion() != descripcion) {
    ++it;
  }
  if (it == secuencias_.end()) {
    std::cout << "Secuencia inválida." << std::endl;
    return;
  }

  Histograma h;
  const std::string& cod = it->ObtenerCodigo();
  for (std::size_t i = 0; i < cod.size(); ++i) {
    h.Acumular(cod[i]);
  }

  // Usa operator<< de Histograma para imprimir en orden FASTA (incluye '-')
  std::cout << h;
}

// ----------------------
// EsSubsecuencia
// ----------------------
void Genoma::EsSubsecuencia(const std::string& sub) const {
  if (EstaVacio()) {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }
  if (sub.empty()) {
    std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria." << std::endl;
    return;
  }

  unsigned long long total = 0ULL;

  std::list<Secuencia>::const_iterator it = secuencias_.begin();
  for (; it != secuencias_.end(); ++it) {
    const std::string& cod = it->ObtenerCodigo();
    std::size_t pos = 0;
    while ((pos = cod.find(sub, pos)) != std::string::npos) {
      ++total;
      ++pos; 
    }
  }

  if (total == 0ULL) {
    std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria." << std::endl;
  } else {
    std::cout << "La subsecuencia dada se repite " << total
              << " veces dentro de las secuencias cargadas en memoria." << std::endl;
  }
}

// ----------------------
// Enmascarar
// ----------------------
void Genoma::Enmascarar(const std::string& sub) {
  if (EstaVacio()) {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }
  if (sub.empty()) {
    std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria, por tanto no se enmascara nada." << std::endl;
    return;
  }

  unsigned long long s = 0ULL;

  std::list<Secuencia>::iterator it = secuencias_.begin();
  for (; it != secuencias_.end(); ++it) {
    std::string& cod = it->RefCodigo();
    std::size_t pos = 0;
    while ((pos = cod.find(sub, pos)) != std::string::npos) {
      // Reemplazar por 'X' manteniendo longitud
      for (std::size_t k = 0; k < sub.size(); ++k) {
        cod[pos + k] = 'X';
      }
      ++s;
      ++pos; // avanzar uno para permitir superposición
    }
  }

  if (s == 0ULL) {
    std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria, por tanto no se enmascara nada." << std::endl;
  } else {
    std::cout << s << " subsecuencias han sido enmascaradas dentro de las secuencias cargadas en memoria." << std::endl;
  }
}

// ----------------------
// Guardar
// ----------------------
void Genoma::Guardar(const std::string& ruta) const {
  if (EstaVacio()) {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }
  std::ofstream out(ruta);
  if (!out) {
    std::cout << "Error guardando en " << ruta << "." << std::endl;
    return;
  }

  std::list<Secuencia>::const_iterator it = secuencias_.begin();
  for (; it != secuencias_.end(); ++it) {
    out << ">" << it->ObtenerDescripcion() << "\n";
    unsigned short w = (it->ObtenerAncho() == 0 ? 60 : it->ObtenerAncho()); //el ? es similar a un if, si es 0 se pone 60, si no se pone el ancho
    const std::vector<std::string> lineas = PartirPorAncho(it->ObtenerCodigo(), w);
    for (std::size_t i = 0; i < lineas.size(); ++i) {
      out << lineas[i] << "\n";
    }
  }
  std::cout << "Las secuencias han sido guardadas en " << ruta << "." << std::endl;
}
// ----------------------
// RutaMasCorta (componente 3)
// ----------------------
void Genoma::RutaMasCorta(const std::string& descripcion, int i, int j, int x, int y) const {
  if (EstaVacio()) {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }

  // buscar secuencia
  std::list<Secuencia>::const_iterator it = secuencias_.begin();
  while (it != secuencias_.end() && it->ObtenerDescripcion() != descripcion) {
    ++it;
  }
  if (it == secuencias_.end()) {
    std::cout << "La secuencia " << descripcion << " no existe." << std::endl;
    return;
  }

  const std::string& cod = it->ObtenerCodigo();
  int ancho = static_cast<int>(it->ObtenerAncho());
  if (ancho <= 0) {
    std::cout << "La base en la posición [" << i << "," << j << "] no existe." << std::endl;
    return;
  }
  int n = static_cast<int>(cod.size());

  auto idx_from_ij = [ancho, n](int fi, int fj) -> int {
    if (fi < 0 || fj < 0) return -1;
    long long idx = static_cast<long long>(fi) * static_cast<long long>(ancho) + fj;
    if (idx < 0 || idx >= n) return -1;
    return static_cast<int>(idx);
  };

  int origen = idx_from_ij(i, j);
  if (origen < 0) {
    std::cout << "La base en la posición [" << i << "," << j << "] no existe." << std::endl;
    return;
  }
  int destino = idx_from_ij(x, y);
  if (destino < 0) {
    std::cout << "La base en la posición [" << x << "," << y << "] no existe." << std::endl;
    return;
  }

  const double INF = std::numeric_limits<double>::infinity();
  std::vector<double> dist(n, INF);
  std::vector<int> prev(n, -1);
  std::vector<bool> vis(n, false);

  struct NodoPQ {
    double dist;
    int idx;
  };
  struct CmpNodoPQ {
    bool operator()(const NodoPQ& a, const NodoPQ& b) const {
      return a.dist > b.dist;
    }
  };

  std::priority_queue<NodoPQ, std::vector<NodoPQ>, CmpNodoPQ> pq;

  dist[origen] = 0.0;
  pq.push(NodoPQ{0.0, origen});

  while (!pq.empty()) {
    NodoPQ cur = pq.top();
    pq.pop();
    int u = cur.idx;
    if (vis[u]) continue;
    vis[u] = true;
    if (u == destino) break;

    int ui = u / ancho;
    int uj = u % ancho;

    auto relajar = [&](int vi, int vj) {
      int v = idx_from_ij(vi, vj);
      if (v < 0) return;
      if (vis[v]) return;
      double w_uv = 1.0 / (1.0 + std::fabs(static_cast<double>(
                         static_cast<int>(cod[u]) - static_cast<int>(cod[v]))));
      double nd = dist[u] + w_uv;
      if (nd < dist[v]) {
        dist[v] = nd;
        prev[v] = u;
        pq.push(NodoPQ{nd, v});
      }
    };

    relajar(ui - 1, uj); // arriba
    relajar(ui + 1, uj); // abajo
    relajar(ui, uj - 1); // izquierda
    relajar(ui, uj + 1); // derecha
  }

  if (!vis[destino]) {
    std::cout << "No existe ruta entre las bases dadas." << std::endl;
    return;
  }

  // reconstruir camino
  std::vector<int> path;
  for (int v = destino; v != -1; v = prev[v]) {
    path.push_back(v);
  }
  std::reverse(path.begin(), path.end());

  std::cout << "Para la secuencia " << descripcion
            << ", la distancia mínima entre la base " << cod[origen]
            << " en [" << i << "," << j << "] y la base " << cod[destino]
            << " en [" << x << "," << y << "] es: " << dist[destino]
            << ", mediante una ruta de " << path.size() << " pasos." << std::endl;
}

// ----------------------
// BaseRemota (componente 3)
// ----------------------
void Genoma::BaseRemota(const std::string& descripcion, int i, int j) const {
  if (EstaVacio()) {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }

  std::list<Secuencia>::const_iterator it = secuencias_.begin();
  while (it != secuencias_.end() && it->ObtenerDescripcion() != descripcion) {
    ++it;
  }
  if (it == secuencias_.end()) {
    std::cout << "La secuencia " << descripcion << " no existe." << std::endl;
    return;
  }

  const std::string& cod = it->ObtenerCodigo();
  int ancho = static_cast<int>(it->ObtenerAncho());
  if (ancho <= 0) {
    std::cout << "La base en la posición [" << i << "," << j << "] no existe." << std::endl;
    return;
  }
  int n = static_cast<int>(cod.size());

  auto idx_from_ij = [ancho, n](int fi, int fj) -> int {
    if (fi < 0 || fj < 0) return -1;
    long long idx = static_cast<long long>(fi) * static_cast<long long>(ancho) + fj;
    if (idx < 0 || idx >= n) return -1;
    return static_cast<int>(idx);
  };

  int origen = idx_from_ij(i, j);
  if (origen < 0) {
    std::cout << "La base en la posición [" << i << "," << j << "] no existe." << std::endl;
    return;
  }

  char base = cod[origen];

  const double INF = std::numeric_limits<double>::infinity();
  std::vector<double> dist(n, INF);
  std::vector<int> prev(n, -1);
  std::vector<bool> vis(n, false);

  struct NodoPQ2 {
    double dist;
    int idx;
  };
  struct CmpNodoPQ2 {
    bool operator()(const NodoPQ2& a, const NodoPQ2& b) const {
      return a.dist > b.dist;
    }
  };

  std::priority_queue<NodoPQ2, std::vector<NodoPQ2>, CmpNodoPQ2> pq;

  dist[origen] = 0.0;
  pq.push(NodoPQ2{0.0, origen});

  while (!pq.empty()) {
    NodoPQ2 cur = pq.top();
    pq.pop();
    int u = cur.idx;
    if (vis[u]) continue;
    vis[u] = true;

    int ui = u / ancho;
    int uj = u % ancho;

    auto relajar = [&](int vi, int vj) {
      int v = idx_from_ij(vi, vj);
      if (v < 0) return;
      if (vis[v]) return;
      double w_uv = 1.0 / (1.0 + std::fabs(static_cast<double>(
                         static_cast<int>(cod[u]) - static_cast<int>(cod[v]))));
      double nd = dist[u] + w_uv;
      if (nd < dist[v]) {
        dist[v] = nd;
        prev[v] = u;
        pq.push(NodoPQ2{nd, v});
      }
    };

    relajar(ui - 1, uj);
    relajar(ui + 1, uj);
    relajar(ui, uj - 1);
    relajar(ui, uj + 1);
  }

  // buscar la base remota (misma letra con mayor distancia)
  int mejor = origen;
  double mejorDist = 0.0;
  for (int idx = 0; idx < n; ++idx) {
    if (cod[idx] != base) continue;
    if (dist[idx] == INF) continue;
    if (dist[idx] > mejorDist) {
      mejorDist = dist[idx];
      mejor = idx;
    }
  }

  // reconstruir camino
  std::vector<int> path;
  for (int v = mejor; v != -1; v = prev[v]) {
    path.push_back(v);
  }
  std::reverse(path.begin(), path.end());
    // Imprimir la ruta (vértices del grafo)
  std::cout << "La ruta es: ";
  for (std::size_t k = 0; k < path.size(); ++k) {
    int pi = path[k] / ancho;
    int pj = path[k] % ancho;
    // Imprimimos base y coordenadas
    std::cout << cod[path[k]] << " [" << pi << "," << pj << "]";
    if (k + 1 < path.size()) {
      std::cout << " -> ";
    }
  }
  std::cout << std::endl;

  int a = mejor / ancho;
  int b = mejor % ancho;

  std::cout << "Para la secuencia " << descripcion
            << ", la base remota de la base en [" << i << "," << j
            << "] está ubicada en [" << a << "," << b << "]"
            << " y la distancia mínima entre ambas es: " << mejorDist
            << ", mediante una ruta de " << path.size() << " pasos." << std::endl;
}
#endif // __GENOMA__HXX__
