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
	if(hFactor > 1){
		//left child left subtree
		if(data < p->left->data)
			return rightRotate(p);
		//left child right subtree
		else if(data > p->left->data){
			p->left = leftRotate(p->left);
			return rightRotate(p);
		}
		//right child right subtree
		else if(data > p->right->data)
			return leftRotate(p);
		//right child left subtree
		else{
			p->right = rightRotate(p->right);
			return leftRotate(p);
		}
	}
	else
		return p;
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
		return abs(findHeight(node->left) - findHeight(node->right));
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
		//printf("node %d\theight %d\n",node->data,node->height);
		printTree(node->right);
	}

}
