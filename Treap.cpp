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

insert(X) : To  insert a value X into our Treap,we first chose a Y = rand() , such that (X,Y) [ (B,H) ] represents the new node to be inserted in the treap. 
Then, keep on going down the tree like a simple BST searching for the correct pos where X should be inserted unless either the correct position is found OR we encounter the first node E s.t. priority(E) < Y . 
Here, call split(E,X) which will actually do such operations according to new node (X,Y) that for inserting node(X,Y) into Treap ,no rules of bst and heap will not violated (like rotations in AVL tree to keep tree balanced ) and attach L and R as left and right subtrees of node (X,Y) . 

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

/// 
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

