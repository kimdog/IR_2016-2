#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

// #include "structure.h"	not use
#include "stopword.h"	// use for removing stopword
// #include "stemming.h" 	not use

#include "api.h"		// use for stemming
#include "stem.h"		// use for stemming

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define _CRT_SECURE_NO_WARNINGS
using namespace std;


// ==========================================================================
// PARSING :
// remove Data except <DOCNO></DOCNO> <HEADLINE></HEADLINE> <TEXT></TEXT>
// SUCCESS - return 1
// FAILURE - return 0
// ==========================================================================

int parsing(char* inputFile, char*outputFile) {

	printf("==============================================\n");
	printf("START PARSING ...\n");

	FILE *fin;		// input file pointer
	FILE *fout;     // output file

	// ===================================================================================
	// OPEN input FILE  and  WRITE in ouput FILE
	// ===================================================================================
	fout = fopen(outputFile, "wt+");
	if (fout == NULL) {
		printf("Create OUTPUT FILE FAIL ...\n");
		return 0;
	}

	printf("LOAD OUTPUT FILE ...\n");

	fin = fopen(inputFile, "rt");
	if (fin == NULL) {
		printf("OPEN INPUT FILE FAIL ...\n");
		return 0;
	}
	else {
		printf("LOAD INPUT FILE ...\n");

		char buf[256];
		char *tok = NULL;

		// remain <DOCNO></DOCNO> <HEADLINE></HEADLINE> <TEXT></TEXT>
		while (fgets(buf, 256, fin) != NULL) {

			tok = strtok(buf, " |\t|\n");
			while (tok != NULL) {

				// <DOCNO>
				if (strcmp(tok, "<DOCNO>") == 0) {
					//					printf("[DOCNO] : ");
					fprintf(fout, "[DOCNO] : ");

					tok = strtok(NULL, " |\t|\n");
					while (strcmp(tok, "</DOCNO>") != 0) {
						//						printf("%s", tok);
						fprintf(fout, "%s", tok);
						tok = strtok(NULL, " |\t|\n");
					}
					//					printf("\n");
					fprintf(fout, "\n");

				}

				// <HEADLINE>
				else if (strcmp(tok, "<HEADLINE>") == 0) {
					//					printf("[HEADLINE] :");
					fprintf(fout, "[HEADLINE] :");

					tok = strtok(NULL, " |\t|\n");
					while (strcmp(tok, "</HEADLINE>") != 0) {
						//						printf(" %s", tok);
						fprintf(fout, " %s", tok);

						tok = strtok(NULL, " |\t|\n");
					}
					//					printf("\n");
					fprintf(fout, "\n");

				}

				// <TEXT>
				else if (strcmp(tok, "<TEXT>") == 0) {
					//					printf("[TEXT] :\n");
					fprintf(fout, "[TEXT] :\n");

					//					tok = strtok(NULL, " |\t|\n");
					while (fgets(buf, 256, fin) != NULL) {
						tok = strtok(buf, " |\t|\n");
						if (tok == NULL)
							continue;
						else if (strcmp(tok, "</TEXT>") == 0)
							break;
						else if (strcmp(tok, "<P>") == 0)
							continue;
						else if (strcmp(tok, "</P>") == 0)
							continue;
						else if (strcmp(tok, "<SUBHEAD>") == 0)
							continue;
						else if (strcmp(tok, "</SUBHEAD>") == 0)
							continue;

						while (tok != NULL) {
							//							printf(" %s", tok);
							fprintf(fout, "%s ", tok);

							tok = strtok(NULL, " |\t|\n");
						}
						fprintf(fout, "\n");
					}

					//					printf("\n\n");
					fprintf(fout, "\n\n");
				}

				else {
					tok = strtok(NULL, " |\t|\n");
				}
			}
		}

		fclose(fin);
		fclose(fout);
		printf("FINISH PARSING ...\n");
		printf("==============================================\n");
	}
	return 1;
}




// ==========================================================================
// REMOVE STOPWORD :
// remove stopwords from Parsed File
// SUCCESS - return 1
// FAILURE - return 0
// ==========================================================================

int remove_stopword(char* inputFile, char*outputFile) {
	printf("==============================================\n");
	printf("START REMOVING STOPWORD ...\n");

	FILE *fin;		// input file pointer
	FILE *fout;     // output file

	fout = fopen(outputFile, "wt+");
	if (fout == NULL) {
		printf("Create OUTPUT FILE FAIL ...\n");
		return 0;
	}

	printf("LOAD OUTPUT FILE ...\n");

	fin = fopen(inputFile, "rt");
	if (fin == NULL) {
		printf("OPEN INPUT FILE FAIL ...\n");
		return 0;
	}
	else {
		printf("LOAD INPUT FILE ...\n");

		char buf[256];
		char *tok = NULL;

		while (fgets(buf, 256, fin) != NULL) {

			tok = strtok(buf, " |\t|\n");
			while (tok != NULL) {

				// <DOCNO>
				if (strcmp(tok, "[DOCNO]") == 0) {
					fprintf(fout, "%s :", tok);
					tok = strtok(NULL, " |\t|\n");
					tok = strtok(NULL, " |\t|\n");

					while (tok != NULL) {
						fprintf(fout, " %s", tok);
						tok = strtok(NULL, " |\t|\n");
					}
					fprintf(fout, "\n");
				}

				// <HEADLINE>
				else if (strcmp(tok, "[HEADLINE]") == 0) {
					fprintf(fout, "%s :", tok);
					tok = strtok(NULL, " |\t|\n");
					tok = strtok(NULL, " |\t|\n.,!?:-()_");

					while (tok != NULL) {
						change_to_low(tok);		// change CAPITAL ALPHABET to LOW			
						// if stop word pass
						if (is_stopword(tok)) {
							tok = strtok(NULL, " |\t|\n.,!?:-()_");
						}
						else {
							fprintf(fout, " %s", tok);
							tok = strtok(NULL, " |\t|\n.,!?:-()_");
						}
					}
					
					fprintf(fout, "\n");
				}

				// <TEXT>
				else if (strcmp(tok, "[TEXT]") == 0) {
					fprintf(fout, "[TEXT] :\n");
					while (fgets(buf, 256, fin) != NULL) {
						change_to_low(buf);		// change CAPITAL ALPHABET to LOW
						tok = strtok(buf, " |\t|\n.,!?:-()_");
						
						// if no word, finish TEXT
						if (tok == NULL)
							break;

						while (tok != NULL) {
							// if stop word pass
							if (is_stopword(tok)) {
								tok = strtok(NULL, " |\t|\n.,!?:-()_");
							}
							else {
								fprintf(fout, "%s ", tok);
								tok = strtok(NULL, " |\t|\n.,!?:-()_");
							}
						}
						// finish one line
						fprintf(fout, "\n");
					}

					fprintf(fout, "\n\n");
				}

				else {
					tok = strtok(NULL, " |\t|\n");
				}
			}
		}

		fclose(fin);
		fclose(fout);
		printf("FINISH REMOVING STOPWORD ...\n");
		printf("==============================================\n");

	}

	return 1;
}



// ==========================================================================
// STEMMING :
// STEMMING from REMOVING STOPWORD File
// USING SNOWBALL - The English (Porter2) stemming algorithm
// SUCCESS - return 1
// FAILURE - return 0
// ==========================================================================

#define INC 10

static void stem_file(struct SN_env * z, FILE * f_in, FILE * f_out) {

	int lim = INC;
	symbol * b = (symbol *)malloc(lim * sizeof(symbol));

	while (1) {
		int ch = getc(f_in);
		if (ch == EOF) {
			free(b);
			return;
		}
		
		{
			int i = 0;
			BOOL is_space;

			while (1) {
				if (ch == ' ') {
					is_space = TRUE;
					break;
				}
				else if (ch == '\n' || ch == EOF) {
					is_space = FALSE;
					break;
				}

				if (i == lim) {  /* make b bigger */
					symbol * q = (symbol *)malloc((lim + INC) * sizeof(symbol));
					memmove(q, b, lim * sizeof(symbol));
					free(b); b = q;
					lim = lim + INC;
				}
				b[i] = ch;
				i++;
				ch = getc(f_in);
			}

			SN_set_current(z, i, b);
			english_ISO_8859_1_stem(z);
			{
				int j;
				for (j = 0; j < z->l; j++) fprintf(f_out, "%c", z->p[j]);
				if (is_space) 
					fprintf(f_out, " ");
				else
					fprintf(f_out, "\n");
			}
		}
	}
}

static int eq(char * s1, char * s2) {
	int s1_len = strlen(s1);
	int s2_len = strlen(s2);
	return s1_len == s2_len && memcmp(s1, s2, s1_len) == 0;
}

static void show_options(int n) {
	printf("options are: file [-o[utput] file] [-h[elp]]\n");
	exit(n);
}

// Main Stemming Function
int stemming(char* inputFile, char*outputFile) {

	printf("==============================================\n");
	printf("START STEMMING ...\n");

//	char * in = 0;
//	char * out = 0;

	/* initialise the stemming process: */

	struct SN_env * z = english_ISO_8859_1_create_env();
	FILE * f_in;
	FILE * f_out;
	f_in = fopen(inputFile, "r");
	if (f_in == NULL) {
		fprintf(stderr, "file %s not found\n", inputFile); return 0;
	}
	f_out = fopen(outputFile, "w");
	if (f_out == NULL) {
		fprintf(stderr, "file %s cannot be opened\n", outputFile); return 0;
	}

	// start stemming process
	stem_file(z, f_in, f_out);
	english_ISO_8859_1_close_env(z);


	printf("FINISH STEMMING ...\n");
	printf("==============================================\n");

	return 1;

}



// ===================================================
// Get Result function
// : PRINT TARGET INFORMATION
//   3 Lines ..
// ===================================================


int get_result(char* inputFile1, char* inputFile2, char* outputFile, char* target) {
	printf("==============================================\n");
	printf("Get Result ...\n");

	FILE *fin;		// input file pointer
	FILE *fout;     // output file

	fout = fopen(outputFile, "wt+");
	if (fout == NULL) {
		printf("Create OUTPUT FILE FAIL ...\n");
		return 0;
	}

	// =============================================
	// Print Stopword File
	// =============================================

	fprintf(fout, "Original Text :\n");

	fin = fopen(inputFile1, "rt");
	if (fin == NULL) {
		printf("OPEN INPUT FILE FAIL ...\n");
		return 0;
	}
	else {

		char buf[256];
		char line[256];
		char *tok = NULL;

		while (fgets(buf, 256, fin) != NULL) {
			strcpy(line, buf);					// store original line.
			tok = strtok(buf, " |\t|\n");
			if (tok != NULL) {
				if (strcmp(tok, "[DOCNO]") == 0) {	// check DOCNO : Target
					tok = strtok(NULL, " |\t|\n");	// Start process
					tok = strtok(NULL, " |\t|\n");
					if (strcmp(tok, target) == 0) {
						fprintf(fout, "%s", line);
						// print [HEADLINE]
						fgets(buf, 256, fin);
						fprintf(fout, "%s", buf);
						// print [TEXT] + 3 Lines
						fgets(buf, 256, fin);
						fprintf(fout, "%s", buf);
						fgets(buf, 256, fin);
						fprintf(fout, "%s", buf);
						fgets(buf, 256, fin);
						fprintf(fout, "%s", buf);
						fgets(buf, 256, fin);
						fprintf(fout, "%s", buf);

						break;						// End Process
					}
				}
			}
		}

		fclose(fin);

	}

	
	// =============================================
	// Print Stemming File
	// =============================================

	fprintf(fout, "==============================================================\n");
	fprintf(fout, "Stemming Text :\n");

	fin = fopen(inputFile2, "rt");
	if (fin == NULL) {
		printf("OPEN INPUT FILE FAIL ...\n");
		return 0;
	}
	else {

		char buf[256];
		char line[256];
		char *tok = NULL;

		while (fgets(buf, 256, fin) != NULL) {
			strcpy(line, buf);					// store original line.
			tok = strtok(buf, " |\t|\n");
			if (tok != NULL) {
				if (strcmp(tok, "[DOCNO]") == 0) {	// check DOCNO : Target
					tok = strtok(NULL, " |\t|\n");	// Start process
					tok = strtok(NULL, " |\t|\n");
					if (strcmp(tok, target) == 0) {
						fprintf(fout, "%s", line);
						// print [HEADLINE]
						fgets(buf, 256, fin);
						fprintf(fout, "%s", buf);
						// print [TEXT] + 3 Lines
						fgets(buf, 256, fin);
						fprintf(fout, "%s", buf);
						fgets(buf, 256, fin);
						fprintf(fout, "%s", buf);
						fgets(buf, 256, fin);
						fprintf(fout, "%s", buf);
						fgets(buf, 256, fin);
						fprintf(fout, "%s", buf);

						break;						// End Process
					}
				}
			}
		}

		fclose(fin);
	}

	fclose(fout);

	return 1;

}

// ===================================================
// Main Program Function
// ===================================================

int main(int argc, char* argv[]) 
{
	char inputFile[256];
	char outputFile[256];

	// CHECK each process SUCCESS
	// SUCCESS	= 1
	// FAIL		= 0
	int check_process = 0;
	
	// ================================================
	// PARSING
	// ================================================
	
	// set input, output file DIRECTORY
	//	strcpy(inputFile, "C:/Users/±èÁ¤¼ö/Desktop/4-2/ir_corpus/APW/20000930_APW_ENG");
	//	strcpy(outputFile, "C:/Users/±èÁ¤¼ö/Desktop/4-2/ir_corpus/APW/parsing.txt");

	strcpy(inputFile, "../20000930_APW_ENG");
	strcpy(outputFile, "../parsing.txt");


	check_process = parsing(inputFile, outputFile);
	if (check_process) 
		printf("PARSING SUCCESS ...\n");
	else 
		printf("PARSING FAIL ...\n");


	
	// ===============================================
	// STOPWORD ............
	// ===============================================
	check_process = 0;

	// set input, output file DIRECTORY
	//	strcpy(inputFile, "C:/Users/±èÁ¤¼ö/Desktop/4-2/ir_corpus/APW/parsing.txt");
	//	strcpy(outputFile, "C:/Users/±èÁ¤¼ö/Desktop/4-2/ir_corpus/APW/stopword.txt");

	strcpy(inputFile, "../parsing.txt");
	strcpy(outputFile, "../stopword.txt");

	check_process = remove_stopword(inputFile, outputFile);
	if (check_process)
		printf("STOPWORD SUCCESS ...\n");
	else
		printf("STOPWORD FAIL ...\n");

	
	
	// ===============================================
	// STEMMING ............
	// ===============================================
	check_process = 0;

	// set input, output file DIRECTORY
	//	strcpy(inputFile, "C:/Users/±èÁ¤¼ö/Desktop/4-2/ir_corpus/APW/stopword.txt");
	//	strcpy(outputFile, "C:/Users/±èÁ¤¼ö/Desktop/4-2/ir_corpus/APW/stem.txt");

	strcpy(inputFile, "../stopword.txt");
	strcpy(outputFile, "../stem.txt");


	check_process = stemming(inputFile, outputFile);
	if (check_process)
		printf("STEMMING SUCCESS ...\n");
	else
		printf("STEMMING FAIL ...\n");
	


	// ===============================================
	// PRINT RESULT TWO LINES ...........
	// ===============================================
	check_process = 0;
	
	// set input, output file DIRECTORY
	char inputFile1[256];		// stopword file
	char inputFile2[256];		// stemming file
	
	//	strcpy(inputFile1, "C:/Users/±èÁ¤¼ö/Desktop/4-2/ir_corpus/APW/stopword.txt");
	//	strcpy(inputFile2, "C:/Users/±èÁ¤¼ö/Desktop/4-2/ir_corpus/APW/stem.txt");
	//	strcpy(outputFile, "C:/Users/±èÁ¤¼ö/Desktop/4-2/ir_corpus/APW/stemming.txt");

	strcpy(inputFile1, "../stopword.txt");
	strcpy(inputFile2, "../stem.txt");
	strcpy(outputFile, "../stemming.txt");



	char DOC_num[256];			// Target Document Number
	strcpy(DOC_num, "APW20000930.0005");

	check_process = get_result(inputFile1, inputFile2, outputFile, DOC_num);
	if (check_process)
		printf("Get Result SUCCESS ...\n");
	else
		printf("Get Result FAIL ...\n");



	// ===============================================
	// MY STEMMING ...........
	// SKIP ....
	// ===============================================

	printf("\nPROGRAM END!!!\n");
	system("pause");

	return 0;
}
