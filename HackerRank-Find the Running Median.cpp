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

 pnode init(int val){
    pnode ret = (pnode)malloc(sizeof(node));
    ret->val=val;ret->size=1;ret->prior=rand();ret->l=ret->r=NULL;
    return ret;
}

/// kth value finding in treap
int find_kth(pnode t, int val)
{
    if(sz(t->l)+1==val) return t->val;
    if(sz(t->l)>=val) return find_kth(t->l,val);
    else return find_kth(t->r,val-sz(t->l)-1);
}

void split(pnode t,pnode &l,pnode &r,int key){
    if(!t)l=r=NULL;
    else if(t->val<=key)split(t->r,t->r,r,key),l=t;
    else split(t->l,l,t->l,key),r=t;
    upd_sz(t);
}

void insert(pnode &t,pnode it){
    if(!t) t=it;
    else if(it->prior>t->prior)split(t,it->l,it->r,it->val),t=it;
    else insert(t->val<=it->val?t->r:t->l,it);
    upd_sz(t);
}


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    /// it will be our main root
    pnode root=NULL;

        int q,c,x,y,n;
        cin>>n;
        while(n--)
        {
            cin>>y;
            insert(root,init(y));
            int siz=sz(root);
            int median;
            double ans=0.0;
            if(siz%2==1)
            {
                median=find_kth(root,(siz/2)+1);
                cout<<median<<".0"<<endl;
            }
            else
            {
                median=find_kth(root,(siz/2))+find_kth(root,(siz/2)+1);
                if(median%2==1){
                median/=2;
                cout<<median<<".5"<<endl;
                }
                else
                {
                    median/=2;
                    cout<<median<<".0"<<endl;
                }
            }

        }

return 0;
}
