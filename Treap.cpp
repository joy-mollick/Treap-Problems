    Teap ----------- Tree + Heap 
    ->  Binary Search Tree + (max / min ) heap 


AVL Tree : It is a self balanced tree (another version of BST),which has been balanced by left and right rotation to keep its depth(Height) logN. 
But it is for only BST not for Heap property.The node of this tree takes only BST value. 

 Cartesian tree : It is tree which used both BST and Heap properties to build tree as a node(pair).
 It takes node(B,H) in this tree , where B holds for BST value of pair and H holds for heap value.
 You can assume this tree as only BST if you think only one value of the node(pair) and you can assume this tree as a heap(max or min) tree if you think with only other one of the node(pair). 
 But cartesian tree is not a better tree because if we go down to search  any node or delete any node or insert any node to the tree it will take O(H) where H is depth(height) of the tree. 
 In worst case, the depth of the cartesian tree can be approximately N. Then time is approximately O(N).

So avoiding this issue , we will introduce another tree called Randomized Binary Search Tree .
Why it is named like randomized ? If we randomly assign values as heap value of every node ,then the expected value of depth(Height) of this tree will be approximately logN. 
Then it will make good time complexity for all operations. So let's move on further......
 
 /// structure of a treap's node
 typedef struct node{
    int val,prior,size;
    struct node *l,*r;
}node;
typedef node* pnode;
/// it will give the size of the tree ,how many nodes of this tree of rooted (t)
int sz(pnode t){
    return t?t->size:0;
}
/// it update the size of the tree 
void upd_sz(pnode t){
    if(t)t->size = sz(t->l)+1+sz(t->r);
}

insert(X) : To  insert a value X into our Treap,we first chose a Y = rand() , such that (X,Y) [ (B,H) ] represents the new node to be inserted in the treap. 
Then, keep on going down the tree like a simple BST searching for the correct pos where X should be inserted unless either the correct position is found OR we encounter the first node E s.t. priority(E) < Y . 
Here, call split(E,X) which will actually do such operations according to new node (X,Y) that for inserting node(X,Y) into Treap ,no rules of bst and heap will not violated (like rotations in AVL tree to keep tree balanced ) and attach L and R as left and right subtrees of node (X,Y) . 

 /// we will initialize new node 
/// And we will send this node's pointer as node (it) into insert function
    
    pnode init(int val){
    pnode ret = (pnode)malloc(sizeof(node));
    ret->val=val;ret->size=1;ret->prior=rand();ret->l=ret->r=NULL;
    return ret;
}
/// its main target is to split the t node , its leftside(l) and rightside(r) into two subtree l and r and assign those l and r recursively to new node's left and right side 
/// So , new node will take the place of t node and t node and its two subtrees will be re-arranged as two subtree of new-node and all upper nodes are samely connected to t as well 
/// t node has value key and all of its left sides have smaller bst value and right sides have greater value 
/// As we assign to the t node as new insertion value 
    
void split(pnode t,pnode &l,pnode &r,int key){
    if(!t)l=r=NULL;
    else if(t->val<=key)split(t->r,t->r,r,key),l=t;
    else split(t->l,l,t->l,key),r=t;
    upd_sz(t);
}

/// we will go down like bst 
/// Then find that first node whose priority is less than the inserted node
/// Just we will send this (t) node and new node's(it) left and right side pointer
/// Then now total tree assuming t as root node will be divided into two grups(Left and Right)
/// And assign them to left and right of (it->left and it->right)
/// make assign t to it ,done insertion

void insert(pnode &t,pnode it){
    if(!t) t=it;
    else if(it->prior>t->prior)split(t,it->l,it->r,it->val),t=it;
    else insert(t->val<=it->val?t->r:t->l,it);
    upd_sz(t);
}

erase(X) : Go down the tree like a BST unless node to be deleted is found.
If the node is found, call merge function for it’s left and right subtrees and attach the resulting tree to the parent of the node to be deleted.
merge function is a special function which actually attached two nodes of that node which will be deleted ,after attaching we will free that deletable node.
So now go to the code ........
    
 /// here merege operation actually take left and right sides of t node(deletable node)
 /// And merge them in such way that to keep all heap and bst property balanced 
 /// After merging it keeps root of new balanced tree into t 
  /// So , t exactly not deleted , merging left and right subtrees of t to make another tree and root of new tree is assigned to t 
    
void merge(pnode &t,pnode l,pnode r){
    if(!l || !r)t=l?l:r;
    else if(l->prior > r->prior)merge(l->r,l->r,r),t=l;
    else merge(r->l,l,r->l),t=r;
    upd_sz(t);
}
    
/// deletion is like dletion from bst 
/// find that deletable node 
/// As t is not exactly deleted ,we skip t node's exact property
/// But we need to free that pointer 
/// By taking pointer of that t node into a temporary pointer ,we can free that node
/// So, deletion done.
void erase(pnode &t,int key){
    if(!t)return;
    else if(t->val==key){pnode temp=t;merge(t,t->l,t->r);free(temp);}
    else erase(t->val<key?t->r:t->l,key);
    upd_sz(t);
}

/// index finding of a value,it will return (zero base index)
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
  return -1; // not found
}
    
/// finding kth value in treap
int find_kth(pnode t, int val)
{
    if(sz(t->l)+1==val) return t->val;
    if(sz(t->l)>=val) return find_kth(t->l,val);
    else return find_kth(t->r,val-sz(t->l)-1);
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
