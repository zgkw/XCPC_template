本类模板主要是`set`附加了`kth`的功能，封装较好，不支持区间修改，胜在代码量较小，其中常数最小的是`FHQ_treap`

操作有

1. `insert`
2. `erase` ：没有会 `RE`
3. `less` ：返回小于目标的元素的数目
4. `operator[]` ：从下标 0 开始，越界会 `RE`
5. `next` ：返回元素的后继，但是代码是面向树套树写的默认状态下 `default` 是 `std::numeric_limits<int>::max()`
6. `prev` ：返回元素的前驱，同 `next` 
7. `dfs` ：输出整个树

ps : `RE` 不是我写的 `assert` 汇报，会发生无限循环之类的错误

替罪羊树需要自己写 `next` 和 `prev` 
