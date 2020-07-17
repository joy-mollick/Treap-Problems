/// Very Nice-> Classical One 
/// :) 
/// Using Basic Treap 

#include<bits/stdc++.h>

using namespace std;

typedef long long ll;

/// structure of a treap's node
 typedef struct node{
    ll val,prior,size;
    struct node *l,*r;
}node;
typedef node* pnode;
/// it will give the size of the tree ,how many nodes of this tree of rooted (t)
inline ll sz(pnode t){
    return t?t->size:0;
}
/// it update the size of the tree
inline void upd_sz(pnode t){
    if(t)t->size = sz(t->l)+1+sz(t->r);
}

pnode init(ll val){
    pnode ret = (pnode)malloc(sizeof(node));
    ret->val=val;ret->size=1;ret->prior=rand();ret->l=ret->r=NULL;
    return ret;
}

 void split(pnode t,pnode &l,pnode &r,ll key){
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

inline void insert(pnode &t,pnode it){
    if(!t) t=it;
    else if(it->prior>t->prior)split(t,it->l,it->r,it->val),t=it;
    else insert(t->val<=it->val?t->r:t->l,it);
    upd_sz(t);
}


/// return number of elements smaller or equal than x
inline ll sma_and_equa(pnode &t,ll x)
{
    pnode l,r;
    split(t,l,r,x);
    ///nw->number of elements equal or smaller than x
    ll nw=sz(l);
    merge(t,l,r);
    return nw;
}

int main()

{
    ll k,n,val;
    scanf("%lld%lld",&n,&k);
    pnode root=NULL;
    insert(root,init(0ll));
    ll pref=0,res=0;
    for(int i=0;i<n;++i)
    {
        scanf("%lld",&val);
        val-=k;
        pref+=val;
        res+=sma_and_equa(root,pref);
        insert(root,init(pref));
    }
    printf("%lld\n",res);

    return 0;
}
