#ifndef __HUFFMAN_HXX__
#define __HUFFMAN_HXX__
#include <cstdint>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

struct HuffNode
{
    char symbol;
    std::uint64_t freq;
    std::shared_ptr<HuffNode> left, right;

    HuffNode(char s, std::uint64_t f) : symbol(s), freq(f), left(nullptr), right(nullptr) {}
    HuffNode(std::shared_ptr<HuffNode> l, std::shared_ptr<HuffNode> r)
        : symbol('\0'), freq((l ? l->freq : 0) + (r ? r->freq : 0)), left(l), right(r) {}

    bool is_leaf() const { return !left && !right; }
};

struct HuffCmp
{
    bool operator()(const std::shared_ptr<HuffNode> &a, const std::shared_ptr<HuffNode> &b) const
    {
        if (a->freq != b->freq) return a->freq > b->freq;
        if (a->is_leaf() && b->is_leaf()) return a->symbol > b->symbol;
        if (a->is_leaf() != b->is_leaf()) return !a->is_leaf();
        return false;
    }
};

inline std::shared_ptr<HuffNode> build_huffman(const std::unordered_map<char, std::uint64_t> &f)
{
    std::priority_queue<std::shared_ptr<HuffNode>, std::vector<std::shared_ptr<HuffNode>>, HuffCmp> pq;

    for (const std::pair<const char, std::uint64_t> &kv : f) {
        if (kv.second > 0) {
            pq.push(std::make_shared<HuffNode>(kv.first, kv.second));
        }
    }

    if (pq.empty()) return nullptr;

    if (pq.size() == 1)
    {
        std::shared_ptr<HuffNode> only = pq.top();
        pq.pop();
        std::shared_ptr<HuffNode> dup = std::make_shared<HuffNode>(only->symbol, only->freq);
        return std::make_shared<HuffNode>(only, dup);
    }

    while (pq.size() > 1)
    {
        std::shared_ptr<HuffNode> a = pq.top();
        pq.pop();
        std::shared_ptr<HuffNode> b = pq.top();
        pq.pop();
        pq.push(std::make_shared<HuffNode>(a, b));
    }

    return pq.top();
}

inline void build_code_map(const std::shared_ptr<HuffNode> &n,std::unordered_map<char, std::vector<bool>> &out,std::vector<bool> &path)
{
    if (!n) return;

    if (n->is_leaf())
    {
        if (path.empty()) path.push_back(false);
        out[n->symbol] = path;
        return;
    }

    path.push_back(false);
    build_code_map(n->left, out, path);
    path.back() = true;
    build_code_map(n->right, out, path);
    path.pop_back();
}
#endif
