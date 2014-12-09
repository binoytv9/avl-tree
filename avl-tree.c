/*	implementation of Adelson-Velsky and Landi's Tree	*/


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
struct avlNode *delete(struct avlNode *p,int data,int *found);

main()
{
	int l;
	int found = 0;
	int data_to_delete;
	int a[] = {8,3,7,5,1,9,4};
	struct avlNode *root = NULL;

	l = sizeof(a)/sizeof(int);
	root = buildTree(a,l);

	printf("\n\n");
	printTree(root);
	printf("\n\n");

	printf("\tenter the data to delete : ");
	scanf("%d",&data_to_delete);
	root = delete(root,data_to_delete,&found);
	if(found){
		printf("\n\nafter deleting %d\n\n\t\t",data_to_delete);
		printTree(root);
	}
	else
		printf("\n\n\t%d not found",data_to_delete);
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

struct avlNode *delete(struct avlNode *p,int data,int *found)
{
	int hFactor;
	struct avlNode *tmp;

	if(p == NULL)
		return NULL;

	if(data < p->data)
		p->left = delete(p->left,data,found);
	else if(data > p->data)
		p->right = delete(p->right,data,found);
	else{
		*found = 1;
		//node with less than two children 
		if(p->left == NULL || p->right == NULL){
			tmp = p->left ? p->left : p->right;

			//No child case
			if(tmp == NULL){
				tmp = p;
				p = NULL;
			} 
			//One child case
			else
			*p = *tmp; //Copy the contents of the non-empty child

			free(tmp);
		}
		//node with two children 
		else{
			tmp = minValueNode(p->right);

			//Copy the inorder successor's data to this node
			p->data = tmp->data;

			//Delete the inorder successor
			p->right = delete(p->right, tmp->data,found);
		}
	}

	if (p == NULL)
		return p;

	p->height = max(findHeight(p->left), findHeight(p->right)) + 1;

	hFactor = checkHeight(p);

	//left child left subtree
	if (hFactor > 1 && checkHeight(p->left) >= 0)
		return rightRotate(p);

	//left child right subtree
	if (hFactor > 1 && checkHeight(p->left) < 0)
	{
		p->left =  leftRotate(p->left);
		return rightRotate(p);
	}

	//right child right subtree
	if (hFactor < -1 && checkHeight(p->right) <= 0)
		return leftRotate(p);

	//right child left subtree
	if (hFactor < -1 && checkHeight(p->right) > 0)
	{
		p->right = rightRotate(p->right);
		return leftRotate(p);
	}

	return p;
}

//Get the inorder successor (smallest in the right subtree)
struct avlNode *minValueNode(struct avlNode *node)
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
