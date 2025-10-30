#ifndef __SECUENCIA__HXX__
#define __SECUENCIA__HXX__

#include <string>

// Constructores
Secuencia::Secuencia() : descripcion_(), codigo_(), ancho_(0) {}
Secuencia::Secuencia(const std::string &desc, const std::string &cod, unsigned short ancho)
    : descripcion_(desc), codigo_(cod), ancho_(ancho) {}

// Accesores
const std::string &Secuencia::ObtenerDescripcion() const { return descripcion_; }
const std::string &Secuencia::ObtenerCodigo() const { return codigo_; }
unsigned short Secuencia::ObtenerAncho() const { return ancho_; }

// Métricas
bool Secuencia::EsCompleta() const
{
  return codigo_.find('-') == std::string::npos;
}
size_t Secuencia::ContarBases() const
{
  size_t c = 0;
  for (std::size_t i = 0; i < codigo_.size(); ++i)
  {
    if (codigo_[i] != '-')
    {
      ++c;
    }
  }
  return c;
}

// Acceso mutador (para enmascarar)
std::string &Secuencia::RefCodigo() { return codigo_; }

#endif // __SECUENCIA__HXX__
