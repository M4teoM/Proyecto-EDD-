#ifndef __GENOMA__HXX__
#define __GENOMA__HXX__

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <list>
#include <vector>
#include "Genoma.h"
#include "util.h"

// ----------------------
// Constructores / Estado
// ----------------------
Genoma::Genoma() : secuencias_() {}
Genoma::~Genoma() {}

bool Genoma::EstaVacio() const
{
  return secuencias_.empty();
}
size_t Genoma::Tam() const
{
  return secuencias_.size();
}

// ----------------------
// Cargar
// ----------------------

void Genoma::Cargar(const std::string &ruta)
{
  std::ifstream f(ruta.c_str());
  if (!f)
  {
    std::cout << ruta << " no se encuentra o no puede leerse." << std::endl;
    return;
  }

  std::list<Secuencia> nuevo;
  std::string line, desc, seq;
  unsigned short ancho = 0;
  bool enSec = false;
  bool invalida = false; // marca si la secuencia actual tiene símbolos no permitidos

  while (std::getline(f, line))
  {
    if (!line.empty() && line[0] == '>')
    {
      if (!desc.empty())
      {
        if (!invalida && !seq.empty())
        {
          nuevo.push_back(Secuencia(desc, seq, ancho));
        }
      }
      // Iniciar nueva secuencia
      desc = line.substr(1);
      seq.clear();
      ancho = 0;
      enSec = true;
      invalida = false;
    }
    else if (enSec)
    {
      bool linea_valida = true;
      std::string agregada;
      for (std::size_t i = 0; i < line.size(); ++i)
      {
        unsigned char cc = static_cast<unsigned char>(line[i]);
        if (std::isspace(cc))
          continue;
        char u = static_cast<char>(std::toupper(cc));
        if (ORDEN_FASTA.find(u) == std::string::npos)
        {
          linea_valida = false;
          break;
        }
        agregada.push_back(u);
      }
      if (!linea_valida)
      {
        invalida = true;
        continue;
      }
      if (ancho == 0)
      {
        ancho = static_cast<unsigned short>(agregada.size()); // ancho = largo de la primera línea válida
      }
      seq += agregada;
    }
  }

  // Cerrar última secuencia al EOF
  if (!desc.empty())
  {
    if (!invalida && !seq.empty())
    {
      nuevo.push_back(Secuencia(desc, seq, ancho));
    }
  }

  if (nuevo.empty())
  {
    std::cout << ruta << " no contiene ninguna secuencia." << std::endl;
  }
  else
  {
    secuencias_.swap(nuevo); // sobrescribe el contenido anterior
    size_t n = secuencias_.size();
    std::cout << n << " secuencia" << (n > 1 ? "s" : "") << " cargada" << (n > 1 ? "s" : "")
              << " correctamente desde " << ruta << "." << std::endl;
  }
}
// ----------------------
// ListarSecuencias
// ----------------------
void Genoma::ListarSecuencias() const
{
  if (EstaVacio())
  {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }

  std::cout << "Hay " << Tam() << " secuencias cargadas en memoria:" << std::endl;

  std::list<Secuencia>::const_iterator it = secuencias_.begin();
  for (; it != secuencias_.end(); ++it)
  {
    size_t b = it->ContarBases();
    if (it->EsCompleta())
    {
      std::cout << "Secuencia " << it->ObtenerDescripcion() << " contiene " << b << " bases." << std::endl;
    }
    else
    {
      std::cout << "Secuencia " << it->ObtenerDescripcion() << " contiene al menos " << b << " bases." << std::endl;
    }
  }
}

// ----------------------
// HistogramaDe
// ----------------------
void Genoma::HistogramaDe(const std::string &descripcion) const
{
  if (EstaVacio())
  {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }

  // Buscar por descripción
  std::list<Secuencia>::const_iterator it = secuencias_.begin();
  while (it != secuencias_.end() && it->ObtenerDescripcion() != descripcion)
  {
    ++it;
  }
  if (it == secuencias_.end())
  {
    std::cout << "Secuencia inválida." << std::endl;
    return;
  }

  Histograma h;
  const std::string &cod = it->ObtenerCodigo();
  for (std::size_t i = 0; i < cod.size(); ++i)
  {
    h.Acumular(cod[i]);
  }

  // Usa operator<< de Histograma para imprimir en orden FASTA (incluye '-')
  std::cout << h;
}

// ----------------------
// EsSubsecuencia
// ----------------------
void Genoma::EsSubsecuencia(const std::string &sub) const
{
  if (EstaVacio())
  {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }
  if (sub.empty())
  {
    std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria." << std::endl;
    return;
  }

  unsigned long long total = 0ULL;

  std::list<Secuencia>::const_iterator it = secuencias_.begin();
  for (; it != secuencias_.end(); ++it)
  {
    const std::string &cod = it->ObtenerCodigo();
    std::size_t pos = 0;
    while ((pos = cod.find(sub, pos)) != std::string::npos)
    {
      ++total;
      ++pos;
    }
  }

  if (total == 0ULL)
  {
    std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria." << std::endl;
  }
  else
  {
    std::cout << "La subsecuencia dada se repite " << total
              << " veces dentro de las secuencias cargadas en memoria." << std::endl;
  }
}

// ----------------------
// Enmascarar
// ----------------------
void Genoma::Enmascarar(const std::string &sub)
{
  if (EstaVacio())
  {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }
  if (sub.empty())
  {
    std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria, por tanto no se enmascara nada." << std::endl;
    return;
  }

  unsigned long long s = 0ULL;

  std::list<Secuencia>::iterator it = secuencias_.begin();
  for (; it != secuencias_.end(); ++it)
  {
    std::string &cod = it->RefCodigo();
    std::size_t pos = 0;
    while ((pos = cod.find(sub, pos)) != std::string::npos)
    {
      // Reemplazar por 'X' manteniendo longitud
      for (std::size_t k = 0; k < sub.size(); ++k)
      {
        cod[pos + k] = 'X';
      }
      ++s;
      ++pos; // avanzar uno para permitir superposición
    }
  }

  if (s == 0ULL)
  {
    std::cout << "La subsecuencia dada no existe dentro de las secuencias cargadas en memoria, por tanto no se enmascara nada." << std::endl;
  }
  else
  {
    std::cout << s << " subsecuencias han sido enmascaradas dentro de las secuencias cargadas en memoria." << std::endl;
  }
}

// ----------------------
// Guardar
// ----------------------
void Genoma::Guardar(const std::string &ruta) const
{
  if (EstaVacio())
  {
    std::cout << "No hay secuencias cargadas en memoria." << std::endl;
    return;
  }
  std::ofstream out(ruta);
  if (!out)
  {
    std::cout << "Error guardando en " << ruta << "." << std::endl;
    return;
  }

  std::list<Secuencia>::const_iterator it = secuencias_.begin();
  for (; it != secuencias_.end(); ++it)
  {
    out << ">" << it->ObtenerDescripcion() << "\n";
    unsigned short w = (it->ObtenerAncho() == 0 ? 60 : it->ObtenerAncho()); // el ? es similar a un if, si es 0 se pone 60, si no se pone el ancho
    const std::vector<std::string> lineas = PartirPorAncho(it->ObtenerCodigo(), w);
    for (std::size_t i = 0; i < lineas.size(); ++i)
    {
      out << lineas[i] << "\n";
    }
  }
  std::cout << "Las secuencias han sido guardadas en " << ruta << "." << std::endl;
}

#endif // __GENOMA__HXX__
