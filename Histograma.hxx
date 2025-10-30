#ifndef __HISTOGRAMA__HXX__
#define __HISTOGRAMA__HXX__

#include "Histograma.h"
#include "util.h"
#include <ostream>

Histograma::Histograma() : freq_(ORDEN_FASTA.size(), 0ULL) {}

void Histograma::Vaciar()
{
  for (std::size_t i = 0; i < freq_.size(); ++i)
  {
    freq_[i] = 0ULL;
  }
}

void Histograma::Acumular(char c)
{
  std::size_t pos = ORDEN_FASTA.find(c);
  if (pos != std::string::npos)
  {
    freq_[pos] += 1ULL;
  }
}

unsigned long long Histograma::Frecuencia(char c) const
{
  std::size_t pos = ORDEN_FASTA.find(c);
  if (pos != std::string::npos)
  {
    return freq_[pos];
  }
  return 0ULL;
}

std::ostream& operator<<(std::ostream& os, const Histograma& h) {
    for (std::size_t i = 0; i < ORDEN_FASTA.size(); ++i) {
        std::uint64_t cnt = h.freq_[i];
        if (cnt == 0ULL) continue;
        os << ORDEN_FASTA[i] << " : " << cnt << '\n';
    }
    return os;
}



#endif // __HISTOGRAMA__HXX__
