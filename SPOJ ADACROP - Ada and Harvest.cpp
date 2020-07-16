
/// Time-6470ms
/// Using treap 
/// Try to compress values first 
/// Then build each treap for each compressed value with these positions 
/// Just insert and erase and answer 

#include<bits/stdc++.h>

using namespace std;

const int mx=1e5;

map<int,int>mp;/// for compressing value
vector< pair<int,int> >query;/// query holding array
set<int>s;

struct node
{
    int prior,size;
    int val;//value stored in the array
    //whatever info you want to maintain in segtree for each node
    int ans;
    struct node *l,*r;
};

typedef node* pnode;

int sz(pnode t)
{
    return t?t->size:0;
}

void upd_sz(pnode t)
{
    if(t)t->size=sz(t->l)+1+sz(t->r);
}

void split(pnode t,pnode &l,pnode &r,int key){
    if(!t)l=r=NULL;
    else if(t->val<=key)split(t->r,t->r,r,key),l=t;
    else split(t->l,l,t->l,key),r=t;
    upd_sz(t);
}

void merge(pnode &t,pnode l,pnode r){
    if(!l || !r)t=l?l:r;
    else if(l->prior > r->prior)merge(l->r,l->r,r),t=l;
    else merge(r->l,l,r->l),t=r;
    upd_sz(t);
}

 pnode init(int val){
    pnode ret = (pnode)malloc(sizeof(node));
    ret->val=val;ret->size=1;ret->prior=rand();ret->l=ret->r=NULL;
    return ret;
}

void insert(pnode &t,pnode it){
    if(!t) t=it;
    else if(it->prior>t->prior)split(t,it->l,it->r,it->val),t=it;
    else insert(t->val<=it->val?t->r:t->l,it);
    upd_sz(t);
}

void erase(pnode &t,int key){
    if(!t)return;
    else if(t->val==key){pnode temp=t;merge(t,t->l,t->r);free(temp);}
    else erase(t->val<key?t->r:t->l,key);
    upd_sz(t);
}

/// return number of elements smaller or equal than x
int sma_and_equa(pnode t,int x)
{
    pnode l,r;
    split(t,l,r,x);
    ///nw->number of elements equal or smaller than x
    int nw=sz(l);
    merge(t,l,r);
    return nw;
}

pnode treap[4*mx];

int main()
{
    int n,h,indx,val;
    scanf("%d%d",&n,&h);
    int arr[n];
    for(int i=0;i<n;i++) {scanf("%d",&arr[i]);s.insert(arr[i]);}
    while(h--)
    {
        scanf("%d%d",&indx,&val);
        query.push_back(make_pair(indx,val));
        s.insert(val);
    }
    int id=0;

    set<int>::iterator it;

    for(it=s.begin();it!=s.end();it++)
    {
        mp[(*it)]=++id;/// compress value
    }

    for(int i=0;i<n;i++)
    {
        arr[i]=mp[arr[i]];
        insert(treap[arr[i]],init(i));
    }

    for(int k=0;k<(int)query.size();k++)
    {
        int idx=query[k].first;
        int val=mp[query[k].second];/// taking compressed value
        erase(treap[arr[idx]],idx);
        insert(treap[val],init(idx));
        arr[idx]=val;
        if(idx==0) {printf("0\n");continue;}
        int res=sma_and_equa(treap[val],idx-1);
        printf("%d\n",res);
    }

    return 0;
}
