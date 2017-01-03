#include <string.h>
#include <map>

typedef int BOOL;

#define TRUE 1
#define FALSE 0

#define WORD_SIZE 68

// word_information form
// Struct size 64 -> 88
typedef struct data_word {

	int CF;			// Collection Frequency
	int ID;			// word ID
	int start;		// Location of start
	BOOL isFirst;	// if True, DF++ 
	int DF;			// Document	Frequency
	int TF;			// Term	Frequency

	//	char word[WORD_SIZE];	// word


} DATA_WORD;


void clearMAP(map<string, DATA_WORD*>& mapWord) {

	map<string, DATA_WORD*>::iterator iter;

	// delete
	for (iter = mapWord.begin(); iter != mapWord.end(); iter++) {
		free(iter->second);
	}
	mapWord.clear();

	return;
}

void destoryMAP(map<string, DATA_WORD*>& mapWord) {

	clearMAP(mapWord);
	delete &mapWord;

}

BOOL addWORD(map<string, DATA_WORD*>& mapWord, char* word, int word_id, int DF, int CF, int start) {
	DATA_WORD* aWord;

	//	map<string, DATA_WORD*>::iterator iter = mapWord.find(word);

	//	if (iter != mapWord.end()) {

	//	}
	//	else {

	aWord = (DATA_WORD*)malloc(sizeof(DATA_WORD));
	if (!aWord) {
		printf("ERROR addWORD function ... !!\n");
		exit(120);
	}
	// ADD word to map

	aWord->ID = word_id;
	aWord->CF = CF;
	aWord->DF = DF;
	aWord->start = start;
	aWord->isFirst = FALSE;
	aWord->TF = 0;

	mapWord.insert(make_pair(word, aWord));

	//	}

}
