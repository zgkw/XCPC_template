struct node;
using Trie = u32_p<node>;

struct node {
    array<Trie, 2> ch{};
    int x; int sum;
};

Trie _new() {
    return Trie::__new();
} 