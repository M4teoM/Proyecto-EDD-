#ifndef __GENOMA__H__
#define __GENOMA__H__
#include <list>
#include <string>
#include <cstddef>
#include <iostream>

#include "util.h"        // ORDEN_FASTA, PartirPorAncho
#include "Secuencia.h"    // NECESARIO Genoma usa Secuencia en std::list<Secuencia>
#include "Histograma.h"   // Genoma usa Histograma en HistogramaDe

class Genoma {
public:
  Genoma();
  ~Genoma();

  bool EstaVacio() const;
  size_t Tam() const;
  void Cargar(const std::string& ruta);
  void ListarSecuencias() const;
  void HistogramaDe(const std::string& descripcion) const;
  void EsSubsecuencia(const std::string& sub) const;
  void Enmascarar(const std::string& sub);
  void Guardar(const std::string& ruta) const;
  //implementacion 2da entrega 
  void Codificar(const std::string& nombre_archivo_fabin) const;
  void Decodificar(const std::string& nombre_archivo_fabin);
  //implementacion 3ra entrega
    // componente 3
  void RutaMasCorta(const std::string& descripcion, int i, int j, int x, int y) const;
  void BaseRemota(const std::string& descripcion, int i, int j) const;

private:
  std::list<Secuencia> secuencias_;
};
#endif
