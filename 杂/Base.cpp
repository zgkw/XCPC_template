/**
 * 1 MB = 1024 KB
 * 1 KB = 1024 B
 * 134'210'000    128
 * 262'144'000    256
 * 520'000'000    524
 * 1'030'000'000  1024
 * 注意事项：记得内存别开小了或者别爆了
 */

constexpr int max_size = 1030000000;
uint8_t buf[max_size];
uint8_t *head = buf;

using u32 = uint32_t;

template <class T>
struct Base {
    u32 x;
    Base(u32 x = 0) : x(x) {}
    T *operator->() {
        return (T *)(buf + x);
    }
    T &operator*() {
        return *((T *)(buf + x));
    }
    operator bool() {
        return x;
    }
    operator u32() {
        return x;
    }
    bool operator==(Base rhs) const {
        return x == rhs.x;
    }
    static Base news() {
        return (head += sizeof(T)) - buf;
    }
};