/* Node is defined as :
typedef struct node
{
    int val;
    struct node* left;
    struct node* right;
    int ht;
} node; */

int height(node *N)
{
    if (N == NULL)
        return -1;
    
    return N->ht;
}

void setHeight(node *nd)
{
    if(nd==NULL) nd->ht=-1;
    else nd->ht=max(height(nd->left),height(nd->right)) + 1;
}

node* newNode(int key)
{
    node* nod = new node();
    nod->val = key;
    nod->left = NULL;
    nod->right = NULL;
    setHeight(nod);
     // new node is initially
    // added at leaf
    return(nod);
}

node *rightRotate(node *y)
{
    node *x = y->left;
    node *T2 = x->right;
    // Perform rotation
    x->right = y;
    y->left = T2;
    // Update heights
    setHeight(x);
    setHeight(y);
    // Return new root
    return x;
}

node *leftRotate(node *x)
{
    node *y = x->right;
    node *T2 = y->left;
    // Perform rotation
    y->left = x;
    x->right = T2;
    // Update heights
    setHeight(x);
    setHeight(y);
    // Return new root
    return y;
}

int getBalance(node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

node * insert(node * root,int val)
{
	if(root==NULL) return newNode(val);
    if(root->val < val)/// we have to recurse for right side 
    {
        root->right=insert(root->right,val);
    }
    else if(root->val > val)/// we have to recurse for left side 
    {
        root->left=insert(root->left,val);
    }
    else return root;
    /// recursively calculate height newly 
    setHeight(root);
    
     int balance = getBalance(root);

    // Left Left Case
    if (balance > 1 && val < root->left->val)
        return rightRotate(root);

    // Right Right Case
    if (balance < -1 && val > root->right->val)
        return leftRotate(root);

    // Left Right Case
    if (balance > 1 && val > root->left->val)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Left Case
    if (balance < -1 && val < root->right->val)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
