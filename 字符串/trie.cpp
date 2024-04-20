constexpr int N = 2e5;

struct node{
    int _ch[2];
    int R = -1, L = -1;
    node *ch(int x) const;
    void _new(int x);
    bool is_null();
    bool not_null();
};
node B[N * 30];
int idx = 1;
node *node::ch(int x) const {
    return B + _ch[x];
}
void node::_new(int x) {
    _ch[x] = ++ idx;
}
bool node::is_null() {
    return this == B;
}
bool node::not_null() {
    return this != B;
}