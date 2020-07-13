/// Time-2800ms
/// Basic Classical treap problem 
/// Every operation takes O(logn) time 

#include<bits/stdc++.h>

using namespace std;


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

void erase(pnode &t,int key){
    if(!t)return;
    else if(t->val==key){pnode temp=t;merge(t,t->l,t->r);free(temp);}
    else erase(t->val<key?t->r:t->l,key);
    upd_sz(t);
}

/// kth value finding in treap
int find_kth(pnode t, int val)
{
    if(sz(t->l)+1==val) return t->val;
    if(sz(t->l)>=val) return find_kth(t->l,val);
    else return find_kth(t->r,val-sz(t->l)-1);
}

/// index finding of a value
int rank_of(pnode t, int val) {
  int rank = 0;
  while (t) {
    if (val < t->val) // move to left subtree
      t = t->l;
    else if (val > t->val) {
      rank += 1 + sz(t->l);
      t = t->r;
    }
    else
      return rank + sz(t->l);
  }
  return -2; // not found
}

void insert(pnode &t,pnode it){
    if(!t) t=it;
    else if(it->prior>t->prior)split(t,it->l,it->r,it->val),t=it;
    else insert(t->val<=it->val?t->r:t->l,it);
    upd_sz(t);
}


int main()
{
    /// it will be our main root
    pnode root=NULL;
        int t,val;
        while(scanf("%d",&t)&& t!=-1)
        {

        int x;

       scanf("%d",&x);
        if(t==1)
        {
            insert(root,init(x));
        }

        else if(t==2)
        {
            erase(root,x);
        }

        else if(t==4)
        {
            if(x<=sz(root)&&x!=0)
            {
                printf("%d\n",find_kth(root,x));
            }
            else
            {
                printf("-1\n");
            }
        }

        else if(t==3)
        {
            printf("%d\n",rank_of(root,x)+1);
        }

        }

        return 0;
    }
