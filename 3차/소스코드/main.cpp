#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <Windows.h>
#include <time.h>		// for Get processing Time
#include <math.h>		// compute Weight

using namespace std;


#include "stopword.h"	// use for removing stopword
#include "api.h"		// use for stemming
#include "stem.h"		// use for stemming

#include "avl_tree.h"		// About avl_tree structure
#include "doc_structure.h"	// About relative document list functions
#include "structure.h"		// About data List functions
#include "query_header.h"	// About query process functions


// map
#include <map>

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define _CRT_SECURE_NO_WARNINGS


// each FOLDER path
#define INPUT_FOLDER_PATH "../input/"
#define OUTPUT_FOLDER_PATH "../output/"

#define BUF_SIZE 256
#define PATH_SIZE 64
#define NAME_SIZE 20
#define WORD_SIZE 68

// size of each data in index.txt (inverted index )
#define s_word		6
#define s_doc		6
#define s_TF		3
#define s_weight	7
#define s_line		24
// ex) 0000010002120030.02347
// (6+6+3+7+1) = (1 line) = 23 + 1 = 24


#include "list_header.h"		// HEADER about main functions of List
#include "list_header_bm25.h"

// =====================================================
// input : index.txt  doc.txt  word.txt  topic25.txt
// output : result_vsm.txt  result_bm.txt
// =====================================================

int main() {
	clock_t time;
	time = clock();

	char folderPath[PATH_SIZE];			// Folder of text Files

	// CHECK each process SUCCESS
	// SUCCESS	= 1
	// FAIL		= 0
	int check_process = 0;

	char inputFile[PATH_SIZE];
	char outputFile[PATH_SIZE];

	// =========================================================================
	// Process of QUERY
	// =========================================================================

	// parse
	strcpy(inputFile, INPUT_FOLDER_PATH);
	strcat(inputFile, "topics25.txt");		// get inputFile path
	strcpy(outputFile, OUTPUT_FOLDER_PATH);
	strcat(outputFile, "topics25_parse.txt");// get outputFile path ( parsing )

	printf("Query Process ... \n");
	printf("=======================================\n");

	check_process = parsing(inputFile, outputFile, *topicList);
	if (check_process)
		printf("PARSING SUCCESS ...\n");
	else {
		printf("PARSING FAIL ...\n");
		return 1;
	}

	// stopword
	strcpy(inputFile, outputFile);					// get inputFile path = previous outputFile path
	strcpy(outputFile, OUTPUT_FOLDER_PATH);
	strcat(outputFile, "topics25_stop.txt");		// get outputFile path ( stopword )

	check_process = remove_stopword(inputFile, outputFile);
	if (check_process)
		printf("STOPWORD SUCCESS ...\n");
	else {
		printf("STOPWORD FAIL ...\n");
		return 1;
	}


	// stemming
	strcpy(inputFile, outputFile);					// get inputFile path = previous outputFile path
	strcpy(outputFile, OUTPUT_FOLDER_PATH);
	strcat(outputFile, "topics25_stem.txt");		// get outputFile path ( stemming )

	check_process = stemming(inputFile, outputFile);
	if (check_process)
		printf("STEMMING SUCCESS ...\n");
	else {
		printf("STEMMING FAIL ...\n");
		return 1;
	}

	printf("FINISH !!!\n");
	printf("=======================================\n\n");
	


	// =========================================================================
	// Process of FIND relative DOCUMENTS -> [ rel_DOC_list ]
	// and RANKING
	// =========================================================================


	// LOAD wordList on Memory, ( and create Sub Lists... )
	check_process = loading_wordLists();

	if (check_process)
		printf("Lists LOAD SUCCESS ... !\n");
	else {
		printf("Lists LOAD FAIL ...\n");
		return 1;
	}

	
	char indexFile[PATH_SIZE];	// inverted_index File
	strcpy(indexFile, INPUT_FOLDER_PATH);
	strcat(indexFile, "index.txt");		// get inputFile path

	char topicFile[PATH_SIZE];	// topic File
//	strcpy(topicFile, outputFile);
	strcpy(topicFile, "../output/topics25_stem.txt");

	char docFile[PATH_SIZE];	// document File
	strcpy(docFile, INPUT_FOLDER_PATH);
	strcat(docFile, "doc.txt");

	strcpy(outputFile, OUTPUT_FOLDER_PATH); // output File
	strcat(outputFile, "result.txt");
/*
	check_process = rel_docs_and_ranking(topicFile, indexFile, docFile, outputFile);

	if (check_process)
		printf("Find Relative DOC and RANKING SUCCESS ... !\n");
	else {
		printf("Find Relative DOC and RANKING FAIL ...\n");
		return 1;
	}


	// Show Time
	time = clock() - time;
	printf("\n---- VSM : It took %f seconds ----\n", ((float)time) / CLOCKS_PER_SEC);

	time = clock();

*/

	// =========================================================================
	// BM25 - RANKING
	// =========================================================================

	strcpy(outputFile, OUTPUT_FOLDER_PATH); // output File
	strcat(outputFile, "BM25_result.txt");

	check_process = BM25_rel_docs_and_ranking(topicFile, indexFile, docFile, outputFile);

	if (check_process)
		printf("BM25 : Find Relative DOC and RANKING SUCCESS ... !\n");
	else {
		printf("BM25 : Find Relative DOC and RANKING FAIL ...\n");
		return 1;
	}
	// Show Time
	time = clock() - time;
	printf("\n---- BM25 : It took %f seconds ----\n", ((float)time) / CLOCKS_PER_SEC);

	time = clock();



	// destory all word lists in Memory
	destroy_all_Lists();

	// Show Time
	time = clock() - time;
	printf("\n---- It took %f seconds ----\n", ((float)time) / CLOCKS_PER_SEC);
	system("pause");

	return 0;
}