#ifndef UTILITIES.HPP
#define UTILITIES.HPP

#include <unordered_map>
#include <utility>

template <typename T>
class disjoint_Set {
private:
    struct Node {
        T parent;
        std::size_t rank{0};
    };

    mutable std::unordered_map<T, Node> nodes;

public:
    DisjointSet() = default;

    void make_set(const T& item) {
        if (nodes.find(item) == nodes.end()) {
            nodes[item] = {item, 0};
        }
    }

    T find(const T& item) const {
        auto it = nodes.find(item);
        if (it == nodes.end()) {
            nodes[item] = {item, 0};
            return item;
        }

        if (it->second.parent != item) {
            it->second.parent = find(it->second.parent);
        }
        return it->second.parent;
    }

    bool unite(const T& item1, const T& item2) {
        T root1 = find(item1);
        T root2 = find(item2);

        if (root1 == root2) {
            return false;
        }

        auto& node1 = nodes[root1];
        auto& node2 = nodes[root2];

        if (node1.rank < node2.rank) {
            node1.parent = root2;
        } else if (node1.rank > node2.rank) {
            node2.parent = root1;
        } else {
            node2.parent = root1;
            node1.rank++;
        }

        return true;
    }

    bool connected(const T& item1, const T& item2) const {
        return find(item1) == find(item2);
    }
};

#endif // UTILITIES.HPP