struct node;
using Trie = Base<node>;

struct node {
    array<Trie, 2> ch{};
    int x; int sum;
};

Trie news() {
    return Trie::news();
} 