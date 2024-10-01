#include <bits/stdc++.h>
#define ll long long
#define INF 0x3f3f3f3f
#define LINF 0x3f3f3f3f3f3f3f3f
#define mod 1000000007 //998244353.1000000007
#define maxn 200005
using namespace std;

int R(int l,int r){
    random_device seed;//硬件生成随机数种子
	ranlux48 engine(seed());//利用种子生成随机数引擎
    uniform_int_distribution<> Rand(l, r);//设置随机数范围，并为均匀分布
    return Rand(engine);
}

void chmn(int& a,int b,int& p,int f){
    if(b<=a){
        a=b;
        p=f;
    } 
}

void chmx(int& a,int b,int& p,int f,int& pp,int ff){
    if(b>a){
        a=b;
        p=f;
        pp=ff;
    }
}

char op[4]={'A','T','C','G'};

string t[10]={"CAGAAC",
        "GAGGGT",
        "GGAGGC",
        "TCCCCG",
        "GTTTGG",
        "AATGGA",
        "TTCCTA",
        "AGCTAC",
        "CCCGTG",
        "CCAGCG"};
map <char,int> mp;

int s[6][11];
int dp[11][11][11][11][11][11]; // 想想状压  update:感觉没必要 状压之后空间反而大了 反正6条序列 手写么好了 顶多改个dfs
int pre[11][11][11][11][11][11];

// 1248    4b*6=24b 1<<24=1e7
/*
dp[i][j]=max(dp[i][j],dp[i-py][j-py]+1)
*/
int dpp[11][11][11][11][11][11];
int pree[11][11][11][11][11][11],prest[11][11][11][11][11][11];

void solve(){
    int m=10;
    // for(int i=0;i<6;i++){
    //     for(int j=1;j<=10;j++){
    //         s[i][j]=R(0,3);
    //     }
    // }
    for(int i=0;i<4;i++){
        mp[op[i]]=i;
    }

    // transpose
    for(int i=0;i<6;i++){
        for(int j=1;j<=m;j++){
            s[i][j]=mp[t[m-j][i]];
        }
    }    
    // freopen("in.in","r",stdin);
    // m=10;

    for(int i=0;i<6;i++){
        string t;
        cin>>t;
        for(int j=1;j<=m;j++){
            // cin>>s[i][j];
            s[i][j]=t[j-1]-'0';
        }
    }
    for(int i=0;i<6;i++){
        for(int j=1;j<=m;j++){
            cout<<s[i][j];
        }
        cout<<endl;
    }

    // dp 求最小步数
    memset(dp,0x3f,sizeof dp);
    dp[0][0][0][0][0][0]=0;

    for(int i1=0;i1<=m;i1++){
        for(int i2=0;i2<=m;i2++){
            for(int i3=0;i3<=m;i3++){
                for(int i4=0;i4<=m;i4++){
                    for(int i5=0;i5<=m;i5++){
                        for(int i6=0;i6<=m;i6++){
                            for(int f=0;f<4;f++){
                                vector <int> py(6,0);

                                if(i1 && s[0][i1]==f) py[0]++;
                                if(i2 && s[1][i2]==f) py[1]++;
                                if(i3 && s[2][i3]==f) py[2]++;
                                if(i4 && s[3][i4]==f) py[3]++;
                                if(i5 && s[4][i5]==f) py[4]++;
                                if(i6 && s[5][i6]==f) py[5]++;
                                int sum=0;
                                for(int i=0;i<6;i++) sum+=py[i];

                                chmn(dp[i1][i2][i3][i4][i5][i6],dp[i1-py[0]][i2-py[1]][i3-py[2]][i4-py[3]][i5-py[4]][i6-py[5]]+1,pre[i1][i2][i3][i4][i5][i6],f);
                            }
                        }
                    }
                }
            }
        }
    }

    vector <int> ops;
    vector <int> id(6,m);
    while(1){
        int f=0;
        for(int i=0;i<6;i++) if(id[i]!=0) f=1;
        if(!f) break;

        int op=pre[id[0]][id[1]][id[2]][id[3]][id[4]][id[5]];
        ops.push_back(op);

        vector <int> py(6,0);

        if(id[0] && s[0][id[0]]==op) py[0]++;
        if(id[1] && s[1][id[1]]==op) py[1]++;
        if(id[2] && s[2][id[2]]==op) py[2]++;
        if(id[3] && s[3][id[3]]==op) py[3]++;
        if(id[4] && s[4][id[4]]==op) py[4]++;
        if(id[5] && s[5][id[5]]==op) py[5]++;
        
        for(int i=0;i<6;i++) id[i]-=py[i];
    }
    reverse(ops.begin(),ops.end());
    cout<<ops.size()<<endl;
    for(auto i:ops) cout<<i<<" ";cout<<endl;
    

    ///这里开始求最大步数

    memset(dpp,-0x3f,sizeof dpp);
    dpp[0][0][0][0][0][0]=0;

    for(int i1=0;i1<=m;i1++){
        for(int i2=0;i2<=m;i2++){
            for(int i3=0;i3<=m;i3++){
                for(int i4=0;i4<=m;i4++){
                    for(int i5=0;i5<=m;i5++){
                        for(int i6=0;i6<=m;i6++){
                            for(int f=0;f<4;f++){
                                vector <int> py(6,0);

                                if(i1<m && s[0][i1+1]==f) py[0]++;
                                if(i2<m && s[1][i2+1]==f) py[1]++;
                                if(i3<m && s[2][i3+1]==f) py[2]++;
                                if(i4<m && s[3][i4+1]==f) py[3]++;
                                if(i5<m && s[4][i5+1]==f) py[4]++;
                                if(i6<m && s[5][i6+1]==f) py[5]++;
                                int sum=0;
                                for(int i=0;i<6;i++) sum+=py[i];
                                
                                int st=0;
                                for(int i=0;i<6;i++) st|=(py[i]<<i);
                                
                                if(sum>0) chmx(dpp[i1+py[0]][i2+py[1]][i3+py[2]][i4+py[3]][i5+py[4]][i6+py[5]]
                                              ,dpp[i1][i2][i3][i4][i5][i6]+1
                                              ,pree[i1+py[0]][i2+py[1]][i3+py[2]][i4+py[3]][i5+py[4]][i6+py[5]],f
                                              ,prest[i1+py[0]][i2+py[1]][i3+py[2]][i4+py[3]][i5+py[4]][i6+py[5]],st);
                            }
                        }
                    }
                }
            }
        }
    }

    ops.clear();
    for(int i=0;i<6;i++) id[i]=m;
    while(1){
        // for(int i=0;i<6;i++) cerr<<id[i]<<" ";cerr<<endl;

        int f=0;
        for(int i=0;i<6;i++) if(id[i]!=0) f=1;
        if(!f) break;

        int op=pree[id[0]][id[1]][id[2]][id[3]][id[4]][id[5]];
        int st=prest[id[0]][id[1]][id[2]][id[3]][id[4]][id[5]];

        ops.push_back(op);

        vector <int> py(6,0);
        for(int i=0;i<6;i++){
            if(st&(1<<i)) py[i]=1;
        }
        
        for(int i=0;i<6;i++) id[i]-=py[i];
    }
    reverse(ops.begin(),ops.end());
    cout<<ops.size()<<endl;
    for(auto i:ops) cout<<i<<" ";cout<<endl;
}

/*

*/
void init(){
	
}
signed main(){
#ifdef LOCAL
	freopen("in.in","r",stdin);
	freopen("out.out","w",stdout);
#endif
	ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);int ca=1;clock_t c1=clock();
	init();
	// cin>>ca;
	while(ca--){
		solve();
	}
	// cerr<<"Time used:"<<1.0*(clock()-c1)/CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}
