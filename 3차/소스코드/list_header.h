// Load word List from word.txt
// devide into sub word List ...

// needed COUNT_words of relative document
#define REL_COUNT 1

// how many DOC RANK
#define RANK_COUNT 50
#define HUGE_BUF_SIZE 16*1024*1024


LIST* wordList = createList();		// Information of words

LIST* wordList_a = createList();		// Information of 'a' start words
LIST* wordList_b = createList();		// Information of 'b' start words
LIST* wordList_c = createList();
LIST* wordList_d = createList();
LIST* wordList_e = createList();
LIST* wordList_f = createList();
LIST* wordList_g = createList();
LIST* wordList_h = createList();
LIST* wordList_i = createList();
LIST* wordList_j = createList();
LIST* wordList_k = createList();
LIST* wordList_l = createList();
LIST* wordList_m = createList();
LIST* wordList_n = createList();
LIST* wordList_o = createList();
LIST* wordList_p = createList();
LIST* wordList_q = createList();
LIST* wordList_r = createList();
LIST* wordList_s = createList();
LIST* wordList_t = createList();
LIST* wordList_u = createList();
LIST* wordList_v = createList();
LIST* wordList_w = createList();
LIST* wordList_x = createList();
LIST* wordList_y = createList();
LIST* wordList_z = createList();
LIST* wordList_else = createList();		// not alphbet start


// list of relative documents with TOPIC
//	DOC_LIST* DOC_list = createDocList();
//	DOC_LIST* rel_DOC_list = createDocList();

// AVL_TREE version.
// AVL_TREE* avl_doclist = AVL_Create(compareDOCID);

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

		tok = strtok(buf, "\t");	// index
		tok = strtok(NULL, "\t");	// word
		_insert_back(wordList, tok);
		tok = strtok(NULL, "\t");	// DF
		wordList->rear->DF = atoi(tok);
		tok = strtok(NULL, "\t"); // CF
		wordList->rear->CF = atoi(tok);
		tok = strtok(NULL, "\t\n"); // startnum
		wordList->rear->start = atoi(tok);

	}

	fclose(wordp);


	// ==============================================
	// load SUB wordList from wordList
	// ==============================================


	wordList->pos = wordList->head;
	while (wordList->pos != NULL){
		switch (wordList->pos->word[0])
		{
		case 'a':
			sub_node_insert(wordList_a, wordList->pos);			break;
		case 'b':
			sub_node_insert(wordList_b, wordList->pos);			break;
		case 'c':
			sub_node_insert(wordList_c, wordList->pos);			break;
		case 'd':
			sub_node_insert(wordList_d, wordList->pos);			break;
		case 'e':
			sub_node_insert(wordList_e, wordList->pos);			break;
		case 'f':
			sub_node_insert(wordList_f, wordList->pos);			break;
		case 'g':
			sub_node_insert(wordList_g, wordList->pos);			break;
		case 'h':
			sub_node_insert(wordList_h, wordList->pos);			break;
		case 'i':
			sub_node_insert(wordList_i, wordList->pos);			break;
		case 'j':
			sub_node_insert(wordList_j, wordList->pos);			break;
		case 'k':
			sub_node_insert(wordList_k, wordList->pos);			break;
		case 'l':
			sub_node_insert(wordList_l, wordList->pos);			break;
		case 'm':
			sub_node_insert(wordList_m, wordList->pos);			break;
		case 'n':
			sub_node_insert(wordList_n, wordList->pos);			break;
		case 'o':
			sub_node_insert(wordList_o, wordList->pos);			break;
		case 'p':
			sub_node_insert(wordList_p, wordList->pos);			break;
		case 'q':
			sub_node_insert(wordList_q, wordList->pos);			break;
		case 'r':
			sub_node_insert(wordList_r, wordList->pos);			break;
		case 's':
			sub_node_insert(wordList_s, wordList->pos);			break;
		case 't':
			sub_node_insert(wordList_t, wordList->pos);			break;
		case 'u':
			sub_node_insert(wordList_u, wordList->pos);			break;
		case 'v':
			sub_node_insert(wordList_v, wordList->pos);			break;
		case 'w':
			sub_node_insert(wordList_w, wordList->pos);			break;
		case 'x':
			sub_node_insert(wordList_x, wordList->pos);			break;
		case 'y':
			sub_node_insert(wordList_y, wordList->pos);			break;
		case 'z':
			sub_node_insert(wordList_z, wordList->pos);			break;
		default:
			sub_node_insert(wordList_else, wordList->pos);			break;
		}

		wordList->pos = wordList->pos->next;
	}


	isSuccess = TRUE;

	return isSuccess;
	
}

// destroy all Lists
void destroy_all_Lists() {

	printf("destroy Lists ... ");

	destroyList(wordList);

	destroyList(wordList_a);
	destroyList(wordList_b);
	destroyList(wordList_c);
	destroyList(wordList_d);
	destroyList(wordList_e);
	destroyList(wordList_f);
	destroyList(wordList_g);
	destroyList(wordList_h);
	destroyList(wordList_i);
	destroyList(wordList_j);
	destroyList(wordList_k);
	destroyList(wordList_l);
	destroyList(wordList_m);
	destroyList(wordList_n);
	destroyList(wordList_o);
	destroyList(wordList_p);
	destroyList(wordList_q);
	destroyList(wordList_r);
	destroyList(wordList_s);
	destroyList(wordList_t);
	destroyList(wordList_u);
	destroyList(wordList_v);
	destroyList(wordList_w);
	destroyList(wordList_x);
	destroyList(wordList_y);
	destroyList(wordList_z);
	destroyList(wordList_else);

//	destroyList(DOC_list);
//	destroyList(rel_DOC_list);

//	AVL_Destroy(avl_doclist);
	destoryMAP(*mapEx);
	destoryMAP(*topicList);

	printf("success !\n");
}


// find NODE by term
// in each type of word Lists.
NODE* find_term_node(char* term) {
	NODE* temp_node;

	switch (term[0])
	{
		// Find word pointer
	case 'a':
		temp_node = find_word_inList(wordList_a, term);			break;
	case 'b':
		temp_node = find_word_inList(wordList_b, term);			break;
	case 'c':
		temp_node = find_word_inList(wordList_c, term);			break;
	case 'd':
		temp_node = find_word_inList(wordList_d, term);			break;
	case 'e':
		temp_node = find_word_inList(wordList_e, term);			break;
	case 'f':
		temp_node = find_word_inList(wordList_f, term);			break;
	case 'g':
		temp_node = find_word_inList(wordList_g, term);			break;
	case 'h':
		temp_node = find_word_inList(wordList_h, term);			break;
	case 'i':
		temp_node = find_word_inList(wordList_i, term);			break;
	case 'j':
		temp_node = find_word_inList(wordList_j, term);			break;
	case 'k':
		temp_node = find_word_inList(wordList_k, term);			break;
	case 'l':
		temp_node = find_word_inList(wordList_l, term);			break;
	case 'm':
		temp_node = find_word_inList(wordList_m, term);			break;
	case 'n':
		temp_node = find_word_inList(wordList_n, term);			break;
	case 'o':
		temp_node = find_word_inList(wordList_o, term);			break;
	case 'p':
		temp_node = find_word_inList(wordList_p, term);			break;
	case 'q':
		temp_node = find_word_inList(wordList_q, term);			break;
	case 'r':
		temp_node = find_word_inList(wordList_r, term);			break;
	case 's':
		temp_node = find_word_inList(wordList_s, term);			break;
	case 't':
		temp_node = find_word_inList(wordList_t, term);			break;
	case 'u':
		temp_node = find_word_inList(wordList_u, term);			break;
	case 'v':
		temp_node = find_word_inList(wordList_v, term);			break;
	case 'w':
		temp_node = find_word_inList(wordList_w, term);			break;
	case 'x':
		temp_node = find_word_inList(wordList_x, term);			break;
	case 'y':
		temp_node = find_word_inList(wordList_y, term);			break;
	case 'z':
		temp_node = find_word_inList(wordList_z, term);			break;
	default:
		temp_node = find_word_inList(wordList_else, term);			break;
	}
	
	return temp_node;
}


void add_doc_node(char* term, int term_count, int total_count, FILE* indexFp) {

	// get word Information NODE by 'term'
	NODE* wordNode;
	wordNode = find_term_node(term);
	
	int termid = wordNode->ID;
	int termCount = wordNode->DF;

	// TOO SLOW ...
/*
	// compute and set index FILE POINTER
	long long pos_fp = (long long)s_line * (long long)(wordNode->start);
	_fseeki64(indexFp, pos_fp, SEEK_SET);
	printf("8\t");
	for (int i = 0; i < termCount; i++) {
		char word_buf[s_word + 1] = { 0 };
		char doc_buf[s_doc + 1] = { 0 };
		char tf_buf[s_TF + 1] = { 0 };
		char weight_buf[s_weight + 1] = { 0 };
		fread(word_buf, sizeof(char), s_word, indexFp);
		fread(doc_buf, sizeof(char), s_doc, indexFp);
		fread(tf_buf, sizeof(char), s_TF, indexFp);
		fread(weight_buf, sizeof(char), s_weight, indexFp);
		_fseeki64(indexFp, 2, SEEK_CUR); // PASS "\n" and null

		//		printf("%s %s %s %s\n", word_buf, doc_buf, tf_buf, weight_buf);
//		addNode(DOC_list, atoi(doc_buf), atof(weight_buf));

	}
*/

	// MORE FASTER ...

	// compute and set index FILE POINTER
	long long pos_fp = (long long)(s_line) * (long long)(wordNode->start);
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
		char weight_buf[s_weight + 1];

		strncpy(word_buf, pos, s_word);
		word_buf[s_word] = '\0';
		pos += s_word;
		strncpy(doc_buf, pos, s_doc);
		doc_buf[s_doc] = '\0';
		pos += s_doc;
		strncpy(tf_buf, pos, s_TF);
		tf_buf[s_TF] = '\0';
		pos += s_TF;
		strncpy(weight_buf, pos, s_weight);
		weight_buf[s_weight] = '\0';
		
		pos += s_weight + 1;	// + '\n' and not '\0'

//		addNode(DOC_list, atoi(doc_buf), atof(weight_buf));
		// list structure is too slow..
		// USE --> AVL_STRUCTURE
//		printf("%s%s%s%s\n", word_buf, doc_buf, tf_buf, weight_buf);
//		addDATA(avl_doclist, atoi(doc_buf), atof(weight_buf));

		// weight = each weight * number of word / total query LENGTH
		float query_f = (float)term_count / (float)total_count;	// query frequency
		float VSM_score = compute_score_VSM(atof(weight_buf), query_f);

		addDATA(*mapEx, atoi(doc_buf), VSM_score);

	}
	delete[] buf;
	

}



// FIND relative documents in memory
// rel_DOC_list
BOOL rel_docs_and_ranking(char* topicFile, char* indexFile, char* docFile, char* outputFile) {

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

				// process find relative DOC LIST by each term
				tempWORDS->pos = tempWORDS->head;
				while (tempWORDS->pos != NULL) {
					add_doc_node(tempWORDS->pos->word, tempWORDS->pos->TF, total_count, indexFp);
					tempWORDS->pos = tempWORDS->pos->next;
				}

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
						while (fgets(buf, BUF_SIZE, docFp) != NULL) {
							tok = strtok(buf, " \t\n");
							if (iter->first == atoi(tok)) {
								tok = strtok(NULL, " \t\n");	// doc_NAME
								doc_ID += tok;
								doc_ID += '\0';
								tok = strtok(NULL, " \t\n");	// doc_length
								iter->second->doc_lenth = atoi(tok);

								break;
							}
						}
						mapRANK->insert(make_pair(weight, doc_ID));
		//				for (it_rank = mapRANK->begin(); it_rank != mapRANK->end(); it_rank++) {
		//					printf("%f\t", it_rank->first);
		//				}
						
						
						// if rank list size > count, delete FIRST element ( smallest one )
						if (mapRANK->size() > RANK_COUNT) {
							it_rank = mapRANK->begin();
							mapRANK->erase(it_rank);
						}
					}
				}

				// print RELATIVE DOCUMENT
				fprintf(outputFp, "topicnum : %3d\n", topic_num);
				fprintf(outputFp, "query : ");

				map<int, string>::iterator title_it = topicList->find(topic_num);
				
				// print TITLE
				if (title_it != topicList->end()) {
					fprintf(outputFp, "%s\n", title_it->second.c_str());
				}

				// print DOC_NAME <tab> Weight
				for (map<float, string>::reverse_iterator r_iter = mapRANK->rbegin(); r_iter != mapRANK->rend(); r_iter++) {
					fprintf(outputFp, "%s\t%f\n", r_iter->second.c_str(), r_iter->first);
				}

				// process of one topic finished .
//				cleanList(DOC_list);		// clean doc list
//				cleanList(rel_DOC_list);	// clean rel doc list

				rewind(docFp);		// reset doc.txt

				clearMAP(*mapEx);	// clear doc map
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
	

//	printAVL(avl_doclist);

//	printf("list count : %d\n", avl_doclist->count);
//	printf("relative list count : %d\n", rel_DOC_list->count);

	
	fclose(indexFp);
	fclose(topicFp);
	fclose(docFp);
	fclose(outputFp);

	return TRUE;
}


