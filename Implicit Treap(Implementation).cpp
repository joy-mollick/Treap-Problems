                  Implicit Treap : What all to expect ?

Implicit treap can be viewed as a dynamic array which supports the following operations , each in O(logN) time :

Insert an element at any position.

Delete an element at any position.

Cut an array A[1..n] at any pos such that it is divided into two different arrays B[1..pos] , C[pos…n] .

Merge two different arrays P[1..n1] , Q[1..n2] into a single array R[1..n1,n+1,…n2].
Maintain any objective function and query over an arbitrary interval. (All the operations supported by a segment tree including range updates using lazy propagation).

In short, we get all the operations supported by a segment tree along with the power to split an array into two parts and merge two different arrays into a single one, both of them in O(logN) time.

                             Key Idea: 

The key idea in implicit treap is that now , we use the array indices of elements as keys , instead of the values .

                      Why does Key Idea fail ? 

Since we are using the array index as the key of the BST this time, with each update (insertion / deletion) we will have to change O(n) values (the index of O(n) nodes would change upon an insertion/deletion of an element in array). This would be very slow.

                        So, new Idea ??? 

we will not explicitly store the index i (i.e. the “key” or Bk value) at each node in the implicit treap.

Hence the name Implicit Treap because the key values are not stored explicitly and are implicit.

The key value for any node x would be 1 + no of nodes in the BST that have key values less than x . (where node x means node representing A[x] ).

Note that nodes having key less than x would occur not only in the left subtree of x , but also in the left subtree of all the parents p of x such that x occurs in the right subtree of p.

Hence the key for a node t = sz(t->l) + sz(p->l) for all parents of t such that t occurs in the right subtree of p.


split : Splits the array A[1..n] into two parts :
L[1..pos] , R[pos+1..n] , about “pos”. 
Note that A[pos] comes in the left part.

 /// here t holds the root node of whole array [1 to n] 
 /// and l holds the root of tree array [1 to pos]
 /// r holds the root of the tree array [pos+1...n] 
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
    
/// it takes two trees (l) and (r) and make a new tree 
/// Just same as previous simple treap 
/// By keeping all properties ok , it makes new tree and set t of that new tree .

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
/// same as basic treap 
pnode init(int val){
    pnode ret = (pnode)malloc(sizeof(node));
    ret->prior=rand();ret->size=1;
    ret->val=val;ret->sum=val;ret->lazy=0;
    return ret;
}

First , each node would now contain an extra field “f” denoting the value of the objective function for the range represented by that node.
We create a function operation(t) which calculates the value of “f” from the values of the left and right childrent of t . (very similar to function upd_sz(t) above)
We insert calls to this function operation(t) , at the end of all functions which modify the tree (eg split and merge) . (Very similar to the calls to upd_sz(t) ).

/// lazy propagation of segment tree
void lazy(pnode t){
    if(!t || !t->lazy)return;
    t->val+=t->lazy;//operation of lazy
    t->sum+=t->lazy*sz(t);
    if(t->l)t->l->lazy+=t->lazy;//propagate lazy
    if(t->r)t->r->lazy+=t->lazy;
    t->lazy=0;
}

/// It is nothing but a function which is a part of segment tree function
void combine(pnode& t,pnode l,pnode r){//combine segtree ranges
    if(!l || !r)return void(t = l?l:r);
    t->sum = l->sum + r->sum;
}

void reset(pnode t){
    if(t)t->sum = t->val;//lazy already propagated
}

/// this is operation for segment tree 
void operation(pnode t){//operation of segtree
    if(!t)return;
    reset(t);//node represents single element of array
    lazy(t->l);lazy(t->r);//imp:propagate lazy before combining l,r
    combine(t,t->l,t);combine(t,t,t->r);
}

To answer a query of  [x..y] , we can split the treap into L[1..x-1] , M[x..y] , R[y+1..n] using two calls to split(). 
The root node of treap M,would now contain the answer for the range [x…y].
We again merge these treaps to get back the original treap using two calls to merge().

/// Very interesting way to make do this operation 
/// first split(t,l,mid,l-1) makes two trees, main tree is t[1.....n] 
/// One tree with rooted(L) [1,l-1] and another tree with rooted(mid) [l,n] 
/// second split(mid,t,R,r-l) , main tree is mid [l...n] 
/// One tree with rooted(t) is [l....l+(r-l)] ,which will contain our main tree
/// Another one tree with rooted(R) [r+1......n] .
/// Now time is to merge them again ,finsih done 
/// First merge(mid,L,t) ,where L is [1,l-1] and t is  [l....l+(r-l)] which will make new tree [1.....r] with root mid .half done
/// Now second half takes mid [1....r] and R [r+1.......n] and merge them with root t .
/// That means , full done .
    
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
Hence, the data is not lost with any change in structure of the tree,

/// range update is almost similar to range_query 
/// Finding the root of that [l.....r] range and assign its lazy to given value.

void range_update(pnode t,int l,int r,int val){//[l,r]
    pnode L,mid,R;
    split(t,L,mid,l-1);split(mid,t,R,r-l);//note: r-l!!
    t->lazy+=val; //lazy_update
    merge(mid,L,t);merge(t,mid,R);
}
Since each call to split and merge is O(logN) , overall complexity remains O(logN).

Additional Operations
Other operations supported by implicit treap can be realized using the basic split and merge operations (with a little modification as we shall see) .

insert(x,i) : insert x at position “i”
To insert an element at position “i”, we split the treap about pos = i-1 such that we get two treaps , L[1..i-1] and R[i .. n]. Now we merge L and x , and then merge the resulting treap with R.
delete(i) : delete A[i] from the array
To erase an element at position “i”, we split the treap about pos = i -1 such that we get two treaps L[1…i-1] and R[i…n]. We again split R about pos = i such we get L’ [ i ] and R'[i+1…n]. We now merge L[1..i-1] and R'[i+1..n].

