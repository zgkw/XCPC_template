#include <iostream>
#include <cstdio>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;
int main()
{
    int ok = 0;
    int n = 50;
    for (int i = 1;; ++i) 
    {
        system("data.exe > in.txt");
        system("std.exe < in.txt > std.txt");
        double begin = clock();
        system("baoli.exe < in.txt > baoli.txt");
        double end = clock();

        double t = (end - begin);
        if (system("fc std.txt baoli.txt"))
        {
            printf("测试点#%d Wrong Answer\n", i);
            system("pause");
        }
        else if (t > 2000) //1秒
        {
            printf("测试点#%d Time Limited Exceeded 用时 %.0lfms\n", i, t);
        }
        else
        {
            printf("测试点#%d Accepted 用时%.0lfms\n", i, t);
            ok++; //AC数量+1
        }
    }
    printf("\n");
    double res = 100.0 * ok / n;
    printf("共 %d 组测试数据，AC数据 %d 组。 得分%.1lf。", n, ok, res);
}
