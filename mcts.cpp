#include <bits/stdc++.h>
#include <random>
#define maxn 2000005
using namespace std;

int R(int l,int r){
    random_device seed;//硬件生成随机数种子
	ranlux48 engine(seed());//利用种子生成随机数引擎
    uniform_int_distribution<> Rand(l, r);//设置随机数范围，并为均匀分布
    return Rand(engine);
}

int action[4]={2,-2,3,-3};
int actionNum=4;
int gameTurn=8;
int MAX_VALUE;

// 定义树上节点的状态
struct State{
    int sum;
    vector <int> actions;
};

// 定义树的结点
struct Node{
    // level 是树的深度 
    // visit 是节点访问次数
    // fa 是节点的 father
    // value 是节点的价值
    int level,visit,fa;
    double value;
    State state;
    vector <int> child;
};

int nodeNum=0;
vector <Node> node;

// 选择最优的儿子进行搜索
int UCBselect(int rt){

    double maxv=-1e10;
    int maxchild=0;
    for(int i=0;i<actionNum;i++){
        int v=node[rt].child[i];
        if(node[v].visit==0) return v;  // 儿子没跑过 UCB无限大

        double t=node[v].value/node[v].visit+sqrt(2*log(node[0].visit)/node[v].visit);
        if(t>maxv){
            maxv=t;
            maxchild=v;
        }
    }
    return maxchild;
}

// 根据UCB select一个节点
int select(int rt){
    while(1){
        if(node[rt].child.size()==0) return rt;
        rt=UCBselect(rt);
    }
}

// expand 一个节点的所有儿子（操作）
void expend(int rt){
    node[rt].child.resize(actionNum);
    // cerr<<nodeNum<<endl;
    for(int i=0;i<actionNum;i++){
        node[rt].child[i]=nodeNum;
        node[nodeNum].level=node[rt].level+1;
        node[nodeNum].state=node[rt].state;
        node[nodeNum].state.sum+=action[i];
        node[nodeNum].state.actions.push_back(i);
        node[nodeNum].fa=rt;
        nodeNum++;
    }
}

// back propagation 将结果反馈到rt
void bp(int rt,double r){
    while(rt!=0){
        node[rt].visit++;
        node[rt].value+=r;
        rt=node[rt].fa;
    }
    node[rt].visit++;
    node[rt].value+=r;
}

// rollout 在这个节点随机模拟 直到结束 统计结果
void rollout(int rt){
    int sum=node[rt].state.sum;
    int level=node[rt].level;

    while(level<=gameTurn){
        int act=R(0,3);
        sum+=action[act]*(gameTurn-level+1);
        level++;
    }
    // r = 1.0-(abs(self.value-self.GOAL)/self.MAX_VALUE)
    double r=1-(1.0*abs(sum)/MAX_VALUE);
    bp(rt,r);
}

// 一次模拟：
// 首先通过UCB select一个节点
// 如果这个点没有vis过 那么rollout
// 否则expand这个点 然后对拓展出的第一个点rollout

void sim_once(int rt){
    // cerr<<rt<<endl;
    int now=select(rt);
    // cerr<<now<<endl;
    if(node[now].visit==0) rollout(now);
    else{
        expend(now);
        now=node[now].child[0];
        rollout(now);
    }
}

// 输出最优决策路径
void printPath(){
    int rt=0;
    while(node[rt].level<gameTurn){
        double mx=-1e10;
        int to=-1;

        if(node[rt].child.size()==0) {
            cerr<<"error"<<endl;
            return ;
        }
        for(int i=0;i<actionNum;i++){
            int v=node[rt].child[i];
            // if(node[v].value/node[v].visit>mx && node[v].visit){  // 根据平均价值 来选择最优策略
            //     mx=node[v].value/node[v].visit;
            //     to=v;
            // }

            if(node[v].visit>mx && node[v].visit){  // 根据访问次数 来选择最优策略
                mx=node[v].visit;
                to=v;
            }

            
        }
        cerr<<rt<<endl;
        if(to==-1){
            cerr<<"er2"<<endl;
            return ;
        }
        rt=to;  
    }
    for(int j=0;j<gameTurn;j++){
        cout<<node[rt].state.actions[j]<<" ";
    }
    cout<<endl;

    for(int j=0;j<gameTurn;j++){
        cout<<action[node[rt].state.actions[j]]*(gameTurn-j+1)<<" ";
    }
    cout<<endl;
}

// 输出整棵树
void printTree(int u){
    
    if(node[u].child.size()){
        for(int i=0;i<actionNum;i++){
            
            int v=node[u].child[i];
            cout<<v<<" "<<node[v].fa<<" "<<node[v].value<<" "<<node[v].visit<<endl;
            printTree(v);
        }
    }
}

void solve(){
    MAX_VALUE=gameTurn*(gameTurn+1)/2*5;
    int sim=1000000;

    node.resize(maxn);

    nodeNum++;
    int rt=0;
    expend(rt);
    
    for(int i=1;i<=sim;i++){
        sim_once(rt);
    }
    freopen("out.out","w",stdout);
    printPath();

    // for(int i=1;i<=4;i++){
    //     cout<<i<<" "<<node[i].fa<<" "<<node[i].value<<" "<<node[i].visit<<endl;
    // }
    printTree(0);
}

signed main(){
	ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);int ca=1;clock_t c1=clock();
	solve();
	// cerr<<"Time used:"<<clock()-c1<<"ms"<<endl;
	return 0;
}
