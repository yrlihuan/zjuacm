#include<stdio.h>
#include<vector>
#include<iostream>
#include<map>
#include<string.h>
#include<queue>
#include<algorithm>
using namespace std;

const int MAXN=2010;
const int MOD=1000000007;
vector<int>g[MAXN];
map<int,int>dp[MAXN];
int n,k;
int score[MAXN];
bool vis[MAXN];

int gcd(int a,int b)
{
    if(b==0)return a;
    return gcd(b,a%b);
}
int bfs()
{
    int ret=0;
    queue<int>q;
    dp[1][score[1]]=1;
    memset(vis,false,sizeof(vis));
    while(!q.empty())q.pop();
    q.push(1);
    vis[1]=true;
    while(!q.empty())
    {
        int cur=q.front();
        q.pop();
        // cout << "pop " << cur << endl;
        vis[cur]=false;
        if(cur==n)ret=(ret+dp[cur][k])%MOD;
        for(int i=0;i<g[cur].size();i++)
        {
            int v=g[cur][i];
            map<int,int>::iterator it;
            for(it=dp[cur].begin();it!=dp[cur].end();it++)
            {
                int temp=it->first;
                int lcm=temp/gcd(temp,score[v])*score[v];
                if(lcm!=temp &&  k%lcm==0)
                {
                    dp[v][lcm]=(dp[v][lcm]+it->second)%MOD;
                    if(!vis[v])
                    {
                        vis[v]=true;
                        // cout << "push " << v << endl;
                        q.push(v);
                    }
                }
            }
        }
        dp[cur].clear();
    }
    return ret;
}
int main()
{
    int m,u,v;
    while(scanf("%d%d%d",&n,&m,&k)!=EOF)
    {
        for(int i=1;i<=n;i++)g[i].clear();
        for(int i=1;i<=n;i++)dp[i].clear();
        while(m--)
        {
            scanf("%d%d",&u,&v);
            g[u].push_back(v);
        }
        for(int i=1;i<=n;i++)scanf("%d",&score[i]);
        printf("%d\n",bfs());
    }
    return 0;
}
