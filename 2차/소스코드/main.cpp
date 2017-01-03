#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <Windows.h>
#include <time.h>		// for Get processing Time
#include <math.h>		// compute Weight
#include <map>

#include "structure.h"	// About data List functions

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define _CRT_SECURE_NO_WARNINGS
using namespace std;

// each FOLDER path
#define INPUT_FOLDER_PATH "../input/"
#define OUTPUT_FOLDER_PATH "../output/"

// SIZE
#define BUF_SIZE 64
#define PATH_SIZE 64
#define NAME_SIZE 20
#define WORD_SIZE 35

// INDEX_SIZE
// index_term	5
// index_doc	6
// TF			2
// Weight		6

static int doc_id = 0;		// document ID : Last ID = Total number of Documents.
// #include "header_map_ver.h"	// map version ( speed test ) --- fail...


// ==========================================================================
// TERM FREQUENCY :
// <DOCNO> (TAB) <WORD> (TAB) <FREQUENCY>
// SUCCESS - return 1
// FAILURE - return 0
// 
// DF, CF : STORE in Memory
// ==========================================================================

int Get_information(char* inputFile, FILE* fout, FILE* fdoc, LIST* list) {
	printf("==============================================\n");
	printf("START GET INFORMATION ...\n");

	FILE *fin;		// input file pointer
	//	FILE *fout;     // output file

	// ===================================================================================
	// OPEN and READ input File and GET TF Information
	// ===================================================================================
	fin = fopen(inputFile, "rt");
	if (fin == NULL) {
		printf("OPEN INPUT FILE FAIL ...\n");
		return 0;
	}
	else {

		printf("LOAD INPUT FILE ... \n");
		printf("WORKING ... \n");

		int num_word = 0;	// Number of words of each DOCUMENT
		char doc_name[NAME_SIZE] = "";	// each NAME of document
		char buf[BUF_SIZE];
		char *tok = NULL;

		// Get Line 
		while (fgets(buf, BUF_SIZE, fin) != NULL) {

			tok = strtok(buf, " |\t|\n");
			while (tok != NULL) {

				// <DOCNO> : set Documnet Name Update
				if (strcmp(tok, "[DOCNO]") == 0) {

					tok = strtok(NULL, " |\t|\n");	// pass :
					tok = strtok(NULL, " |\t|\n");

					strcpy(doc_name, tok);		// get Document Name
					doc_id++;					// get Document ID

					fgets(buf, BUF_SIZE, fin);	// GO next Line
					tok = strtok(buf, " |\t|\n");
					continue;
				}

				// <HEADLINE> : same to <TEXT> NOW ...
				else if (strcmp(tok, "[HEADLINE]") == 0) {

					tok = strtok(NULL, " |\t|\n");	// pass :
					tok = strtok(NULL, " |\t|\n");
					while (tok != NULL) {
						addNode(list, tok);			// add word in LIST
						num_word++;					// count word
						tok = strtok(NULL, " |\t|\n");
					}
					//					printf(".");

				}

				// <TEXT>
				else if (strcmp(tok, "[TEXT]") == 0) {

					while (fgets(buf, BUF_SIZE, fin) != NULL) {
						tok = strtok(buf, " |\t|\n");

						// if no word, finish TEXT
						if (tok == NULL)
							break;

						while (tok != NULL) {
							addNode(list, tok);			// add word in LIST
							num_word++;					// count word
							tok = strtok(NULL, " |\t|\n");
						}
					}
					//					printf(".");

					// FINISH one Document, then PRINT TF information
					list->pos = list->head;
					while (list->pos != NULL) {
						if (!(list->pos->isFirst))
							fprintf(fout, "%s\t%s\t%d\n", doc_name, list->pos->word, list->pos->TF);
						list->pos = list->pos->next;
					}
					setCountZero(list);		// RESET : TF , isFirst

					// PRINT DOCUMENT INFORMATION in doc.dat
					fprintf(fdoc, "%d\t%s\t%d\n", doc_id, doc_name, num_word);
					num_word = 0; // reset number of word
				}

				else {
					tok = strtok(NULL, " |\t|\n");
				}
			}
		}

		fclose(fin);	// end inputFile
	}

	printf("FINISH GET INFORMATION ...\n");
	printf("==============================================\n");
	return 1;

}

// ===================================================
// Make word information : word.dat
// term_ID	term	DF	CF	Start
// ===================================================

int make_word_info(char* outputFile, LIST* list) {

	printf("CREATE WORD INFORMATION FILE word.dat ...\n");
	
	FILE *fout;     // output file
	fout = fopen(outputFile, "wt+");
	if (fout == NULL) {
		printf("Create OUTPUT FILE FAIL ...\n");
		return 0;
	}

	int start_location = 0;
	
	list->pos = list->head;
	while (list->pos != NULL) {
		list->pos->start = start_location;	// store in memory
		fprintf(fout, "%d\t%s\t%d\t%d\t%d\n", list->pos->ID, list->pos->word, list->pos->DF, list->pos->CF, start_location);
		start_location += list->pos->DF;

		list->pos = list->pos->next;
	}
	
	fclose(fout);
	return 1;

}

// ===================================================
// Compute Weight in Inversed index file
// ---------------------------------------------------
// Total_TF = Length of DOC
// tfk = TF / Total_TF
// N : last doc_id,  nk : DF
// iDF = N / nk
// ===================================================

// get not normalize weight
float get_weight(int TF, int total_TF, int N, int nk) {
	float weight;
	float tf, idf;

	tf = (float)TF / total_TF;
	idf = log(((float) N / nk));
	
	weight = tf * idf;

//	result = log(param);	
//	result = pow(param, 2);
//	result = sqrt(param);
	

	return weight;
}

// Make inverted index file
// ================================
// word_ID	doc_ID	TF	Weight
// ================================
int make_invertIndexFile(char* tfFile, char* docFile, char* outputFile, LIST* list, int last_doc_index) {

	printf("CREATE INVERTED INDEX FILE index.dat ...\n");

	int num_Total_doc = last_doc_index;	// Number of Total Documents

	FILE *ftf;		// TF file
	FILE *fdoc;		// doc file
	FILE *fout;		// outputFile

	// Open TF FILE
	ftf = fopen(tfFile, "rt");
	if (ftf == NULL) {
		printf("OPEN TF FILE FAIL ...\n");
		return 0;
	}

	// Open DOC FILE
	fdoc = fopen(docFile, "rt");
	if (fdoc == NULL) {
		printf("OPEN Document FILE FAIL ...\n");
		return 0;
	}

	// Open OUTPUT FILE
	fout = fopen(outputFile, "wt+");
	if (fout == NULL) {
		printf("OPEN Output FILE FAIL ...\n");
		return 0;
	}


	// process go on...

	char buf[BUF_SIZE];
	char *tok = NULL;

	// each Loop : get 1 record ---- word_ID	doc_ID	TF	Weight
	// Process of Term Memory
	list->pos = list->head;
	while (list->pos != NULL) {
		char doc_name[NAME_SIZE];
		char term[WORD_SIZE];
		int TF;
		int DF;
		int index_term;	// index of Term
		int index_doc;	// index of Document
		int len_doc;	// Number of words in a Document 
		float weight;	// Weight

		int count = 0;		// count How many document is READ
		long ftf_start;		// using ftell(), skip not needed RECORDS

		
		strcpy(term, list->pos->word);	// get Term
		index_term = list->pos->ID;		// get ID
		DF = list->pos->DF;				// get DF

		// Above Data are SAME for Each Term .
		while (fgets(buf, BUF_SIZE, ftf) != NULL) {			// if count ALL document, break;

			if (count >= DF)					// get one line
				break;							// it's for Deleted LINE

			tok = strtok(buf, "\t");	// get doc_name
			strcpy(doc_name, tok);

			tok = strtok(NULL, "\t");	// get term
			if (strcmp(tok, term) != 0)
				continue;				// if Not correct Term, PASS

			// if Correct Term, process Go on.

			tok = strtok(NULL, "\t");	// get TF
			TF = atoi(tok);

			if (count == 0)				// if first Occur
				ftf_start = ftell(ftf);	// get start Position of next ftf

			// Process of DOC.dat
			while (fgets(buf, BUF_SIZE, fdoc) != NULL) {
				tok = strtok(buf, "\t");	// get index_doc
				index_doc = atoi(tok);


				tok = strtok(NULL, "\t");	// get doc_name
				if (strcmp(tok, doc_name) != 0)
					continue;				// if not Correct, PASS

				tok = strtok(NULL, "\t");	// get length of DOC
				len_doc = atoi(tok);

				break;	// END process of doc.dat
			}


			// Compute Weight and PRINT
			weight = get_weight(TF, len_doc, num_Total_doc, DF);

			fprintf(fout, "%05d  %06d  %02d  %06.4f \n", index_term, index_doc, TF, weight);


			count++;
		}

		list->pos = list->pos->next;	// Go Next Term
		// RESET each FILE pointer
		fseek(ftf, ftf_start, SEEK_SET);// Skip not needed RECORDS
		rewind(fdoc);					//		fseek(fdoc, 0L, SEEK_SET);	// Reset this time, because DOCUMENT are ORDERED .
	}

	fclose(ftf);
	fclose(fdoc);
	fclose(fout);

	return 1;
}


// Make inverted index file Version. 2.
// ================================
// word_ID	doc_ID	TF	Weight
// ================================
int make_invertIndexFile_VER_2(char* tfFile, char* docFile, char* outputFile, LIST* list, int last_doc_index) {

	printf("CREATE INVERTED INDEX FILE index.dat ...\n");

	int num_Total_doc = last_doc_index;	// Number of Total Documents

	FILE *ftf;		// TF file
	FILE *fdoc;		// doc file
	FILE *fout;		// outputFile

	// Open TF FILE
	ftf = fopen(tfFile, "rt");
	if (ftf == NULL) {
		printf("OPEN TF FILE FAIL ...\n");
		return 0;
	}

	// Open DOC FILE
	fdoc = fopen(docFile, "rt");
	if (fdoc == NULL) {
		printf("OPEN Document FILE FAIL ...\n");
		return 0;
	}

	// Open OUTPUT FILE
	fout = fopen(outputFile, "wt+");
	if (fout == NULL) {
		printf("OPEN Output FILE FAIL ...\n");
		return 0;
	}


	// process go on...

	char buf[BUF_SIZE];
	char *tok = NULL;

	// Now we use variance [pos->TF] for count term UNTIL DF 
	list->pos = list->head;
	while (list->pos != NULL) {
		list->pos->TF = 0;			// reset count
		list->pos = list->pos->next;
	}


	// Read each LINE of TF.txt and directly WRITE in index.txt
	while (fgets(buf, BUF_SIZE, ftf) != NULL) {
		char doc_name[NAME_SIZE];
		char term[WORD_SIZE];
		int TF;
		int DF;
		int index_term;	// index of Term
		int index_doc;		// index of Document
		int len_doc;		// Number of words in a Document 
		float weight;	// Weight


		tok = strtok(buf, "\t\n");	// get doc_name
		strcpy(doc_name, tok);
	
		tok = strtok(NULL, "\t\n");	// get term
		strcpy(term, tok);
	
		tok = strtok(NULL, "\t\n");	// get TF
		TF = atoi(tok);


		// Process of DOC.txt
		while (fgets(buf, BUF_SIZE, fdoc) != NULL) {
			tok = strtok(buf, "\t\n");	// get index_doc
			index_doc = atoi(tok);

			tok = strtok(NULL, "\t\n");	// get doc_name
			if (strcmp(tok, doc_name) != 0)
				continue;				// if not Correct, PASS

			tok = strtok(NULL, "\t\n");	// get length of DOC
			len_doc = atoi(tok);

			break;	// END process of doc.dat
		}


		// Process of TERM in memory
		list->pos = list->head;
		// get pos matching term
		while (strcmp(list->pos->word, term) != 0) {
			list->pos = list->pos->next;
		}
		DF = list->pos->DF;				// get DF
		index_term = list->pos->ID;		// get index_term

		// Compute Weight and PRINT
		weight = get_weight(TF, len_doc, num_Total_doc, DF);
		long pos_fp = (long) ( 22 * ((list->pos->start) + (list->pos->TF)) );
			// (5+6+2+7+1) = (1 line) = 21 + 1 = 22

		fseek(fout, pos_fp, SEEK_SET);// Go file pointer target RECORD
//		fprintf(fout, "%05d%06d%02d %06.4f\n", index_term, index_doc, TF, weight);
		switch (print_type(weight))
		{
		case 1:
			fprintf(fout, "%05d%06d%02d%06.3f\n", index_term, index_doc, TF, weight);
			break;
		case 2:
			fprintf(fout, "%05d%06d%02d%06.4f\n", index_term, index_doc, TF, weight);
			break;
		default:
			fprintf(fout, "%05d%06d%02d%06.5f\n", index_term, index_doc, TF, weight);
			break;
		}

		list->pos->TF++;	// count TERM for compute WRITE POSITION
		rewind(fdoc);		// reset fdoc File Pointer
	}


	fclose(ftf);
	fclose(fdoc);
	fclose(fout);

	return 1;
}


// ===================================================
// Main Program Function
// ===================================================

int main(int argc, char* argv[])
{
	clock_t time;
	time = clock();

	char folderPath[PATH_SIZE];			// Folder of text Files
	LIST* wordList = createList();		// Information of words
//	map<char*, MAP_NODE> wordMap;		// Information of words (ver. MAP)

//	int word_id = 0;		// word ID

	// CHECK each process SUCCESS
	// SUCCESS	= 1
	// FAIL		= 0
	int check_process = 0;

	// ===================================================================================
	// Set outputFile : TF.txt , doc.dat
	// and OPEN
	// ===================================================================================
	
	// Create Required Directories
	CreateDirectoryA(OUTPUT_FOLDER_PATH, NULL);

	char tfFile[PATH_SIZE];
	char docFile[PATH_SIZE];
	strcpy(tfFile, OUTPUT_FOLDER_PATH);
	strcat(tfFile, "TF.txt");	// get outputFile path ( TF )
	strcpy(docFile, OUTPUT_FOLDER_PATH);
	strcat(docFile, "doc.txt");	// get documentFile path ( information of doc )


	FILE *ftf;		// TF file
	FILE *fdoc;		// doc file

	ftf = fopen(tfFile, "wt+");
	if (ftf == NULL) {
		printf("Create OUTPUT FILE FAIL ...\n");
		return 0;
	}
	printf("LOAD OUTPUT FILE ...\n");

	fdoc = fopen(docFile, "wt+");
	if (fdoc == NULL) {
		printf("Create DOCUMENT FILE FAIL ...\n");
		return 0;
	}
	printf("LOAD DOCUMENT FILE ...\n");


	// ===================================================================================
	// Find inputFiles of Folder and process ...
	// INPUT  Folder	: documents
	// OUTPUT Folder	: TF.txt   doc.dat
	// OUTPUT Data		: Word information ( word ID, DF, CF )
	// ===================================================================================

	HANDLE hFind;
	WIN32_FIND_DATAA currentFile;			// current inputfile of Folder

	strcpy(folderPath, INPUT_FOLDER_PATH);
	strcat(folderPath, "*");		// all files

	hFind = FindFirstFileA(folderPath, &currentFile);

	if (hFind != INVALID_HANDLE_VALUE) {
		FindNextFileA(hFind, &currentFile);				// pass /.		
		while (FindNextFileA(hFind, &currentFile)) {	// pass	/..
			// each File start ..
			printf("**** File : %s ****\n", currentFile.cFileName);

			char inputFile[PATH_SIZE];
			
			// =============================================================
			// GET TF (Term Frequency) and DOCUMENT Inforamtion ( doc.dat )
			// =============================================================

			// set input, output file DIRECTORY

			strcpy(inputFile, INPUT_FOLDER_PATH);
			strcat(inputFile, currentFile.cFileName);	// get inputFile path
			
			check_process = Get_information(inputFile, ftf, fdoc, wordList);
//			wordMap = Get_information_map(inputFile, ftf, fdoc, wordMap);


			if (check_process)
				printf("GET INFORMATION SUCCESS ...\n");
			else {
				printf("GET INFORMATION FAIL ...\n");
				return 1;
			}

			printf("\n");	// Each File END ...
		}
		FindClose(hFind);	// end find Files of Folder
	}

	else {
		printf("INVALID FIND FILE ERROR ...\n");
		return 1;
	}

	fclose(ftf);
	fclose(fdoc);


	// ===================================================================================
	// Now, We have TF.txt doc.dat and Word inforamtion( ID, DF, CF )
	// We should Make [ Inverted index File ]
	// word_ID	doc_ID	TF	Weight
	// ===================================================================================

	// get Word Information File
	char outputFile[PATH_SIZE];
	strcpy(outputFile, OUTPUT_FOLDER_PATH);
	strcat(outputFile, "word.txt");	// get outputFile path ( word.dat )

	check_process = make_word_info(outputFile, wordList);
//	wordMap = make_word_info_map(outputFile, wordMap);

	if (check_process)
		printf("MAKE word.dat SUCCESS ...\n");
	else {
		printf("MAKE word.dat FAIL ...\n");
		return 1;
	}

//	printf("STORE MEM To DISK ...\n");
//	storeList(wordList);	// Store list ... PREVENT ERROR .



	// get INVERTED INDEX FILE
	strcpy(outputFile, OUTPUT_FOLDER_PATH);
	strcat(outputFile, "index.txt");	// get outputFile path ( word.dat )

	check_process = make_invertIndexFile_VER_2(tfFile, docFile, outputFile, wordList, doc_id);
//	check_process = make_invertIndexFile_VER_2_map(tfFile, docFile, outputFile, wordMap, doc_id);


	if (check_process)
		printf("MAKE index.dat SUCCESS ...\n");
	else {
		printf("MAKE index.dat FAIL ...\n");
		return 1;
	}


	// FINISH PROGRAM

	destroyList(wordList);
//	wordMap.clear();

	printf("\nPROGRAM END!!!\n");


	// Show Time
	time = clock() - time;
	printf("\n---- It took %f seconds ----\n", ((float)time) / CLOCKS_PER_SEC);
	system("pause");

	return 0;
}
