#include <bits/stdc++.h>
#include <random>
#define maxn 200005
using namespace std;

void printGraph();
bool endState(vector <int>& nowST);

int R(int l,int r){
    random_device seed;//硬件生成随机数种子
	ranlux48 engine(seed());//利用种子生成随机数引擎
    uniform_int_distribution<> Rand(l, r);//设置随机数范围，并为均匀分布
    return Rand(engine);
}

int action[4]={0,1,2,3};
int actionNum=4;

int strLen=10;
int strNum=6;

vector <vector<int>> Graph;
vector <int> nowState;

// 定义树上节点的状态
struct State{
    vector <int> q;
};

// 定义树的结点
struct Node{
    // level 是树的深度 
    // visit 是节点访问次数
    // fa 是节点的 father
    // value 是节点的价值
    int level,visit,fa;
    double value;
    bool terminal;

    State state;
    vector <int> child;
};

int nodeNum=0;

// 选择最优的儿子进行搜索
int UCBselect(int rt,vector <Node>& node){
    double maxUCB=-1e10;
    int maxchild=-1;

    int sumN=0; // 计算总次数
    for(int i=0;i<actionNum;i++){
        int v=node[rt].child[i];
        sumN+=node[v].visit;
    }
    
    vector <int> cnt(actionNum,0);  //统计四个字符的个数
    vector <int> len(actionNum,0);  //统计四个操作的总长
    int tot=0;  // 当前剩余序列个数 

    for(int j=0;j<strNum;j++){
        if(node[rt].state.q[j]<strLen) {
            tot++;
            int head=node[rt].state.q[j];
            cnt[Graph[j][head]]++;
            len[Graph[j][head]]+=strLen-head;
        } 
    }
    int totlen=len[0]+len[1]+len[2]+len[3];

    for(int i=0;i<actionNum;i++) if(len[i]){
        int v=node[rt].child[i];

        double P;
        P=1.0*cnt[i]/tot;

        double UCB=node[v].value/(1+node[v].visit)+(P*0.5+len[i]/totlen)*sqrt(2.0*sumN+1)/(1+node[v].visit);  //// 
        if(UCB>maxUCB){
            maxUCB=UCB;
            maxchild=v;
        }
    }
    return maxchild;
}

// 根据UCB select一个节点
int select(int rt,vector <Node>& node){
    while(1){
        if(node[rt].child.size()==0) return rt;
        rt=UCBselect(rt,node);
    }
}

// expand 一个节点的所有儿子（操作）
void expend(int rt,vector <Node>& node){
    node[rt].child.resize(actionNum);
    // cerr<<nodeNum<<endl;
    for(int i=0;i<actionNum;i++){
        node[rt].child[i]=nodeNum;
        node[nodeNum].visit=0;
        node[nodeNum].value=0;
        node[nodeNum].level=node[rt].level+1;
        node[nodeNum].state=node[rt].state;

        for(int j=0;j<strNum;j++){
            int head=node[nodeNum].state.q[j];
            if( head<strLen &&  Graph[j][head]==i)  node[nodeNum].state.q[j]++; 
        }
        node[nodeNum].terminal=endState(node[nodeNum].state.q);
        node[nodeNum].fa=rt;
        nodeNum++;
    }
}

// back propagation 将结果反馈到rt
void bp(int rt,double r,vector<Node>& node){
    while(rt!=0){
        node[rt].visit++;
        node[rt].value+=r;
        rt=node[rt].fa;
    }
    node[rt].visit++;
    node[rt].value+=r;
}

// // rollout 在这个节点随机模拟 直到结束 统计结果
void rollout(int rt,vector<Node>& node){
    // bp(rt,1,node);return ;

    vector <int> q=node[rt].state.q;

    int cnt=node[rt].level;
    int tot=0;
    for(int i=0;i<strNum;i++) tot+=strLen-q[i];

    while(tot){
        // 先随机再判断有没有 好像比 在有的里面随机更快
        int op=R(0,3);
        int have=0;
        for(int i=0;i<strNum;i++) if(q[i]<strLen && Graph[i][q[i]]==op) have=1;

        // map <int,int> mp;
        // for(int i=0;i<strNum;i++) if(q[i]<strLen){
        //     mp[Graph[i][q[i]]]=1;
        // }
        // int sz=mp.size(),szz=0;
        // vector <int> mpp(sz);
        // for(auto [x,y]:mp){
        //     mpp[szz]=x;
        //     szz++;
        // }
        // int op=mpp[R(0,sz-1)];

        if(have){
            cnt++;
            for(int i=0;i<strNum;i++){
                if(q[i]<strLen && Graph[i][q[i]]==op) q[i]++,tot--;
            }
        }
        // cerr<<op<<" "<<tot<<endl;
    }
    
    // double r=1.0*strLen/cnt;
    double r=(27-cnt);                        ////
    bp(rt,r,node);
}

// 一次模拟：
// 首先通过UCB select一个节点
// 如果这个点没有vis过 那么rollout
// 否则expand这个点 然后对拓展出的第一个点rollout

void sim_once(int rt,vector <Node>& node){
    // cerr<<1<<endl;
    int now=select(rt,node);
    // cerr<<2<<endl;
    // rollout(now,node);
    // cerr<<3<<endl;
    // return ;

    if(node[now].terminal) rollout(now,node);
    else if(node[now].visit==0) rollout(now,node);
    else{
        expend(now,node);
        int Bestchild=UCBselect(now,node);
        rollout(Bestchild,node);
    }
}

void GenerateGraph(vector <queue<int>>& Graph){
    Graph.resize(strNum);
    for(int i=0;i<strNum;i++){
        for(int j=0;j<strLen;j++){
            Graph[i].push(R(0,3));
        }
    }
}

void ReadGraph(){
    Graph.resize(strNum);
    freopen("in.in","r",stdin);

    for(int i=0;i<strNum;i++){
        string s;
        cin>>s;
        for(int j=0;j<strLen;j++){
            Graph[i].push_back(s[j]-'0');
        }
    }
}

void printGraph(){
    for(int i=0;i<strNum;i++){
        for(int j=nowState[i];j<strLen;j++){
            cout<<Graph[i][j];
        }
        cout<<endl;
    }
}

bool endState(vector <int>& nowST){
    for(int i=0;i<strNum;i++){
        if(nowST[i]<strLen) return false;
    }
    return true;
}

int MCTS(){
    vector <Node> node;
    nodeNum=0;
    int sim=100000;  //每轮 运行时间的决定因素 改大可以增加赢的概率 但是会慢 感觉1e5可以接受
    node.resize(5*sim);

    int rt=0;
    node[rt].state.q=nowState;
    nodeNum++;
    expend(rt,node);
    
    for(int i=1;i<=sim;i++){
        // cerr<<"i:"<<" "<<i<<endl;
        sim_once(rt,node);
    }    
    int op=-1;
    int mx=0;
    for(int i=0;i<actionNum;i++){
        // 最大次数
        // if(node[node[0].child[i]].visit>mx){
        //     op=i;
        //     mx=node[node[0].child[i]].visit;
        // }
        // 最大平均价值
        if(node[node[0].child[i]].value/node[node[0].child[i]].visit>mx){
            op=i;
            mx=node[node[0].child[i]].value/node[node[0].child[i]].visit;
        }
    }
    return op;
}

void solve(){
    
    // GenerateGraph(Graph);
    ReadGraph();
    nowState=vector<int> (strNum,0);

    vector <int> ansActions;

    while(!endState(nowState)){
        cout<<"---------------"<<endl;
        printGraph();

        int bestaction=MCTS();
        // cerr<<bestaction<<endl;
        ansActions.push_back(bestaction);

        // st_i -> st_i+1
        for(int i=0;i<strNum;i++){
            if(nowState[i]<strLen && Graph[i][nowState[i]]==bestaction) nowState[i]++;
        }
    }
    cout<<ansActions.size()<<endl;
    for(auto i:ansActions) cout<<i<<" ";cout<<endl;

}

signed main(){
	ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);int ca=1;clock_t c1=clock();
	solve();
	cerr<<"Time used:"<<1.0*(clock()-c1)/CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}

/*
21
3 0 2 2 3 2 1 0 1 2 3 0 3 3 2 1 2 3 3 0 2 
Time used:5.69207s
*/