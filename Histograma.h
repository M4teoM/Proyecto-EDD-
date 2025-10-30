#ifndef __HISTOGRAMA__H__
#define __HISTOGRAMA__H__
#include <vector>
#include <string>
#include <ostream>

class Histograma
{
  friend std::ostream &operator<<(std::ostream &os, const Histograma &h);

public:
  Histograma();
  void Vaciar();
  void Acumular(char c);
  unsigned long long Frecuencia(char c) const;

private:
  std::vector<unsigned long long> freq_;
};
std::ostream &operator<<(std::ostream &os, const Histograma &h);
#endif
