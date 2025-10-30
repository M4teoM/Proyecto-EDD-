#ifndef __SECUENCIA__H__
#define __SECUENCIA__H__
#include <string>

class Secuencia
{
public:
  Secuencia();
  Secuencia(const std::string &desc, const std::string &cod, unsigned short ancho);
  const std::string &ObtenerDescripcion() const;
  const std::string &ObtenerCodigo() const;
  unsigned short ObtenerAncho() const;
  bool EsCompleta() const;
  size_t ContarBases() const;
  std::string &RefCodigo();

private:
  std::string descripcion_;
  std::string codigo_;
  unsigned short ancho_;
};
#endif
