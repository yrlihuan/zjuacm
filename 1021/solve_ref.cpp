/*
ZOJ1021
参考“狗狗40题搞完纪念“标程 
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <list>
#include <queue>
#include <cstdio>
#include <utility>
using namespace std;
#define maxp 30

int main()
{
    int cases, pipes, links, target, level, tot_water;
    int w,p,pp;
    int px[maxp], py[maxp], bottom[maxp], water[maxp];
    bool find_ans;
    set<int> link;
    list<pair<int, int> > e[maxp];
    priority_queue<pair<int, int> > pq;
    
    scanf("%d", &cases);
    while (cases--)
    {
        //read data
        scanf("%d", &pipes);
        for (int i=0; i<pipes; i++)
        {
            int tmp;
            scanf("%d%d%d", &px[i], &py[i], &tmp);
            bottom[i]=py[i]+tmp;    //bottom y-coordinate of the pipe
            water[i]=-1;            //means !visit pipe i
            e[i].clear();
        }
        scanf("%d", &links);
        link.clear();
        for (int i=0; i<links; i++)
        {
            int lx, ly, len, lk1=-1, lk2=-1;
            scanf("%d%d%d", &lx, &ly, &len);
            for (int j=0; j<pipes; j++)
                if (bottom[j]>=ly && py[j]<=ly)
                {
                    if (px[j]+1==lx) lk1=j;
                    if (px[j]==lx+len) lk2=j;
                    if (lk1!=-1 && lk2!=-1)
                    {
                        break;
                    }
                }
            link.insert(ly);
            e[lk1].push_back(make_pair(ly, lk2));
            e[lk2].push_back(make_pair(ly, lk1));
        }
        scanf("%d%d", &target, &level);
        link.insert(level);
        target--;
        //initial part
        if (level<py[target]) 
        {
            printf("No Solution\n");
            continue;
        }
        if (py[target]<=level && level<=bottom[target])
            e[target].push_back(make_pair(level, -2));
        for (int i=0; i<pipes; i++)
        {
            for (set<int>::const_iterator j=link.lower_bound(py[i]);
                 j!=link.end()&&*j<bottom[i]; j++)
            {
                e[i].push_back(make_pair(*j, i));
            }
            e[i].push_back(make_pair(py[i], -1));
            e[i].sort();
            e[i].reverse();
        }
        while (!pq.empty())
            pq.pop();
        water[0]=bottom[0];
        pq.push(make_pair(water[0], 0));
        find_ans=false;
        tot_water=0;
        //solve part
        while (!pq.empty())
        {
            w=pq.top().first;   //water   
            p=pq.top().second;  //pipe
            pq.pop();
            if (p==-1)
                break;
            else if (p==-2)
            {
                find_ans=true;
                break;
            }
            tot_water+=(water[p]-w);
            water[p]=w;
            while (!e[p].empty() && e[p].front().first==water[p])
            {
                pp=e[p].front().second;
                if (pp<0)
                {
                    pq.push(e[p].front());
                }
                else if (water[pp]==-1)
                {
                    water[pp]=bottom[pp];
                    pq.push(make_pair(water[pp], pp));
                }
                e[p].pop_front();
            }
            if (!e[p].empty())
            {
                pq.push(make_pair(e[p].front().first, p));
            }
        }
        if (find_ans)
            printf("%d\n", tot_water);
        else
            printf("No Solution\n");
    }
    return 0;
}
        
            
