split : Splits the array A[1..n] into two parts :
L[1..pos] , R[pos+1..n] , about “pos”. 
Note that A[pos] comes in the left part.

void split(pnode t,pnode &l,pnode &r,int pos,int add=0){
    if(!t)return void(l=r=NULL);
    int curr_pos = add + sz(t->l);
    if(curr_pos<=pos)//element at pos goes to "l"
        split(t->r,t->r,r,pos,curr_pos+1),l=t;
    else
        split(t->l,l,t->l,pos,add),r=t;
    upd_sz(t);
}


merge : merges L[1..n1] , R[1..n2] to form A[1..n1,n1+1,…n2] . 
Note that the condition for merge in treap (i.e. greatest element in l <= smallest element in r) is satisified here
since the keys are array indices and we are assuming that mergin L and R would result in A such that 
first n1 elements of A come from L and next n2 from R.

void merge(pnode &t,pnode l,pnode r){ //l->leftarray,r->rightarray,t->resulting array
    if(!l || !r) t = l?l:r;
    else if(l->prior>r->prior)merge(l->r,l->r,r),t=l;
    else    merge(r->l,l,r->l),t=r;
    upd_sz(t);
}

Like segment tree, here also each node of treap represents a range

Initialization of a node 

typedef struct node{
    int prior,size,val,sum,lazy;
    //value in array,info of segtree,lazy update
    struct node *l,*r;
}node;typedef node* pnode;
int sz(pnode t){
    return t?t->size:0;
}
void upd_sz(pnode t){
    if(t)t->size=sz(t->l)+1+sz(t->r);
}

pnode init(int val){
    pnode ret = (pnode)malloc(sizeof(node));
    ret->prior=rand();ret->size=1;
    ret->val=val;ret->sum=val;ret->lazy=0;
    return ret;
}

First , each node would now contain an extra field “f” denoting the value of the objective function for the range represented by that node.
We create a function operation(t) which calculates the value of “f” from the values of the left and right childrent of t . (very similar to function upd_sz(t) above)
We insert calls to this function operation(t) , at the end of all functions which modify the tree (eg split and merge) . (Very similar to the calls to upd_sz(t) ).

void lazy(pnode t){
    if(!t || !t->lazy)return;
    t->val+=t->lazy;//operation of lazy
    t->sum+=t->lazy*sz(t);
    if(t->l)t->l->lazy+=t->lazy;//propagate lazy
    if(t->r)t->r->lazy+=t->lazy;
    t->lazy=0;
}
void combine(pnode& t,pnode l,pnode r){//combine segtree ranges
    if(!l || !r)return void(t = l?l:r);
    t->sum = l->sum + r->sum;
}
void operation(pnode t){//operation of segtree
    if(!t)return;
    reset(t);//node represents single element of array
    lazy(t->l);lazy(t->r);//imp:propagate lazy before combining l,r
    combine(t,t->l,t);combine(t,t,t->r);
}

To answer a query of  [x..y] , we can split the treap into L[1..x-1] , M[x..y] , R[y+1..n] using two calls to split(). 
The root node of treap M,would now contain the answer for the range [x…y].
We again merge these treaps to get back the original treap using two calls to merge().

int range_query(pnode t,int l,int r){//[l,r]
    pnode L,mid,R;
    split(t,L,mid,l-1);split(mid,t,R,r-l);//note: r-l!!
    int ans = t->sum;
    merge(mid,L,t);merge(t,mid,R);
    return ans;
}

For a lazy range update at [x..y] , we need to maintain an additional field “lazy” at each node.
We create a function push(t) which propagates the lazy update from a node to its children.
We insert calls to this function push(t) ,at the beginning of all the functions which modify the tree (eg split and merge) such that before the position of a node is changed, the lazy is propagated to its children. 
Hence, the data is not lost with any change in structure of the tree.

void range_update(pnode t,int l,int r,int val){//[l,r]
    pnode L,mid,R;
    split(t,L,mid,l-1);split(mid,t,R,r-l);//note: r-l!!
    t->lazy+=val; //lazy_update
    merge(mid,L,t);merge(t,mid,R);
}
Since each call to split and merge is O(logN) , overall complexity remains O(logN).
