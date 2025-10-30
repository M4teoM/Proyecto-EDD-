#ifndef __GENOMA_PART2_HXX__
#define __GENOMA_PART2_HXX__
#include <fstream>
#include <vector>
#include <list>
#include <iostream>
#include "Fabin.hxx"
#include "Genoma.h"

void Genoma::Codificar(const std::string &ruta_fabin) const
{
    if (EstaVacio())
    {
        std::cout << "No hay secuencias cargadas en memoria." << std::endl;
        return;
    }
    try
    {
        std::ofstream os(ruta_fabin.c_str(), std::ios::binary);
        if (!os)
        {
            std::cout << "No se pueden guardar las secuencias cargadas en " << ruta_fabin << " ." << std::endl;
            return;
        }

        std::vector<SeqRecord> seqs;
        seqs.reserve(static_cast<std::size_t>(secuencias_.size()));

        
        std::list<Secuencia>::const_iterator it = secuencias_.begin();
        for (; it != secuencias_.end(); ++it)
        {
            const Secuencia &s = *it;
            seqs.push_back(SeqRecord{ s.ObtenerDescripcion(), s.ObtenerCodigo(), s.ObtenerAncho() });
        }

        write_fabin(os, seqs);
        std::cout << "Secuencias codificadas y almacenadas en " << ruta_fabin << " ." << std::endl;
    }
    catch (...)
    {
        std::cout << "No se pueden guardar las secuencias cargadas en " << ruta_fabin << " ." << std::endl;
    }
}

void Genoma::Decodificar(const std::string &ruta_fabin)
{
    try
    {
        std::ifstream is(ruta_fabin.c_str(), std::ios::binary);
        if (!is)
        {
            std::cout << "No se pueden cargar las secuencias desde " << ruta_fabin << " ." << std::endl;
            return;
        }


        std::vector<SeqRecord> seqs = read_fabin(is);

        secuencias_.clear();


        std::vector<SeqRecord>::iterator it = seqs.begin();
        for (; it != seqs.end(); ++it)
        {
            SeqRecord &r = *it;
            secuencias_.push_back(Secuencia(r.name, r.code, r.width));
        }

        std::cout << "Secuencias decodificadas desde " << ruta_fabin << " y cargadas en memoria." << std::endl;
    }
    catch (...)
    {
        std::cout << "No se pueden cargar las secuencias desde " << ruta_fabin << " ." << std::endl;
    }
}
#endif
