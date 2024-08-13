#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

#define N 50010
#define x first
#define y second
#define Point pair<int,int>
Point p[N],s[N];
int n;

int cross(Point a,Point b,Point c){ //叉积
  return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}
int dis(Point a, Point b){ //距离平方
  return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
}
void Andrew(){
  sort(p+1,p+n+1);
  int t=0;
  for(int i=1; i<=n; i++){ //下凸包
    while(t>1&&cross(s[t-1],s[t],p[i])<=0)t--;
    s[++t]=p[i];
  }
  int k=t;
  for(int i=n-1; i>=1; i--){ //上凸包
    while(t>k&&cross(s[t-1],s[t],p[i])<=0)t--;
    s[++t]=p[i];
  }
  n=t-1; //n为凸包上的点数
}
int rotating_calipers(){ //旋转卡壳
  int res=0;
  for(int i=1,j=2; i<=n; i++){
    while(cross(s[i],s[i+1],s[j])<cross(s[i],s[i+1],s[j+1]))j=j%n+1;
    res=max(res,max(dis(s[i],s[j]),dis(s[i+1],s[j])));
  }
  return res;
}
int main(){
  scanf("%d",&n);
  for(int i=1; i<=n; i++) scanf("%d%d",&p[i].x,&p[i].y);
  Andrew();
  printf("%d\n",rotating_calipers());
  return 0;
}
