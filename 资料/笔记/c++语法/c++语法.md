# 库函数

`lower_bound` 中使用匿名函数重载

实例：

```c++
auto ro = lower_bound(ord.begin(), ord.end(), m, 
    [&] (int i, int m) {
        return a[i] < m;
    });
```

