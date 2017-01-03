// Load word List from word.txt
// devide into sub word List ...

// needed COUNT_words of relative document
#define REL_COUNT 1

// how many DOC RANK
#define RANK_COUNT 1000
#define HUGE_BUF_SIZE 16*1024*1024


// word_ map structure
map<string, DATA_WORD*>* mapWord = new map<string, DATA_WORD*>; // information of words 'map' version

// map (RB_TREE) version.
map<int, DATA*>* mapEx = new map<int, DATA*>;

// map TOPIC list.
map<int, string>* topicList = new map < int, string >;



BOOL loading_wordLists() {

	BOOL isSuccess = FALSE;

	// =====================================
	// load wordList from word.txt
	// =====================================

	char wordFile[PATH_SIZE];
	strcpy(wordFile, INPUT_FOLDER_PATH);
	strcat(wordFile, "word.txt");

	FILE *wordp;

	wordp = fopen(wordFile, "rt");
	if (wordp == NULL) {
		printf("OPEN word.txt FILE FAIL ...\n");
		return FALSE;
	}

	char buf[BUF_SIZE];
	char* tok;

	while (fgets(buf, BUF_SIZE, wordp) != NULL) {
		char term[WORD_SIZE];
		int term_id;
		int DF;
		int CF;
		int start;

		tok = strtok(buf, "\t");	// index
		term_id = atoi(tok);
		tok = strtok(NULL, "\t");	// word
		strcpy(term, tok);
		tok = strtok(NULL, "\t");	// DF
		DF = atoi(tok);
		tok = strtok(NULL, "\t"); // CF
		CF = atoi(tok);
		tok = strtok(NULL, "\t\n"); // startnum
		start = atoi(tok);

		addWORD(*mapWord, term, term_id, DF, CF, start);

	}


	fclose(wordp);

	isSuccess = TRUE;

	return isSuccess;

}


// destroy all Lists
void destroy_all_Lists() {

	printf("destroy Lists ... ");

	destoryMAP(*mapWord);
	destoryMAP(*mapEx);
	destoryMAP(*topicList);

	printf("success !\n");
}



// version. Language Model
void LM_add_doc_node(char* term, int term_count, int total_count, FILE* indexFp, float* doc_f) {

	// get word Information NODE by 'term'
	map<string, DATA_WORD*>::iterator iter = mapWord->find(term);

	int termCount = iter->second->DF;
	int start = iter->second->start;

	// Language Model
	int CollCount = iter->second->CF;


	// MORE FASTER ...

	// compute and set index FILE POINTER
	long long pos_fp = (long long)(s_line)* (long long)(start);
	_fseeki64(indexFp, pos_fp, SEEK_SET);

	// get ALL needed information at one TIME.
	char* buf = new char[HUGE_BUF_SIZE];	// allocate HUGE size memory
	fread(buf, sizeof(char), termCount * (s_line - 1), indexFp);	// s_line - 1 : except '\0'
	buf[termCount * (s_line - 1)] = '\0';

	char* pos;
	pos = buf;	// position to first index of buf

	for (int i = 0; i < termCount; i++) {

		char word_buf[s_word + 1];
		char doc_buf[s_doc + 1];
		char tf_buf[s_TF + 1];
		//		char weight_buf[s_weight + 1];

		strncpy(word_buf, pos, s_word);
		word_buf[s_word] = '\0';
		pos += s_word;
		strncpy(doc_buf, pos, s_doc);
		doc_buf[s_doc] = '\0';
		pos += s_doc;
		strncpy(tf_buf, pos, s_TF);
		tf_buf[s_TF] = '\0';
		pos += s_TF;
//		strncpy(weight_buf, pos, s_weight);
//		weight_buf[s_weight] = '\0';

		pos += s_weight + 1;	// + '\n' and not '\0'

//		float BM25_score = compute_weight_BM25_first((float)term_count);
		float LM_score = compute_weight_LM_first(atof(tf_buf), (float)CollCount, (float)term_count);

		*doc_f = atof(tf_buf);	// store doc frequency
//		addDATA(*mapEx, atoi(doc_buf), BM25_score);
		addDATA(*mapEx, atoi(doc_buf), LM_score);

	}
	delete[] buf;


}



// Language Model
// FIND relative documents in memory
// rel_DOC_list
BOOL LM_rel_docs_and_ranking(char* topicFile, char* indexFile, char* docFile, char* outputFile) {

	// start get relative documents and RANKING
	printf("Get relative and RANKING ... \n");
	printf("------------------------------------------\n");

	FILE* indexFp;		// index.txt
	FILE* topicFp;		// topics25_stem.txt
	FILE* docFp;		// doc.txt
	FILE* outputFp;		// result.txt

	indexFp = fopen(indexFile, "rt");
	if (indexFp == NULL) {
		printf("OPEN index.txt FILE FAIL ...\n");
		return FALSE;
	}

	topicFp = fopen(topicFile, "rt");
	if (topicFp == NULL) {
		printf("OPEN topic25 FILE FAIL ...\n");
		return FALSE;
	}

	docFp = fopen(docFile, "rt");
	if (docFp == NULL) {
		printf("OPEN doc.txt FILE FAIL ...\n");
		return FALSE;
	}

	outputFp = fopen(outputFile, "wt+");
	if (outputFp == NULL) {
		printf("OPEN output FILE FAIL ...\n");
		return FALSE;
	}

	char buf[BUF_SIZE];
	char* tok;

	while (fgets(buf, BUF_SIZE, topicFp) != NULL) {
		tok = strtok(buf, " \t\n");

		while (tok != NULL) {

			// when <top> .. process of each topic.
			if (strcmp(tok, "<top>") == 0) {
				fgets(buf, BUF_SIZE, topicFp);
				tok = strtok(buf, " \t\n");		// pass <num>
				tok = strtok(NULL, " \t\n");

				int topic_num = atoi(tok);		// get topic number

				LIST* tempWORDS = createList();
				// use TF -> each word count
				int total_count = 0;

				while (fgets(buf, BUF_SIZE, topicFp) != NULL) {

					tok = strtok(buf, " \t\n");

					if (tok == NULL)
						continue;

					if (strcmp(tok, "</top>") == 0)
						break;

					while (tok != NULL) {
						// count words
						total_count++;
						addNode(tempWORDS, tok);
						tok = strtok(NULL, " \t\n");
					}
				}

				float doc_f = 0;

				// process find relative DOC LIST by each term
				tempWORDS->pos = tempWORDS->head;
				while (tempWORDS->pos != NULL) {
					LM_add_doc_node(tempWORDS->pos->word, tempWORDS->pos->TF, total_count, indexFp, &doc_f);
					tempWORDS->pos = tempWORDS->pos->next;
				}


				// more faster read doc.txt file
				fseek(docFp, 0, SEEK_END);
				int docfp_size = ftell(docFp);
				rewind(docFp);

				char* buf_doc = new char[docfp_size + 1];	// allocate HUGE size memory
				fread(buf_doc, sizeof(char), docfp_size, docFp);	// 
				buf_doc[docfp_size] = '\0';

				char* tok_doc;
				tok_doc = strtok(buf_doc, " \t\n");

				// select RELATIVE DOCUMENT and RANKING from all document
				// <weight, document number>
				map<float, string>* mapRANK = new map<float, string>;

				for (map<int, DATA*>::iterator iter = mapEx->begin(); iter != mapEx->end(); iter++) {

					// if relative term APPEAR less than REL_COUNT, PASS..
					if (iter->second->num_words < REL_COUNT) {
						continue;
					}

					// smallest component
					map<float, string>::iterator it_rank = mapRANK->begin();
					if ((mapRANK->size() < RANK_COUNT) || (it_rank->first < iter->second->weight)) {

						string doc_ID = "";
						float weight = iter->second->weight;
						// find document NAME from doc.txt by doc_ID
				
						// find doc id
						while (atoi(tok_doc) != iter->first) {
							// if not correct skip two step
							tok_doc = strtok(NULL, " \t\n");	// SKIP doc_NAME
							tok_doc = strtok(NULL, " \t\n");	// SKIP doc_length
							tok_doc = strtok(NULL, " \t\n");	// next doc id
						}
						// if found doc id, escape while LOOP
						tok_doc = strtok(NULL, " \t\n");	// doc_NAME
						doc_ID += tok_doc;
						doc_ID += '\0';

						tok_doc = strtok(NULL, " \t\n");	// doc_length
						iter->second->doc_lenth = atoi(tok_doc);
						weight = compute_weight_LM_second(weight, iter->second->doc_lenth, total_count);

						tok_doc = strtok(NULL, " \t\n");	// next doc id, if end return NULL


						mapRANK->insert(make_pair(weight, doc_ID));


						// if rank list size > count, delete FIRST element ( smallest one )
						if (mapRANK->size() > RANK_COUNT) {
							it_rank = mapRANK->begin();
							mapRANK->erase(it_rank);
						}
					}
				}

				// print RELATIVE DOCUMENT
				fprintf(outputFp, "%3d\n", topic_num);
				
				
				// print DOC_NAME <tab> Weight
				for (map<float, string>::reverse_iterator r_iter = mapRANK->rbegin(); r_iter != mapRANK->rend(); r_iter++) {
					fprintf(outputFp, "%s\t", r_iter->second.c_str());
				}

				// process of one topic finished .
				
				delete[] buf_doc;
				rewind(docFp);		// reset doc.txt

				resetMAP(*mapEx);	// clear doc map
				destoryMAP(*mapRANK);
				destroyList(tempWORDS);
				fprintf(outputFp, "\n");
				printf("finish topic %d ... \n", topic_num);

				break;
			}

			else {
				tok = strtok(NULL, " |\t|\n");
			}

		}
	}


	printf("------------------------------------------\n");



	fclose(indexFp);
	fclose(topicFp);
	fclose(docFp);
	fclose(outputFp);

	return TRUE;
}
