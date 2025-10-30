#ifndef __UTIL__H__
#define __UTIL__H__
#include <string>
#include <vector>

static const std::string ORDEN_FASTA = "ACGTURYKMSWBDHVNX-";

inline std::vector<std::string> PartirPorAncho(const std::string& s, unsigned short w) {
    std::vector<std::string> out;
    if (w == 0) return out;
    for (std::size_t i = 0; i < s.size(); i += w) {
        std::size_t len = (i + w <= s.size() ? w : s.size() - i);
        out.push_back(s.substr(i, len));
    }
    return out;
}
#endif
