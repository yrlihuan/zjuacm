#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool map[52][52];
int num,n,list[11];
void printmap()
{
     int i,j;
     for(i=1;i<=2*n;i++)
     {
       for(j=1;j<4*n;j++) printf("%d ",map[i][j]);
       printf("\n");
     }
     printf("\n\n");
}
bool canput(int r,int c,int len)
{
   int i,j;
   if(r+len-1>2*n||c+len-1>4*n) return 0;
   if(c%2==0)
   {
      for(i=r;i<r+len;i++)
        for(j=c+2*(i-r);j<c+2*(i-r)+2*len-1-2*(i-r);j++)
          if(map[i][j]) return 0;
   } 
   else
   {
       for(i=r;i<r+len;i++)
         for(j=c;j<c+2*(i-r+1)-1;j++)
           if(map[i][j]) return 0;
   }
   return 1;
}
void put(int r,int c,int len)
{
   int i,j;
   if(c%2==0)
   {
      for(i=r;i<r+len;i++)
        for(j=c+2*(i-r);j<c+2*(i-r)+2*len-1-2*(i-r);j++)
          map[i][j]=1;
   } 
   else
   {
       for(i=r;i<r+len;i++)
         for(j=c;j<c+2*(i-r+1)-1;j++)
           map[i][j]=1;
   }
}
void deput(int r,int c,int len)
{
   int i,j;
   if(c%2==0)
   {
      for(i=r;i<r+len;i++)
        for(j=c+2*(i-r);j<c+2*(i-r)+2*len-1-2*(i-r);j++)
          map[i][j]=0;
   } 
   else
   {
       for(i=r;i<r+len;i++)
         for(j=c;j<c+2*(i-r+1)-1;j++)
           map[i][j]=0;
   }
}
bool detect(int r,int c)
{
   if(r>2*n) return 1;
   else if(c>4*n) return detect(r+1,1);
   else if(map[r][c])
   {
     int i;
     for(i=c;i<=4*n;i++)
       if(!map[r][i]) break;
     return detect(r,i);
   }
   else
   {
     int i;
     for(i=1;i<=num;i++)
     {
       if(canput(r,c,list[i]))
       {
        put(r,c,list[i]);
        if(detect(r,c+1)) return 1;
        deput(r,c,list[i]);
       }
       else break;
     }
     return 0;
   }
}
int cmp(const void *a,const void *b)
{
  return *(int *)a-*(int *)b;
}
int main()
{
    int test,i,k;
    scanf("%d",&test);
    for(i=1;i<=test;i++)
    {
        int j;
        bool flag=0;
        scanf("%d",&n);
        scanf("%d",&num);
        for(j=1;j<=num;j++) scanf("%d",&list[j]);
        qsort(list+1,num,sizeof(int),cmp);
        for(j=1;j<=num;j++)/*优化*/ 
        {
         if(n%list[j]==0)
          {
           printf("YES\n");
           flag=1;
           break;
          }
          if(list[j]>n)
          {
           num=j-1;
           break;
          }
         }
         if(flag) continue;
         for(j=1;j<=num;j++)
           for(k=1;k<j;k++)
             {
              if(list[j]%list[k]==0)
              {
                int l;
                for(l=j;l<num;l++) list[l]=list[l+1];
                j--;
                num--;
                break;
              }
              }
         /*---------------------------------------------*/ 
         memset(map,1,sizeof(map));
         for(j=1;j<=n;j++)
           for(k=1;k<=2*n+1+(j-1)*2;k++)
             map[j][k]=0;
         for(j=n+1;j<=2*n;j++)
            for(k=(j-n)*2;k<=4*n;k++)
              map[j][k]=0;
         if(detect(1,1)) printf("YES\n");
         else printf("NO\n");
    }
   
    return 0;
}
