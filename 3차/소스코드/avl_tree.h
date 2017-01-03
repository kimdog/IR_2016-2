#include <string.h>

// ========================================================
// DATA structure - AVL TREE
// for Speed up

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define LH +1	// Left High
#define EH 0	// Even High
#define RH -1	// Right High

typedef struct avl_node {
	void* dataPtr;
	struct avl_node* left;
	int bal;
	struct avl_node* right;
} avl_NODE;

typedef struct {
	int count;
	int(*compare) (void* argu1, void* argu2);
	avl_NODE* root;
} AVL_TREE;

// Prototype Declarations
AVL_TREE* AVL_Create(int(*compare)(void* argu1, void* argu2));
AVL_TREE* AVL_Destroy(AVL_TREE* tree);

BOOL AVL_Insert(AVL_TREE* tree, void* dataInPtr);
BOOL AVL_Delete(AVL_TREE* tree, void* dltKey);
void* AVL_Retrieve(AVL_TREE* tree, void* keyPtr);
void AVL_Travers(AVL_TREE* tree, void(*process)(void* dataPtr));

int AVL_Count(AVL_TREE* tree);
BOOL AVL_Empty(AVL_TREE* tree);
BOOL AVL_Full(AVL_TREE* tree);

static avl_NODE* _insert(AVL_TREE* tree, avl_NODE* root, avl_NODE* newPtr, BOOL* taller);
static avl_NODE* _delete(AVL_TREE* tree, avl_NODE* root, void* dltKey, BOOL* shorter, BOOL* success);
static void* _retrieve(AVL_TREE* tree, void* keyPtr, avl_NODE* root);
static void _traversal(avl_NODE* root, void(*process)(void* dataPtr));
static void _destory(avl_NODE* root);

static avl_NODE* rotateLeft(avl_NODE* root);
static avl_NODE* rotateRight(avl_NODE* root);
static avl_NODE* insLeftBal(avl_NODE* root, BOOL* taller);
static avl_NODE* insRightBal(avl_NODE* root, BOOL* taller);
static avl_NODE* dltLeftBal(avl_NODE* root, BOOL* shorter);
static avl_NODE* dltRightBal(avl_NODE* root, BOOL* shorter);

// AVL_Create
AVL_TREE* AVL_Create(int(*compare)(void* argu1, void* argu2)){
	AVL_TREE* tree;

	tree = (AVL_TREE*)malloc(sizeof(AVL_TREE));
	if (tree)
	{
		tree->root		= NULL;
		tree->count		= 0;
		tree->compare	= compare;
	}

	return tree;
}


BOOL AVL_Insert(AVL_TREE* tree, void* dataInPtr) {
	avl_NODE* newPtr;
	BOOL forTaller;

	newPtr = (avl_NODE*)malloc(sizeof(avl_NODE));
	if (!newPtr)
		return FALSE;

	newPtr->bal		= EH;
	newPtr->right	= NULL;
	newPtr->left	= NULL;
	newPtr->dataPtr = dataInPtr;

	tree->root = _insert(tree, tree->root, newPtr, &forTaller);
	
	(tree->count)++;
	return TRUE;
}

// _insert
avl_NODE* _insert(AVL_TREE* tree, avl_NODE* root, avl_NODE* newPtr, BOOL* taller) {
	if (!root)
	{
		// Insert at root
		root	= newPtr;
		*taller = TRUE;
		return root;
	} // if NULL tree

	if (tree->compare(newPtr->dataPtr, root->dataPtr) < 0)
	{
		// newData < root -- go left
		root->left = _insert(tree, root->left, newPtr, taller);

		if (*taller) {
			// Left subtree is taller
			switch (root->bal)
			{	
			case LH:	// Was left high -- rotate
				root = insLeftBal(root, taller);
				break;
			
			case EH:	// Was balanced -- now LH
				root->bal = LH;
				break;
			
			case RH:	// Was right high -- now EH
				root->bal = EH;
				*taller = FALSE;
				break;
			}
		}
		return root;
	}	// new < node
	else
		// new data >= root data
	{
		root->right = _insert(tree, root->right, newPtr, taller);

		if (*taller) {
			// Right subtree is taller
			switch (root->bal)
			{
			case LH:	// Was left high -- now EH
				root->bal = EH;
				*taller = FALSE;
				break;

			case EH:	// Was balanced -- now RH
				root->bal = RH;
				break;

			case RH:	// Was right high -- rotate
				root = insRightBal(root, taller);
				break;
			}
		}
		return root;
	}	// else new data >= root data

	return root;
}


avl_NODE* insLeftBal(avl_NODE* root, BOOL* taller) {
	avl_NODE* rightTree;
	avl_NODE* leftTree;

	leftTree = root->left;
	switch (leftTree->bal)
	{
	case LH:	// Left High--Rotate Right
		root->bal		= EH;
		leftTree->bal	= EH;

		//Rotate right
		root	= rotateRight(root);
		*taller = FALSE;
		break;

	case EH:	// This is an error
		printf("\n\aError in insLeftBal\n");
		exit(100);
	case RH:	// Right High-Requires double
				// rotation: first left, then right
		rightTree = leftTree->right;
		switch (rightTree->bal)
		{
		case LH:
			root->bal = RH;
			leftTree->bal = EH;
			break;
		case EH:
			root->bal = EH;
			leftTree->bal = LH;
			break;
		case RH:
			root->bal = EH;
			leftTree->bal = LH;
			break;
		}	// switch rightTree

		rightTree->bal = EH;
		// Rotate Left
		root->left = rotateLeft(leftTree);

		// Rotate Right
		root = rotateRight(root);
		*taller = FALSE;
	} // switch

	return root;
}	// leftBalance


// mirror of insLeftBal
avl_NODE* insRightBal(avl_NODE* root, BOOL* taller) {
	avl_NODE* rightTree;
	avl_NODE* leftTree;

	rightTree = root->right;
	switch (rightTree->bal)
	{
	case RH:	// Left High--Rotate Right
		root->bal = EH;
		rightTree->bal = EH;

		//Rotate right
		root = rotateLeft(root);
		*taller = FALSE;
		break;

	case EH:	// This is an error
		printf("\n\aError in insRightBal\n");
		exit(100);
	case LH:	// Right High-Requires double
		// rotation: first left, then right
		leftTree = rightTree->left;
		switch (leftTree->bal)
		{
		case RH:
			root->bal = LH;
			rightTree->bal = EH;
			break;
		case EH:
			root->bal = EH;
			rightTree->bal = RH;
			break;
		case LH:
			root->bal = EH;
			rightTree->bal = RH;
			break;
		}	// switch rightTree

		leftTree->bal = EH;
		// Rotate Left
		root->right = rotateRight(rightTree);

		// Rotate Right
		root = rotateLeft(root);
		*taller = FALSE;
	} // switch

	return root;
}	// rightBalance

//RR
avl_NODE* rotateLeft(avl_NODE* root) {
	avl_NODE* tempPtr;

	tempPtr			= root->right;
	root->right		= tempPtr->left;
	tempPtr->left	= root;

	return tempPtr;
}	// rotate Left

//LL
avl_NODE* rotateRight(avl_NODE* root) {
	avl_NODE* tempPtr;

	tempPtr			= root->left;
	root->left		= tempPtr->right;
	tempPtr->right	= root;

	return tempPtr;
}	// rotate Right


BOOL AVL_Delete(AVL_TREE* tree, void* dltKey) {
	BOOL shorter;
	BOOL success;
	avl_NODE* newRoot;

	newRoot = _delete(tree, tree->root, dltKey, &shorter, &success);

	if (success) {
		tree->root = newRoot;
		(tree->count)--;
		return TRUE;
	}	// if
	else
		return FALSE;
}	// AVL_Delete

avl_NODE* _delete(AVL_TREE* tree, avl_NODE* root, void* dltKey, BOOL* shorter, BOOL* success) {
	avl_NODE* dltPtr;
	avl_NODE* exchPtr;
	avl_NODE* newRoot;

	if (!root) {
		*shorter = FALSE;
		*success = FALSE;
		return NULL;
	}

	if (tree->compare(dltKey, root->dataPtr) < 0) {
		root->left = _delete(tree, root->left, dltKey, shorter, success);
		if (*shorter)
			root = dltRightBal(root, shorter);
	}	// if less
	else if (tree->compare(dltKey, root->dataPtr) > 0) {
		root->right = _delete(tree, root->right, dltKey, shorter, success);
		if (*shorter)
			root = dltLeftBal(root, shorter);
	}	// if greater
	else {
		// Found equal node
		dltPtr = root;
		if (!root->right) {
			// Only left subtree
			newRoot = root->left;
			*success = TRUE;
			*shorter = TRUE;
			free(dltPtr);
			return newRoot;		// base case
		}	// if true
		else {
			if (!root->left) {
				// Only right subtree
				newRoot = root->right;
				*success = TRUE;
				*shorter = TRUE;
				free(dltPtr);
				return newRoot;
			}	// if
			else {
				// Delete Node has two subtrees
				exchPtr = root->left;
				while (exchPtr->right)
					exchPtr = exchPtr->right;
				root->dataPtr = exchPtr->dataPtr;
				root->left = _delete(tree, root->left, exchPtr->dataPtr, shorter, success);
				if (*shorter)
					root = dltRightBal(root, shorter);
			}	// else
		}	// equal node
	}
	return root;
}	// _delete


avl_NODE* dltRightBal(avl_NODE* root, BOOL* shorter) {
	avl_NODE* rightTree;
	avl_NODE* leftTree;

	switch (root->bal) {
	case LH:		// Deleted Left--Now balanced
		root->bal = EH;
		break;

	case EH:		// Now Right high
		root->bal	= RH;
		*shorter	= FALSE;
		break;

	case RH:		// Right High - Rotate Left
		rightTree = root->right;
		if (rightTree->bal == LH) {
			// Double rotation required

			leftTree = rightTree->left;

			switch (leftTree->bal) {
			case LH:
				rightTree->bal	= RH;
				root->bal		= EH;
				break;

			case EH:
				root->bal		= EH;
				rightTree->bal	= EH;
				break;

			case RH:
				root->bal		= LH;
				rightTree->bal	= EH;
				break;
			}	// switch

			leftTree->bal = EH;

			// Rotate Right then Left
			root->right = rotateRight(rightTree);
			root = rotateLeft(root);
		}	// if rightTree->bal == LH
		
		else {
			// Single Rotation Only
			switch (rightTree->bal) {
			case LH:
			case RH: 
				root->bal		= EH;
				rightTree->bal	= EH;
				break;
			case EH:
				root->bal		= RH;
				rightTree->bal	= LH;
				*shorter = FALSE;
				break;				
			}	// switch rightTree->bal

			root = rotateLeft(root);
		}	// else
	}	// switch
	return root;
}	// dltRightBal


// mirror of dltRightBal
avl_NODE* dltLeftBal(avl_NODE* root, BOOL* shorter) {
	avl_NODE* rightTree;
	avl_NODE* leftTree;

	switch (root->bal) {
	case RH:		// Deleted Left--Now balanced  ->R
		root->bal = EH;
		break;

	case EH:		// Now Right high -> LEFT
		root->bal = LH;
		*shorter = FALSE;
		break;

	case LH:		// Right High - Rotate Left   -> L
		leftTree = root->left;
		if (leftTree->bal == RH) {
			// Double rotation required

			rightTree = leftTree->right;

			switch (rightTree->bal) {
			case RH:
				leftTree->bal = LH;
				root->bal = EH;
				break;

			case EH:
				root->bal = EH;
				leftTree->bal = EH;
				break;

			case LH:
				root->bal = RH;
				leftTree->bal = EH;
				break;
			}	// switch

			rightTree->bal = EH;

			// Rotate Right then Left		-> left then right
			root->left = rotateLeft(leftTree);
			root = rotateRight(root);
		}	// if rightTree->bal == LH		-> left, RH

		else {
			// Single Rotation Only
			switch (leftTree->bal) {
			case RH:
			case LH:
				root->bal = EH;
				leftTree->bal = EH;
				break;
			case EH:
				root->bal = LH;
				leftTree->bal = RH;
				*shorter = FALSE;
				break;
			}	// switch rightTree->bal	-> left

			root = rotateRight(root);
		}	// else
	}	// switch
	return root;
}	// dltLeftBal


void* AVL_Retrieve(AVL_TREE* tree, void* keyPtr) {
	if (tree->root)
		return _retrieve(tree, keyPtr, tree->root);
	else
		return NULL;
}	// AVL_Retrieve

void* _retrieve(AVL_TREE* tree, void* keyPtr, avl_NODE* root) {
	
	if (root) {
		if (tree->compare(keyPtr, root->dataPtr) < 0)
			return _retrieve(tree, keyPtr, root->left);
		else if (tree->compare(keyPtr, root->dataPtr) > 0)
			return _retrieve(tree, keyPtr, root->right);
		else
			// Found equal key
			return root->dataPtr;
	}
	else
		// Data not in tree
		return NULL;
}	// _RETRIEVE

void AVL_Travers(AVL_TREE* tree, void(*process)(void* dataPtr)) {
	_traversal(tree->root, process);
	return;
}	// end AVL_Traverse


void _traversal(avl_NODE* root, void(*process)(void* dataPtr)) {

	if (root) {
		_traversal(root->left, process);
		process(root->dataPtr);
		_traversal(root->right, process);
	}	// if

	return;
}	// _traversal


BOOL AVL_Empty(AVL_TREE* tree) {
	return (tree->count == 0);
}

BOOL AVL_Full(AVL_TREE* tree) {
	avl_NODE* newPtr;

	newPtr = (avl_NODE*)malloc(sizeof(*(tree->root)));
	if (newPtr) {
		free(newPtr);
		return FALSE;
	}
	else
		return TRUE;
}

int AVL_Count(AVL_TREE* tree) {
	return (tree->count);
}

AVL_TREE* AVL_Destroy(AVL_TREE* tree) {
	
	if (tree)
		_destory(tree->root);

	// ALL nodes deleted. Free structure
	free(tree);
	return NULL;
}

void _destory(avl_NODE* root) {

	if (root) {
		_destory(root->left);
		free(root->dataPtr);
		_destory(root->right);
		free(root);
	}
	return;
}

