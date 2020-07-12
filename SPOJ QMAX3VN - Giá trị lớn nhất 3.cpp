/// Time-360ms

#include<bits/stdc++.h>

using namespace std;


struct node
{
    int prior,siz;
    int val;//value stored in the array
    //whatever info you want to maintain in segtree for each node
    int ans;
    struct node *l,*r;
};

typedef node* pnode;

int sz(pnode t)
{
    return t?t->siz:0;
}

void upd_sz(pnode t)
{
    if(t)t->siz=sz(t->l)+1+sz(t->r);
}

void reset(pnode t)
{
    if(t) t->ans=t->val;//no need to reset lazy coz when we call this lazy would itself be propagated
}
/// we are finding maximum contiguous sum inside a range
/// Here we should take into account another some fields
/// leftmost is maximum contiguous sum starting from first element in a range
/// rightmost is maximum contiguous sum which will end at last element of a range
/// when we merge two tree then ,we have to keep these properties
/// new leftbest will be max(l->leftbest,l->sum+r->leftbest)
/// new rightbest will be max(r->rightbest,r->sum+l->rightbest)
/// so,answer will be max (l->rightbest+r->leftbest , l->ans,r->ans)

void combine(pnode& t,pnode l,pnode r) ///combining two ranges of segtree
{
    if(!l || !r)return void(t = l?l:r);
    t->ans=max(l->ans,r->ans);
}
void operation(pnode t) ///operation of segtree
{
    if(!t)return;
    reset(t);//reset the value of current node assuming it now represents a single element of the array
//    lazy(t->l);
//    lazy(t->r);//imp:propagate lazy before combining t->l,t->r;
    combine(t,t->l,t);
    combine(t,t,t->r);
}

void split(pnode t,pnode &l,pnode &r,int pos,int add=0)
{
    if(!t)return void(l=r=NULL);
//    lazy(t);
    int curr_pos = add + sz(t->l);
    if(curr_pos<=pos)//element at pos goes to left subtree(l)
        split(t->r,t->r,r,pos,curr_pos+1),l=t;
    else
        split(t->l,l,t->l,pos,add),r=t;
    upd_sz(t);
    operation(t);
}

void merge(pnode &t,pnode l,pnode r)  //l->leftarray,r->rightarray,t->resulting array
{
//    lazy(l);
//    lazy(r);
    if(!l || !r) t = l?l:r;
    else if(l->prior>r->prior)merge(l->r,l->r,r),t=l;
    else    merge(r->l,l,r->l),t=r;
    upd_sz(t);
    operation(t);
}

pnode init(int val)
{
    pnode ret = (pnode)malloc(sizeof(node));
    ret->l=NULL;
    ret->r=NULL;
    ret->prior=rand();/// randomly assigned priority value
    ret->siz=1;
    ret->ans=val;
    ret->val=val;
//    ret->lazy=0;
    return ret;
}

int range_query(pnode t,int l,int r) //[l,r]
{
    pnode L,mid,R;
    split(t,L,mid,l-1);
    split(mid,t,R,r-l);//note: r-l!!
    int ans = t->ans;
    merge(mid,L,t);
    merge(t,mid,R);
    return ans;
}


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    /// it will be our main root
    pnode root=NULL,t=NULL;

        int q,c,x,y;
        cin>>q;
        bool first_time=true;
        while(q--)
        {
        char ch;
        cin>>ch;
        /// putting element(y) into x position i.e. between ( x-1 and x )
        if(ch=='A')
        {
            cin>>y>>x;
            x--;
            if(first_time) {t=init(y);root=t;first_time=false;}
            else
            {
            pnode l,r;
            split(root,l,r,x-1);
            merge(root,l,init(y));
            merge(root,root,r);
            }
        }

        /// range query ,like segment tree
        else
        {
            cin>>x>>y;
            x--,y--;
            cout<<range_query(root,x,y)<<'\n';
        }
        }
    }
