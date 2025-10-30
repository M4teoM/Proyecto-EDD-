#ifndef __FABIN_HXX__
#define __FABIN_HXX__

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map> //agregado para usar std::unordered_map
#include <istream>
#include <ostream>
#include <stdexcept> //agregado para usar std::runtime_error
#include <utility> //agregado para usar std::pair

#include "BitIO.hxx"
#include "Huffman.hxx"

struct SeqRecord
{
    std::string name;
    std::string code;
    std::uint16_t width;
};

inline std::unordered_map<char, std::uint64_t> frequencies(const std::vector<SeqRecord> &seqs)
{
    std::unordered_map<char, std::uint64_t> f;
    for (const SeqRecord &s : seqs)
    {
        for (char c : s.code)
        {
            f[c]++;
        }
    }
    return f;
}

inline void write_fabin(std::ostream &os, const std::vector<SeqRecord> &seqs)
{
    std::unordered_map<char, std::uint64_t> f = frequencies(seqs);

    std::uint16_t n = 0;
    for (const std::pair<const char, std::uint64_t> &kv : f)
    {
        if (kv.second > 0) n++;
    }
    write_le<std::uint16_t>(os, n);

    for (const std::pair<const char, std::uint64_t> &kv : f)
    {
        if (kv.second == 0) continue;
        char ci = kv.first;
        std::uint64_t fi = kv.second;
        os.write(&ci, 1);
        write_le<std::uint64_t>(os, fi);
    }

    std::uint32_t ns = static_cast<std::uint32_t>(seqs.size());
    write_le<std::uint32_t>(os, ns);

    for (const SeqRecord &s : seqs)
    {
        std::uint16_t li = static_cast<std::uint16_t>(s.name.size());
        write_le<std::uint16_t>(os, li);
        os.write(s.name.data(), li);
    }

    std::shared_ptr<HuffNode> root = build_huffman(f);
    if (!root) return;

    std::unordered_map<char, std::vector<bool>> cmap;
    std::vector<bool> tmp;
    build_code_map(root, cmap, tmp);

    for (const SeqRecord &s : seqs)
    {
        std::uint64_t wi = static_cast<std::uint64_t>(s.code.size());
        std::uint16_t xi = s.width;
        write_le<std::uint64_t>(os, wi);
        write_le<std::uint16_t>(os, xi);

        BitWriter bw;
        for (char c : s.code)
        {
            std::unordered_map<char, std::vector<bool>>::const_iterator it = cmap.find(c);
            if (it == cmap.end())
                throw std::runtime_error("symbol missing");
            bw.put_bits(it->second);
        }
        bw.flush_byte_align();
        bw.write_to(os);
    }
}

inline std::vector<SeqRecord> read_fabin(std::istream &is)
{
    std::vector<SeqRecord> out;

    std::uint16_t n = read_le<std::uint16_t>(is);
    if (n == 0) return out;

    std::unordered_map<char, std::uint64_t> f;
    for (std::uint16_t i = 0; i < n; ++i)
    {
        char ci = 0;
        if (!is.read(&ci, 1))
            throw std::runtime_error("EOF ci");
        std::uint64_t fi = read_le<std::uint64_t>(is);
        f[ci] = fi;
    }

    std::shared_ptr<HuffNode> root = build_huffman(f);
    if (!root) return out;

    std::uint32_t ns = read_le<std::uint32_t>(is);

    std::vector<std::string> names;
    names.reserve(static_cast<std::size_t>(ns));
    for (std::uint32_t i = 0; i < ns; ++i)
    {
        std::uint16_t li = read_le<std::uint16_t>(is);
        std::string name(li, '\0');
        if (li) is.read(&name[0], li);
        names.push_back(std::move(name));
    }

    out.resize(static_cast<std::size_t>(ns));

    for (std::uint32_t i = 0; i < ns; ++i)
    {
        std::uint64_t wi = read_le<std::uint64_t>(is);
        std::uint16_t xi = read_le<std::uint16_t>(is);

        std::string code;
        code.reserve(static_cast<std::size_t>(wi));

        if (!root->left && !root->right)
        {
            char sym = root->symbol;
            code.assign(static_cast<std::size_t>(wi), sym);
        }
        else
        {
            BitReader br(is);
            for (std::uint64_t k = 0; k < wi; ++k)
            {
                std::shared_ptr<HuffNode> cur = root;
                while (!cur->is_leaf())
                {
                    bool bit = br.get_bit();
                    cur = bit ? cur->right : cur->left;
                    if (!cur) throw std::runtime_error("invalid path");
                }
                code.push_back(cur->symbol);
            }
            br.discard_to_byte();
        }

        out[static_cast<std::size_t>(i)].name  = std::move(names[static_cast<std::size_t>(i)]);
        out[static_cast<std::size_t>(i)].code  = std::move(code);
        out[static_cast<std::size_t>(i)].width = xi;
    }

    return out;
}
#endif
