#ifndef MERKLE_TREE_HPP
#define MERKLE_TREE_HPP

#include <string>
#include <vector>
#include "picosha2.h"

class MerkleTree {
public:
    MerkleTree(const std::vector<std::string>& leaf_hashes) {
        buildTree(leaf_hashes);
    }

    std::string getRoot() const {
        return !tree.empty() ? tree.back().front() : "";
    }

private:
    std::vector<std::vector<std::string>> tree;

    void buildTree(std::vector<std::string> leaves) {
        if (leaves.empty()) return;

        tree.push_back(leaves);
        while (tree.back().size() > 1) {
            const auto& prev_level = tree.back();
            std::vector<std::string> new_level;

            for (size_t i = 0; i < prev_level.size(); i += 2) {
                if (i + 1 < prev_level.size()) {
                    std::string combined = prev_level[i] + prev_level[i + 1];
                    new_level.push_back(picosha2::hash256_hex_string(combined));
                } else {
                    // Si hay un nodo sin par, se duplica
                    new_level.push_back(prev_level[i]);
                }
            }
            tree.push_back(new_level);
        }
    }
};

#endif // MERKLE_TREE_HPP
