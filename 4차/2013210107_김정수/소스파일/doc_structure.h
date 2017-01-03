#include <string.h>
#include <map>			// version 3. STL - map !

typedef int BOOL;

#define TRUE 1
#define FALSE 0

#define WORD_SIZE 68

#define MU 1300;

// ==============================================
// Header for functions :
// Find Relative Documents
// ==============================================


// ==================================================================
// 1.
// FOR LIST(QUEUE) STRUCTURE VERSION.
// ==================================================================

// information of relative DOCUMENTS
typedef struct doc_node {

	int doc_ID;		// document ID
	int doc_lenth;	// total num of words in document
	int num_words;	// number of relative words
	float weight;	// total weight of document - query

	struct doc_node* next;		// next Node in the List

} DOC_NODE;


// list of document_information
typedef struct {

	int count;		// number of Word.
	DOC_NODE *pos;		// current word
	DOC_NODE *head;		// head word
	DOC_NODE *rear;		// rear word

} DOC_LIST;



// Create List
DOC_LIST* createDocList() {

	DOC_LIST* list;

	list = (DOC_LIST*)malloc(sizeof(DOC_LIST));
	if (list) {
		list->head = NULL;
		list->pos = NULL;
		list->rear = NULL;
		list->count = 0;
	}

	return list;
}


// ADD NODE in DOC LIST by document number
// like QUEUE, insert BACK
static BOOL _insert_back(DOC_LIST* pList, int doc_id, float weight) {
	DOC_NODE* pNew;

	if (!(pNew = (DOC_NODE*)malloc(sizeof(DOC_NODE))))
		return FALSE;

	// Setting New Node
	pNew->doc_ID = doc_id;	// set document id
	pNew->weight = weight;		// set weight
	pNew->doc_lenth = 0;	// set document length
	pNew->num_words = 1;
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
BOOL addNode(DOC_LIST* pList, int doc_id, float weight) {
	DOC_NODE* pNow;		// now position
	pNow = pList->head;

	// CHECK MATCHING doc_id, if exist, do it
	while (pNow != NULL) {

		if (doc_id == pNow->doc_ID) {
			// add weight += new weight
			pNow->weight += weight;
			pNow->num_words++;	// count++ number of words
			return TRUE;
		}

		// GO next word
		pNow = pNow->next;
	}

	// If not EXIST, insert_back 
	_insert_back(pList, doc_id, weight);
	return TRUE;

}


DOC_LIST* destroyList(DOC_LIST* pList) {
	DOC_NODE* deletePtr;

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

DOC_LIST* cleanList(DOC_LIST* pList) {
	DOC_NODE* deletePtr;

	if (pList)
	{
		while (pList->count > 0) {
			//			free(pList->head->word);

			deletePtr = pList->head;
			pList->head = pList->head->next;
			pList->count--;
			free(deletePtr);
		}

		//		free(pList); -> in clean do not FREE.
	}

	return NULL;
}


// ==================================================================
// 3.
// FOR STL _ MAP VERSION. -RB TREE..
// ==================================================================

typedef struct {
	//	char word[51];
	//	int count;
	int doc_lenth;	// total num of words in document
	int num_words;	// number of relative words
	float weight;	// total weight of document - query
	//	char doc_ID[20];// document ID

} DATA;


void clearMAP(map<int, DATA*>& mapEx) {

	map<int, DATA*>::iterator iter;

	// delete
	for (iter = mapEx.begin(); iter != mapEx.end(); iter++) {
		free(iter->second);
	}
	mapEx.clear();

	return;
}

void resetMAP(map<int, DATA*>& mapEx) {

	map<int, DATA*>::iterator iter;

	// reset
	for (iter = mapEx.begin(); iter != mapEx.end(); iter++) {
		iter->second->doc_lenth = 0;
		iter->second->num_words = 0;
		iter->second->weight = 0;
	}
	return;
}

void destoryMAP(map<float, int>& mapEx) {

	mapEx.clear();
	delete &mapEx;

	return;
}

void destoryMAP(map<int, string>& mapEx) {

	mapEx.clear();
	delete &mapEx;

	return;
}

void destoryMAP(map<float, string>& mapEx) {

	mapEx.clear();
	delete &mapEx;

	return;
}


void destoryMAP(map<int, DATA*>& mapEx) {

	clearMAP(mapEx);
	delete &mapEx;

}

BOOL addDATA(map<int, DATA*>& mapEx, int doc_id, float weight) {
	DATA* aDOC;

	map<int, DATA*>::iterator iter = mapEx.find(doc_id);

	if (iter != mapEx.end()) {
		(iter->second->weight) += weight;
		(iter->second->num_words)++;
	}
	else {

		aDOC = (DATA*)malloc(sizeof(DATA));
		if (!aDOC) {
			printf("ERROR addDATA function ... !!\n");
			exit(120);
		}
		// ADD doc id to map
		//		aDOC->doc_ID[0] = '\0';
		aDOC->weight = weight;
		aDOC->doc_lenth = 0;	// later change.
		aDOC->num_words = 1;

		mapEx.insert(make_pair(doc_id, aDOC));

	}

}

float compute_score_VSM(float doc_f, float query_f) {

	float score = doc_f * query_f;

	return score;
}


float compute_weight_BM25_first(float query_f) {
	float R = 0;
	float r = 0;
	float k2 = 500; // 0< k2 <1000
	float N = 554028;	// all number of documents.
	float n = 0;	// = DF		doc_f = TF

	float score = log10((r + 0.5) * (N - n - R + r + 0.5) / (R - r + 0.5) / (n - r + 0.5))
		*(k2 + 1)*query_f / (k2 + query_f);


	return score;

}

float compute_weight_BM25_second(float score, int dl, float doc_f) {

	float b = 0.75;
	float k1 = 1.2;
	float avdl = 304.7;	// already computed...
	float K = k1 * ((1 - b) + b*dl / avdl);

	score = score * (k1 + 1) * doc_f / (K + doc_f);
	return score;
}

// Language Model weight
float compute_weight_LM_first(float TF, float CF, float query_f) {

	float mu = MU;
	float total_C = 304.7 * 554028;

	float score = log2(TF + (mu*CF / total_C)) * query_f;

	return score;

}

float compute_weight_LM_second(float score, int DOC_LEN, int num_words) {

	float mu = MU;

	float sub = (log2(DOC_LEN + mu)) * num_words;

	float result = score - sub;

	return result;

}