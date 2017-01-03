#define BUF_SIZE 256


// ==========================================================================
// PARSING :
// <top> -> each topic
// <num> -> number
// <title> <desc> <narr> -> term
// SUCCESS - return 1
// FAILURE - return 0
// ==========================================================================

int parsing(char* inputFile, char*outputFile, map<int, string>& topicList) {

	FILE *fin;		// input file pointer
	FILE *fout;     // output file

	// ===================================================================================
	// OPEN and READ input File to WRITE in output FILE
	// ===================================================================================
	fout = fopen(outputFile, "wt+");
	if (fout == NULL) {
		printf("Create OUTPUT FILE FAIL ...\n");
		return 0;
	}

	fin = fopen(inputFile, "rt");
	if (fin == NULL) {
		printf("OPEN INPUT FILE FAIL ...\n");
		return 0;
	}
	else {

		//printf("LOAD INPUT FILE ... \n");

		char buf[BUF_SIZE];
		char *tok = NULL;

		// <top> - <num> - ( <title> <desc> <narr> )
		while (fgets(buf, BUF_SIZE, fin) != NULL) {

			tok = strtok(buf, " |\t|\n");
			while (tok != NULL) {

				// <top>
				if (strcmp(tok, "<top>") == 0) {

					// for store topic TITLE
					int top_num;
					string top_title = "";

					fprintf(fout, "<top>\n");

					while (fgets(buf, BUF_SIZE, fin) != NULL) {
						tok = strtok(buf, " |\t|\n");

						if (tok == NULL)
							continue;

						// </top> -> stop.
						if (strcmp(tok, "</top>") == 0) {
							fprintf(fout, "</top>\n");
							break;
						}

						// <num>
						else if (strcmp(tok, "<num>") == 0) {
							tok = strtok(NULL, " |\t|\n"); // pass string Number:
							tok = strtok(NULL, " |\t|\n"); // get number.
							top_num = atoi(tok);
							fprintf(fout, "<num> %d\n", top_num);

						}

						// <title>
						else if (strcmp(tok, "<title>") == 0) {

							tok = strtok(NULL, " |\t|\n");
							while (tok != NULL) {

								fprintf(fout, "%s ", tok);
								top_title += tok;
								top_title += ' ';

								tok = strtok(NULL, " |\t|\n");

								// if 1 Line finished, next line check.
								if (tok == NULL) {
									fgets(buf, BUF_SIZE, fin);
									tok = strtok(buf, " |\t|\n");
									fprintf(fout, "\n");
									//	top_title += "\n";
								} // if next line is NULL, stop while LOOP
							}

						}

						// <desc>, <narr>
						else if ((strcmp(tok, "<desc>") == 0) || (strcmp(tok, "<narr>") == 0)) {

							while (fgets(buf, BUF_SIZE, fin) != NULL) {
								tok = strtok(buf, " |\t|\n");
								if (tok == NULL)
									break;

								while (tok != NULL) {
									fprintf(fout, "%s ", tok);
									tok = strtok(NULL, " |\t|\n");
								}

								fprintf(fout, "\n");
							}
						}
					}

					fprintf(fout, "\n"); // finish 1 topic.
					top_title += '\0';
					topicList.insert(make_pair(top_num, top_title)); // store TOPIC TITLE
				}

				else {
					tok = strtok(NULL, " |\t|\n");
				}
			}
		}

		fclose(fin);	// end inputFile
		fclose(fout);	// end outputFile
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

	FILE *fin;		// input file pointer
	FILE *fout;     // output file

	fout = fopen(outputFile, "wt+");
	if (fout == NULL) {
		printf("Create OUTPUT FILE FAIL ...\n");
		return 0;
	}

	fin = fopen(inputFile, "rt");
	if (fin == NULL) {
		printf("OPEN INPUT FILE FAIL ...\n");
		return 0;
	}
	else {

		char buf[BUF_SIZE];
		char *tok = NULL;

		while (fgets(buf, BUF_SIZE, fin) != NULL) {

			tok = strtok(buf, " |\t|\n");
			while (tok != NULL) {

				// <top>
				if (strcmp(tok, "<top>") == 0) {

					fprintf(fout, "<top>\n");
					fgets(buf, BUF_SIZE, fin);
					fprintf(fout, "%s", buf); // query Number

					while (fgets(buf, BUF_SIZE, fin) != NULL) {

						// if </top> TAG, finish TEXT, in buf it contains >>> "\n"
						if (strcmp(buf, "</top>\n") == 0)
							break;

						change_to_low(buf);		// change CAPITAL ALPHABET to LOW
						tok = strtok(buf, tokenizer);

						// if no word, continue..
						if (tok == NULL)
							continue;

						while (tok != NULL) {
							// if stop word pass
							if (is_stopword(tok)) {
								tok = strtok(NULL, tokenizer);
							}
							else {
								fprintf(fout, "%s ", tok);
								tok = strtok(NULL, tokenizer);
							}
						}
						// finish one line
						fprintf(fout, "\n");
					}

					fprintf(fout, "</top>\n\n");

				}

				else {
					tok = strtok(NULL, " |\t|\n");
				}
			}
		}

		fclose(fin);
		fclose(fout);

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

	fclose(f_in);
	fclose(f_out);

	return 1;

}

