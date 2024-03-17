template <int Lim>
struct Linear_Base{
  int a[Lim + 1] , n ;
  Linear_Base() {
    n = Lim ;
    memset(a,0,sizeof a);
  }
  void insert(int x){//插入
    for(int i=Lim;i>=0;i--)if(x&(1ll<<i)){
      if(!a[i]){a[i]=x;return;}
      else x^=a[i];
  }
    }
  bool check(int x){//查询x是否能被异或出来
    for(int i=Lim;i>=0;i--)if(x&(1ll<<i)){
      if(!a[i])break;
      x^=a[i];
    }
    return x==0;
  }
  int querymax(int res){//查询最大异或和
    for(int i=Lim;i>=0;i--)if((res^a[i])>res)res^=a[i];
    return res;
  }
  int querymin(int res){//查询最小
    for(int i=Lim;i>=0;i--)if(res&(1ll<<i))res^=a[i];
    return res;
  }
  int querykth(int k){//查询第k大的异或和
    int tmp[Lim+10],res=0,cnt=0;
    for(int i=0;i<=Lim;i++){
      for(int j=i-1;j>=0;j--)if(a[i]&(1ll<<j))a[i]^=a[j];
      if(a[i])tmp[cnt++]=a[i];
    }
    for(int i=0;i<cnt;i++)if(k&(1ll<<i))res^=tmp[i];
    return res;
  }
  void merge(const Linear_Base &other)//合并
  {for(int i=0;i<=Lim;i++) insert(other.a[i]);}
};
