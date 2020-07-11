#include<bits/stdc++.h>

using namespace std;


struct node
{
    int prior,size;
    int val;//value stored in the array
    int sum;//whatever info you want to maintain in segtree for each node
    int leftbest;
    int rightbest;
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

void reset(pnode t)
{
    if(t)t->sum =t->leftbest=t->rightbest=t->ans=t->val;//no need to reset lazy coz when we call this lazy would itself be propagated
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
    t->ans=max(l->rightbest+r->leftbest,max(l->ans,r->ans));
    t->leftbest=max(l->leftbest,l->sum+r->leftbest);
    t->rightbest=max(r->rightbest,r->sum+l->rightbest);
    t->sum = l->sum + r->sum;
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
    ret->size=1;
    ret->val=val;
    ret->sum=val;
    ret->leftbest=val;
    ret->rightbest=val;
    ret->ans=val;
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

void range_update(pnode t,int l,int r,int val) //[l,r]
{
    pnode L,mid,R;
    split(t,L,mid,l-1);
    split(mid,t,R,r-l);//note: r-l!!
//    t->lazy+=val; //lazy_update
    merge(mid,L,t);
    merge(t,mid,R);
}


int main()
{

    /// it will be our main root
    pnode root=NULL;
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        int a;
        cin>>a;
    /// adding elements in our root gradually
        merge(root,root,init(a));
    }

    int q;
    cin>>q;
    while(q--)
    {
        char ch;
        int x,y;
        cin>>ch;
/*
insert(x,i) : insert x at position “i”
To insert an element at position “i”, we split the treap about pos = i-1 such that we get two treaps , L[1..i-1] and R[i .. n].
 Now we merge L and x , and then merge the resulting treap with R.
*/

/// putting element(y) into x position i.e. between ( x-1 and x )
        if(ch=='I')
        {
            cin>>x>>y;
            x--;
            pnode l,r;
            split(root,l,r,x-1);
            merge(root,l,init(y));
            merge(root,root,r);
        }
        /*
delete(i) :delete A[i] from the array
To erase an element at position “i”, we split the treap about pos = i -1 such that we get two treaps L[1…i-1] and R[i…n].
We again split R about pos = i such we get L’ [ i ] and R'[i+1…n].
We now merge L[1..i-1] and R'[i+1..n].
        */
        else if(ch=='D')
        {
            cin>>x;
            x--;
            pnode l,r;
            split(root,l,r,x-1);/// main ->[1.....n]  , l-> [1...x-1] & r-> [x.....n]
            split(r,r,root,0);/// main ->[x....n] , l->[x] & root->[x+1....n]
            merge(root,l,root);/// merge l->[1....x-1] and root->[x+1...n] ,so automatically x position's element deleted.
        }
        /// replace x position's element with y
        else if(ch=='R')
        {
            cin>>x>>y;
            x--;
            pnode l,r;
            split(root,l,r,x-1);/// main->[1...n], l->[1...x-1], r->[x...n]
            split(r,r,root,0);/// main->[x...n], r->[x], root[x+1....n]
            merge(l,l,init(y));/// merge l->[1....x-1] & [y] ,now l->[1...x] where x position takes new element
            merge(root,l,root);/// merge l->[1..x] & root[x+1...n] ,now again root->[1....n]
        }
        /// range query ,like segment tree
        else if(ch=='Q')
        {
            cin>>x>>y;
            x--,y--;
            printf("%d\n",range_query(root,x,y));
        }
    }

    return 0;
}
