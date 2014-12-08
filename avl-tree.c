/* implementation of Adelson-Velsky and Landi's Tree */


#include<stdio.h>
#include<stdlib.h>

struct avlNode{
	int data;
	int height;
	struct avlNode *left;
	struct avlNode *right;
};

int max(int a,int b);
int findHeight(struct avlNode *node);
void printTree(struct avlNode *node);
int checkHeight(struct avlNode *node);
struct avlNode *buildTree(int *a,int l);
struct avlNode *leftRotate(struct avlNode *node);
struct avlNode *rightRotate(struct avlNode *node);
struct avlNode *insert(struct avlNode *p,int data);
struct avlNode * minValueNode(struct avlNode* node);
struct avlNode *delete(struct avlNode *root,int data);

main()
{
	int l;
	int a[] = {8,3,7,5,1,9,4};
	struct avlNode *root = NULL;

	l = sizeof(a)/sizeof(int);
	root = buildTree(a,l);

	printf("\n\n");
	printTree(root);
	printf("\n\n");

	printf("after deleting 8\n\n");
	root = delete(root,8);
	printTree(root);
	printf("\n\n");
}

struct avlNode *buildTree(int *a,int l)
{
	int i;
	struct avlNode *root = NULL;

	for(i=0;i<l;++i)
		root = insert(root,a[i]);
	return root;
}

struct avlNode *insert(struct avlNode *p,int data)
{
	int hFactor;

	if(p == NULL){
		p = (struct avlNode *)malloc(sizeof(struct avlNode));
		p->data = data;
		p->height = 0;
		p->left = p->right = NULL;
	}
	else if(data <= p->data)
		p->left = insert(p->left,data);
	else
		p->right = insert(p->right,data);

	p->height = max(findHeight(p->left),findHeight(p->right)) + 1;

	hFactor = checkHeight(p);

	//left child left subtree
	if(hFactor > 1 && data < p->left->data)
		return rightRotate(p);

	//left child right subtree
	if(hFactor > 1 && data > p->left->data){
		p->left = leftRotate(p->left);
		return rightRotate(p);
	}

	//right child right subtree
	if(hFactor < -1 && data > p->right->data)
		return leftRotate(p);

	//right child left subtree
	if(hFactor < -1 && data < p->right->data){
		p->right = rightRotate(p->right);
		return leftRotate(p);
	}

	return p;
}

struct avlNode *delete(struct avlNode *root,int data)
{
	int balance;
	struct avlNode *tmp;

	if(root == NULL)
		return NULL;

	if(data < root->data)
		root->left = delete(root->left,data);
	else if(data > root->data)
		root->right = delete(root->right,data);
	else{
		if(root->left == NULL || root->right == NULL){
			tmp = root->left ? root->left : root->right;

			// No child case
			if(tmp == NULL){
				tmp = root;
				root = NULL;
			}
			else // One child case
			*root = *tmp; // Copy the contents of the non-empty child

			free(tmp);
		}
		else{
			// node with two children: Get the inorder successor (smallest in the right subtree)
			tmp = minValueNode(root->right);

			// Copy the inorder successor's data to this node
			root->data = tmp->data;

			// Delete the inorder successor
			root->right = delete(root->right, tmp->data);
		}
	}

	if (root == NULL)
	  return root;

	root->height = max(findHeight(root->left), findHeight(root->right)) + 1;

	balance = checkHeight(root);

	// Left Left Case
	if (balance > 1 && checkHeight(root->left) >= 0)
		return rightRotate(root);

	// Left Right Case
	if (balance > 1 && checkHeight(root->left) < 0)
	{
		root->left =  leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Right Case
	if (balance < -1 && checkHeight(root->right) <= 0)
		return leftRotate(root);

	// Right Left Case
	if (balance < -1 && checkHeight(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

struct avlNode * minValueNode(struct avlNode* node)
{
    while (node->left != NULL)
        node = node->left;

    return node;
}

int max(int a,int b)
{
	return a>b ? a : b;
}

int findHeight(struct avlNode *node)
{
	if(node == NULL)
		return 0;
	else
		return node->height;
}

int checkHeight(struct avlNode *node)
{
	if(node == NULL)
		return 0;
	else
		return findHeight(node->left) - findHeight(node->right);
}

struct avlNode *rightRotate(struct avlNode *node)
{
	struct avlNode *tmp;

	tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;

	node->height = max(findHeight(node->left),findHeight(node->right))+1;
	tmp->height = max(findHeight(tmp->left),findHeight(tmp->right))+1;

	return tmp;
}

struct avlNode *leftRotate(struct avlNode *node)
{
	struct avlNode *tmp;

	tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;

	node->height = max(findHeight(node->left),findHeight(node->right))+1;
	tmp->height = max(findHeight(tmp->left),findHeight(tmp->right))+1;

	return tmp;
}

void printTree(struct avlNode *node)
{
	if(node != NULL){
		printTree(node->left);
		printf("%d ",node->data);
		printTree(node->right);
	}

}
