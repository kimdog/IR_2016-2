#include <string.h>

typedef int BOOL;

#define TRUE 1
#define FALSE 0

#define WORD_SIZE 68

// word_information form
// Struct size 64 -> 88
typedef struct node {

	int CF;			// Collection Frequency
	int ID;			// word ID
	int start;		// Location of start
	BOOL isFirst;	// if True, DF++ 
	int DF;			// Document	Frequency
	int TF;			// Term	Frequency

	struct node* next;		// next Node in the List
	char word[WORD_SIZE];	// word


} NODE;


// list of word_information
typedef struct {

	int count;		// number of Word.
	NODE *pos;		// current word
	NODE *head;		// head word
	NODE *rear;		// rear word

} LIST;



// Create List
LIST* createList() {

	LIST* list;

	list = (LIST*)malloc(sizeof(LIST));
	if (list) {
		list->head = NULL;
		list->pos = NULL;
		list->rear = NULL;
		list->count = 0;
	}

	return list;
}



// ADD NODE in LIST
// like QUEUE, insert BACK
static BOOL _insert_back(LIST* pList, char* word) {
	NODE* pNew;

	if (!(pNew = (NODE*)malloc(sizeof(NODE))))
		return FALSE;

	// Setting New Node
	strcpy(pNew->word, word);
	pNew->ID = (pList->count) + 1;	// set word ID
	pNew->TF = 1;
	pNew->DF = 1;
	pNew->CF = 1;
	pNew->start = 0;
	pNew->isFirst = FALSE;		// this time is First, so change to FALSE
	pNew->next = NULL;

	if (pList->count == 0)
		pList->head = pNew;
	else
		pList->rear->next = pNew;

	(pList->count)++;
	pList->rear = pNew;

	return TRUE;
}

// add NODE NOT ORDER
// if success return TRUE, else FALSE
BOOL addNode(LIST* pList, char* word) {
	NODE* pNow;		// now position
	pNow = pList->head;

	// CHECK MATCHING WORD, if exist, do it
	while (pNow != NULL) {

		if (strcmp(word, pNow->word) == 0) {
			// add count ++
			pNow->TF++;		// reset if next Document
			pNow->CF++;		// Maintain value
			if (pNow->isFirst) {
				pNow->DF++;
				pNow->isFirst = FALSE;	// next time, not DF++
			}
			return TRUE;
		}

		// GO next word
		pNow = pNow->next;
	}

	// If not EXIST, insert_back 
	_insert_back(pList, word);
	return TRUE;

}

BOOL setCountZero(LIST* pList) {
	pList->pos = pList->head;
	while (pList->pos != NULL) {
		pList->pos->TF = 0;
		pList->pos->isFirst = TRUE;
		pList->pos = pList->pos->next;
	}

	return TRUE;
}

BOOL emptyList(LIST* pList) {
	return (pList->count == 0);
}


LIST* destroyList(LIST* pList) {
	NODE* deletePtr;

	if (pList)
	{
		while (pList->count > 0) {
			//			free(pList->head->word);

			deletePtr = pList->head;
			pList->head = pList->head->next;
			pList->count--;
			free(deletePtr);
		}

		free(pList);
	}

	return NULL;
}


int print_type(float f) {
	if (f < 10)
		return 3;
	else if (f < 100)
		return 2;
	else if (f < 1000)
		return 1;
	else
		return 0;

}

NODE* find_word_inList(LIST* list, char* term) {

	list->pos = list->head;
	// get pos matching term
	while (strcmp(list->pos->word, term) != 0) {
		list->pos = list->pos->next;
	}

	return list->pos;
}

void sub_node_insert(LIST* sub_list, NODE* node) {
	_insert_back(sub_list, node->word);
	sub_list->rear->CF = node->CF;
	sub_list->rear->DF = node->DF;
	sub_list->rear->ID = node->ID;
	sub_list->rear->start = node->start;
	// Now we use variance [pos->TF] for count term UNTIL DF 
	sub_list->rear->TF = 0;	// reset
}