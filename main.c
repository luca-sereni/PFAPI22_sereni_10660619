#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BLACK 0
#define RED 1

struct node{
    char* word;
    struct node *left, *right, *parent;
    short int is_valid;
    short int color;
};

struct filter{
    short int belonged_to_reference_word;
    short int* in_position;
    short int* not_in_position;
    int minimum_number;
    int exact_number;
};

struct node* t_nil;
struct node leaf2;

void setup_words(struct node** tree, int words_size, int* number_of_words);
//void new_game(char*** eligible_words, char ***filter_words, int words_size, int* number_of_words);
void new_game(struct node** leaf, int words_size, int* number_of_words);
void quicksort(char** eligible_words, int start, int end);
void heapsort(char** eligible_words, int number_of_words);
void build_max_heap(char** eligible_words, int number_of_words);
void max_heapify(char** eligible_words, int i, int number_of_words);
int partition(char** eligible_words, int start, int end);
void swap(char** a, char** b);
//int findWord(char** eligible_words, char* word_to_find, int start, int end);
int findWord(struct node** root, char* word_to_find);
void check_words(char* word, char* reference_word, struct filter* filters);
int check_incorrect_positions(char* word, char* reference_word, int index, struct filter* filters);
//void addWords(char*** eligible_words, char*** filter_words, int* number_of_words, int words_size);
int addWords(struct node** leaf, int* number_of_words, int words_size);
//void insert_node(char* word, struct node** leaf, int* occurrences);
void insert_node(char* word, struct node** leaf);
void inorder_tree_walk(struct node* tree);
int print_number_filtered_words(struct node** tree, struct filter* filters, char* reference_word);
void free_tree(struct node* node);
int check_for_presence(char* reference_word, char character);
char number_of_occurrences(char* word, char* reference_word, int index);
int hash(char character);
void setup_filters(struct filter* filters, int words_size);
void reset_words(struct node** tree);
int find_occurrences(char *word, char c);

void left_rotate(struct node** tree, struct node* element);
void right_rotate(struct node** tree, struct node* element);
void rb_insert(struct node** tree, struct node* element);
void rb_insert_fixup(struct node** tree, struct node* element);
struct node* get_new_node(char* word);

int main(){
    int words_size;
    int index = 0;
    char char_words_size[10];
    int next_instruction;
    int c, number_of_words = 0;
    struct node* root = NULL;
    //char *word = NULL;
    //char** eligible_words = NULL;
    //char** filter_words = NULL;

    while((c = getchar()) != '\n' && c != EOF){
        char_words_size[index] = c;
        index++;
    }
    char_words_size[index] = '\0';
    if(sscanf(char_words_size, "%d", &words_size)<0);
    t_nil = &leaf2;
    t_nil->color = BLACK;
    root = t_nil;
    //printf("%d\n", words_size);
    //words_size = atoi(char_words_size);
    //word = (char*)malloc((words_size+1) * sizeof(char));
    //while((c=getchar()) != '\n' && c != EOF);
    /*do{
        index = 0;
        c = getchar();
        if(c == '+'){
            while((c = getchar()) != '\n' && c != EOF){
                instruction[index] = c;
                index++;
            }
            instruction[index] = '\0';
            if(strcmp(instruction, NEW_GAME) == 0){
                //New game
                printf("New game\n");
                new_game(&eligible_words, &filter_words, words_size, &number_of_words);
            } else if(strcmp(instruction, START_ADD_WORDS) == 0){
                addWords(&eligible_words, &filter_words, &number_of_words, words_size);
                //quicksort(eligible_words, 0, number_of_words-1);
                heapsort(eligible_words, number_of_words);
            } else if(strcmp(instruction, FILTRATE_PRINT) == 0){
                //stampa_filtrate
                printf("Filtrate print\n");
            }
        } else if(c != '\n' && c != EOF){
            //add words
            //while ((c = getchar()) != '\n');
            word[index] = c;
            index++;
            while((c = getchar()) != '\n' && c != EOF){
                word[index] = c;
                index++;
            }
            word[index] = '\0';
            printf("%s\n", word);
            eligible_words = realloc(eligible_words, (number_of_words + 1) * sizeof(char*));
            filter_words = realloc(filter_words, (number_of_words + 1) * sizeof(char*));
            eligible_words[number_of_words] = (char*)malloc((words_size + 1) * sizeof(char));
            filter_words[number_of_words] = (char*)malloc((words_size + 1) * sizeof(char));
            strcpy(eligible_words[number_of_words], word);
            strcpy(filter_words[number_of_words], word);
            number_of_words++;
        } else{
            free(word);
            for(i = 0; i < number_of_words; i++){
                free(eligible_words[i]);
                free(filter_words[i]);
            }
            free(filter_words);
            free(eligible_words);
            end = 1;
            printf("Need to finish");
        }
    } while (end == 0);*/
    setup_words(&root, words_size, &number_of_words);
    do{
        index = 0;
        next_instruction = getchar();
        while((c = getchar()) != '\n' && c!= EOF);
        /*while((c = getchar()) != '\n' && c!= EOF){
            next_instruction[index] = c;
            index++;
        }*/
        //next_instruction[index] = '\0';
        if(next_instruction == 'n'){
            new_game(&root, words_size, &number_of_words);
            reset_words(&root);
            c = getchar();
        } else if(next_instruction == 'i'){
            addWords(&root, &number_of_words, words_size);
            c = getchar();
        }
    } while(c != '\n' && c!= EOF);
    free_tree(root);
    return 0;
}

    void setup_words(struct node** tree, int words_size, int* number_of_words){
    char word[words_size + 1];
    //struct node* tree = NULL;
    //char** eligible_words = NULL;
    //char** filter_words = NULL;
    int c, index;
    while ((c = getchar()) != '+'){
        index = 0;
        //occurrences = calloc(64, sizeof(int));
        while (c != '\n' && c != EOF){
            word[index] = c;
            //i = hash(c);
            //occurrences[i]++;
            index++;
            c = getchar();
        }
        word[index] = '\0';
        //printf("%s\n", word);
        //insert_node(word, tree, occurrences);
        //insert_node(word, tree);
        rb_insert(tree, get_new_node(word));
        //inorder_tree_walk(tree);
        (*number_of_words)++;

        //eligible_words = realloc(eligible_words, (number_of_words + 1) * sizeof(char*));
        //filter_words = realloc(filter_words, (number_of_words + 1) * sizeof(char*));
        //eligible_words[number_of_words] = (char*)malloc((words_size + 1) * sizeof(char));
        //filter_words[number_of_words] = (char*)malloc((words_size + 1) * sizeof(char));
        //strcpy(eligible_words[number_of_words], word);
        //strcpy(filter_words[number_of_words], word);
    }
}


/*void new_game(char ***eligible_words, char*** filter_words, int words_size, int* number_of_words){
    char reference_word[words_size+1];
    char word_to_analyze[words_size+1];
    int c;
    char char_number_of_controls[10], next_instruction[17];
    int index = 0, number_of_controls, end = 0;
    while((c = getchar()) != '\n' && c != EOF){
        reference_word[index] = c;
        index++;
    }
    reference_word[index] = '\0';
    index = 0;
    while((c = getchar()) != '\n' && c != EOF){
        char_number_of_controls[index] = c;
        index++;
    }
    char_number_of_controls[index] = '\0';
    if(sscanf(char_number_of_controls, "%d", &number_of_controls)>0);
    //number_of_controls = atoi(char_number_of_controls);
    //quicksort(*eligible_words, 0, (*number_of_words) - 1);
    printf("%d\n", *number_of_words);
    heapsort(*eligible_words, *number_of_words);
    do{
        index = 0;
        if((c = getchar()) == '+'){
            //instructions
            while((c = getchar()) != '\n' && c != EOF){
                next_instruction[index] = c;
                index++;
            }
            next_instruction[index] = '\0';
            if(strcmp(next_instruction, START_ADD_WORDS) == 0){
                addWords(eligible_words, filter_words, number_of_words, words_size);
                //quicksort(*eligible_words, 0, (*number_of_words) - 1);
                heapsort(*eligible_words, *number_of_words);
            } else if(strcmp(next_instruction, FILTRATE_PRINT) == 0){
                //stampa filtrate
                printf("Stampa filtrate\n");
            }
        }else{
            word_to_analyze[index] = c;
            index++;
            while((c = getchar()) != '\n' && c != EOF){
                word_to_analyze[index] = c;
                index++;
            }
            word_to_analyze[index] = '\0';
            if(findWord(*eligible_words, word_to_analyze, 0, (*number_of_words)-1) == 1){
                if(strcmp(reference_word, word_to_analyze) == 0){
                    printf("ok\n");
                    end = 1;
                }else{
                    //Analyze
                    printf("analizzo %s\n", word_to_analyze);
                    printf("%d\n", *number_of_words);
                    number_of_controls--;
                }
            }else{
                printf("not_exists\n");
            }
        }
    } while (end == 0 && number_of_controls > 0);
    index = 0;
    c = getchar();
    if(c == '+'){
        while((c = getchar()) != '\n' && c != EOF){
            next_instruction[index] = c;
            index++;
        }
        next_instruction[index] = '\0';
        if(strcmp(next_instruction, START_ADD_WORDS) == 0){
            addWords(eligible_words, filter_words, number_of_words, words_size);
            //quicksort(*eligible_words, 0, (*number_of_words) - 1);
            heapsort(*eligible_words, *number_of_words);
        } else if(strcmp(next_instruction, NEW_GAME) == 0){
            new_game(eligible_words, filter_words, words_size, number_of_words);
        }
    }
}*/

/*void quicksort(char** eligible_words, int start, int end){
    int q;
    if(start < end){
        q = partition(eligible_words, start, end);
        quicksort(eligible_words, start, q-1);
        quicksort(eligible_words, q+1, end);
    }
}*/

/*int partition(char** eligible_words, int start, int end){
    char* pivot = eligible_words[end];
    int i = start - 1;
    int j;
    for(j = start; j <= end - 1; j++){
        if(strcmp(eligible_words[j], pivot) < 0){
            i++;
            swap(&eligible_words[i], &eligible_words[j]);
        }
    }
    swap(&eligible_words[i+1], &eligible_words[end]);
    return i+1;
}*/

void swap(char** a, char** b){
    char *t = *a;
    *a = *b;
    *b = t;
}

/*int findWord(char **eligible_words, char* word_to_find, int start, int end){
    if(end >= start){
        int middle = start + (end-start)/2;
        if(strcmp(eligible_words[middle], word_to_find) == 0)
            return 1;
        if(strcmp(eligible_words[middle], word_to_find) > 0)
            return findWord(eligible_words, word_to_find, start, middle-1);
        return findWord(eligible_words, word_to_find, middle+1, end);
    }
    return 0;
}*/

/*void addWords(char ***eligible_words, char*** filter_words, int* number_of_words, int words_size){
    int index, c, end = 0;
    char* word_to_add = (char*)malloc((words_size+1) * sizeof(char));
    do{
        index = 0;
        c = getchar();
        if(c == '+'){
            end = 1;
            while ((c = getchar()) != '\n' && c != EOF);
        }else{
            word_to_add[index] = c;
            index++;
            while((c = getchar()) != '\n' && c != EOF){
                word_to_add[index] = c;
                index++;
            }
            word_to_add[index] = '\0';
            printf("%s\n", word_to_add);
            *eligible_words = (char**)realloc(*eligible_words, ((*number_of_words) + 1) * sizeof(char *));
            // *filter_words = (char**)realloc(*filter_words, ((*number_of_words)+1)*sizeof(char *));
            (*eligible_words)[(*number_of_words)] = (char*)malloc((words_size + 1) * sizeof(char));
            //(*filter_words)[(*number_of_words)] = (char*)malloc((words_size + 1) * sizeof(char));
            strcpy((*eligible_words)[*number_of_words], word_to_add);
            //strcpy((*filter_words)[*number_of_words], word_to_add);
            (*number_of_words)++;
        }
    } while(end == 0);
    free(word_to_add);
}*/

void max_heapify(char** eligible_words, int i, int number_of_words){
    int left = (2*i) + 1;
    int right = (2*i) + 2;
    int max;
    if(left < number_of_words && strcmp(eligible_words[left], eligible_words[i]) > 0) {
        max = left;
    }else{
        max = i;
    }
    if(right < number_of_words && strcmp(eligible_words[right], eligible_words[max]) > 0){
        max = right;
    }
    if(max != i){
        swap(&eligible_words[i], &eligible_words[max]);
        max_heapify(eligible_words, max, number_of_words);
    }
}

void build_max_heap(char** eligible_words, int number_of_words){
    int i;
    for(i = number_of_words/2 - 1; i >= 0; i--){
        max_heapify(eligible_words, i, number_of_words);
    }
}

void heapsort(char** eligible_words, int number_of_words){
    int i;
    int heap_size = number_of_words;
    build_max_heap(eligible_words, number_of_words);
    for(i = number_of_words - 1; i >= 0; i--){
        swap(&eligible_words[0], &eligible_words[i]);
        heap_size--;
        max_heapify(eligible_words, 0, heap_size);
    }
}

void insert_node(char* word, struct node** leaf){
    int compare;
    if(*leaf == NULL){
        *leaf = (struct node*) malloc(sizeof(struct node));
        (*leaf)->word = malloc((strlen(word) + 1)* sizeof(char));
        strcpy((*leaf)->word, word);
        (*leaf)->right = NULL;
        (*leaf)->left = NULL;
        //(*leaf)->occurrences = occurrences;
        (*leaf)->is_valid = 1;
    }else{
        compare = strcmp(word, (*leaf)->word);
        if(compare < 0){
            //insert_node(word, &(*leaf)->left, occurrences);
            insert_node(word, &(*leaf)->left);
        } else if(compare > 0){
            //insert_node(word, &(*leaf)->right, occurrences);
            insert_node(word, &(*leaf)->right);
        }
    }
    //printf("word: %s, occurrences first: %c = %d\n", (*leaf)->word, (*leaf)->word[0], (*leaf)->occurrences[hash((*leaf)->word[0])]);
}

void reset_words(struct node** tree){
    if((*tree) != t_nil){
        reset_words(&(*tree)->left);
        (*tree)->is_valid = 1;
        reset_words(&(*tree)->right);
    }
}

void inorder_tree_walk(struct node* tree) {
    if (tree != t_nil) {
        inorder_tree_walk(tree->left);
        if (tree->is_valid == 1)
            printf("%s\n", tree->word);
        inorder_tree_walk(tree->right);
    }
}

int print_number_filtered_words(struct node** tree, struct filter* filters, char* reference_word){
    int i,count=0, index, index_reference_word, num_of_occurrences, num_of_occurrences_ref_word;
    unsigned int len;
    int end = 0;
    if((*tree) != t_nil){
        count = count + print_number_filtered_words(&(*tree)->left, filters, reference_word);
        if((*tree)->is_valid == 1 && strcmp((*tree)->word, reference_word) != 0) {
            //printf("word: %s\n", (*tree)->word);
            //printf("Ref.word: %s\n", reference_word);
            len = strlen((*tree)->word);
            i = 0;
            while (i < len && end == 0){
                index = hash((*tree)->word[i]);
                index_reference_word = hash(reference_word[i]);
                num_of_occurrences = find_occurrences((*tree)->word, (*tree)->word[i]);
                num_of_occurrences_ref_word = find_occurrences((*tree)->word, reference_word[i]);
                if(filters[index].belonged_to_reference_word == 0 || filters[index].not_in_position[i] == 1 || filters[index].exact_number > num_of_occurrences || filters[index].minimum_number > num_of_occurrences || filters[index_reference_word].minimum_number > num_of_occurrences_ref_word || filters[index_reference_word].exact_number > num_of_occurrences_ref_word || (filters[index_reference_word].in_position[i] == 1 && reference_word[i] != (*tree)->word[i])){
                    (*tree)->is_valid = 0;
                    count++;
                    end = 1;
                }
                i++;
            }
            /*for (i = 0; i < len; i++) {
                index = hash((*tree)->word[i]);
                index_reference_word = hash(reference_word[i]);
                num_of_occurrences = find_occurrences((*tree)->word, (*tree)->word[i]);
                num_of_occurrences_ref_word = find_occurrences((*tree)->word, reference_word[i]);
                if (filters[index].belonged_to_reference_word == 0) {
                    //printf("%c\n", (*tree)->word[i]);
                    //printf("dentro primo if\n");
                    (*tree)->is_valid = 0;
                    count++;
                    break;
                } else {
                    if (filters[index].not_in_position[i] == 1) {
                        //printf("dentro 1 if/else\n");
                        (*tree)->is_valid = 0;
                        count++;
                        break;
                    } else if (filters[index].exact_number > num_of_occurrences) {
                        //(*tree)->occurrences[index]
                        //printf("dentro 2 if/else\n");
                        //printf("%d  --  %d\n", filters[index].exact_number, (*tree)->occurrences[index]);
                        (*tree)->is_valid = 0;
                        count++;
                        break;
                    } else if (filters[index].minimum_number > num_of_occurrences) {
                        //printf("%c\n", (*tree)->word[i]);
                        //printf("%d -- %d\n", filters[index].minimum_number, (*tree)->occurrences[index]);
                        //printf("dentro 3 if/else\n");
                        (*tree)->is_valid = 0;
                        count++;
                        break;
                    } else if(filters[index_reference_word].belonged_to_reference_word == 1 && filters[index_reference_word].minimum_number > num_of_occurrences_ref_word){
                        //printf("dentro 4 if/else\n");
                        (*tree)->is_valid = 0;
                        count++;
                        break;
                    } else if(filters[index_reference_word].belonged_to_reference_word == 1 && filters[index_reference_word].exact_number > num_of_occurrences_ref_word){
                        //printf("dentro 5 if/else\n");
                        (*tree)->is_valid = 0;
                        count++;
                        break;
                    } else if(filters[index_reference_word].belonged_to_reference_word == 1 && filters[index_reference_word].in_position[i] == 1 && reference_word[i] != (*tree)->word[i]){
                        //printf("dentro 6 if/else\n");
                        (*tree)->is_valid = 0;
                        count++;
                        break;
                    }
                }
            }*/
        }
        count = count + print_number_filtered_words(&(*tree)->right, filters, reference_word);
    }
    return count;
}

void new_game(struct node** leaf, int words_size, int* number_of_words){
    char reference_word[words_size+1];
    char word_to_analyze[words_size+1];
    struct filter filters[64];
    int c,i,n, number_filtered_words = *number_of_words;
    char char_number_of_controls[10];
    int next_instruction;
    int index = 0, number_of_controls, end = 0;
    setup_filters(filters, words_size);
    //while((c=getchar()) != '\n' && c != EOF);
    while((c = getchar()) != '\n' && c != EOF){
        reference_word[index] = c;
        //i = hash(c);
        //filters[i].belonged_to_reference_word = 1;
        //filters[i].in_position[index] = 1;
        //filters[i].exact_number++;
        index++;
    }
    reference_word[index] = '\0';
    //while((c=getchar()) != '\n' && c != EOF);
    index = 0;
    while((c = getchar()) != '\n' && c != EOF){
        char_number_of_controls[index] = c;
        index++;
    }
    char_number_of_controls[index] = '\0';
    if(sscanf(char_number_of_controls, "%d", &number_of_controls)>0);
    //number_of_controls = atoi(char_number_of_controls);
    //quicksort(*eligible_words, 0, (*number_of_words) - 1);
    //while((c=getchar()) != '\n' && c != EOF);
    do{
        //while((c=getchar()) != '\n' && c != EOF);
        index = 0;
        if((c = getchar()) == '+'){
            //instructions
            next_instruction = getchar();
            while((c = getchar()) != '\n' && c != EOF);
            /*while((c = getchar()) != '\n' && c != EOF){
                next_instruction[index] = c;
                index++;
            }*/
            //next_instruction[index] = '\0';
            if(next_instruction == 'i'){
                //addWords(eligible_words, filter_words, number_of_words, words_size);
                //quicksort(*eligible_words, 0, (*number_of_words) - 1);
                //heapsort(*eligible_words, *number_of_words);
                n = addWords(leaf, number_of_words, words_size);
                number_filtered_words = number_filtered_words + n - print_number_filtered_words(leaf, filters, reference_word);
            } else {
                inorder_tree_walk(*leaf);
            }
        }else{
            word_to_analyze[index] = c;
            index++;
            while((c = getchar()) != '\n' && c != EOF){
                word_to_analyze[index] = c;
                index++;
            }
            word_to_analyze[index] = '\0';
            //printf("%s\n", word_to_analyze);
            if(findWord(leaf, word_to_analyze) != 0){
                if(strcmp(reference_word, word_to_analyze) == 0){
                    printf("ok\n");
                    end = 1;
                }else{
                    check_words(word_to_analyze, reference_word, filters);
                    number_of_controls--;
                    number_filtered_words = number_filtered_words - print_number_filtered_words(leaf, filters, reference_word);
                    printf("%d\n", number_filtered_words);
                    if(number_of_controls == 0)
                        printf("ko\n");
                }
            }else{
                printf("not_exists\n");
            }
        }
    } while (end == 0 && number_of_controls > 0);
    for(i = 0; i < 64; i++){
        free(filters[i].in_position);
        free(filters[i].not_in_position);
    }
    /*index = 0;
    c = getchar();
    if(c == '+'){
        while((c = getchar()) != '\n' && c != EOF){
            next_instruction[index] = c;
            index++;
        }
        next_instruction[index] = '\0';
        if(strcmp(next_instruction, START_ADD_WORDS) == 0){
            n = addWords(leaf, number_of_words, words_size);
        } else if(strcmp(next_instruction, NEW_GAME) == 0){
            reset_words(leaf);
            new_game(leaf, words_size, number_of_words);
        }
    }*/
}

int findWord(struct node** root, char* word_to_find){
    if(*root == t_nil)
        return 0;
    if(strcmp((*root)->word, word_to_find) == 0)
        return 1;
    else if(strcmp((*root)->word, word_to_find) < 0)
        return findWord(&(*root)->right, word_to_find);
    else
        return findWord(&(*root)->left, word_to_find);
}

int addWords(struct node** leaf, int* number_of_words, int words_size){
    int index, c, end = 0, number_added_words = 0;
    char word_to_add[words_size +1];
    //int* occurrences;
    do{
        index = 0;
        c = getchar();
        if(c == '+'){
            end = 1;
            while ((c=getchar()) != '\n' && c!= EOF);
        }else{
            //occurrences = calloc(64, sizeof(int));
            word_to_add[index] = c;
            //i = hash(c);
            //occurrences[i]++;
            index++;
            while((c = getchar()) != '\n' && c != EOF){
                word_to_add[index] = c;
                //i = hash(c);
                //occurrences[i]++;
                index++;

            }
            word_to_add[index] = '\0';
            //printf("%s\n", word_to_add);
            //*eligible_words = (char**)realloc(*eligible_words, ((*number_of_words) + 1) * sizeof(char *));
            //*filter_words = (char**)realloc(*filter_words, ((*number_of_words)+1)*sizeof(char *));
            //(*eligible_words)[(*number_of_words)] = (char*)malloc((words_size + 1) * sizeof(char));
            //(*filter_words)[(*number_of_words)] = (char*)malloc((words_size + 1) * sizeof(char));
            //strcpy((*eligible_words)[*number_of_words], word_to_add);
            //strcpy((*filter_words)[*number_of_words], word_to_add);
            //insert_node(word_to_add, leaf, occurrences);
            //insert_node(word_to_add, leaf);
            rb_insert(leaf, get_new_node(word_to_add));
            number_added_words++;
            (*number_of_words)++;
        }
    } while(end == 0);
    return number_added_words;
}

void free_tree(struct node* node){
    if(node == t_nil)
        return;
    free_tree(node->left);
    free_tree(node->right);
    //free_tree(node->parent);
    //free(node->occurrences);
    free(node->word);
    free(node);
}

void check_words(char* word, char* reference_word, struct filter* filters){
    int i;
    int index;
    unsigned int len = strlen(reference_word);
    char c;
    for(i = 0; i < len; i++){
        index = hash(word[i]);
        if(word[i] == reference_word[i] && filters[index].in_position[i] != 1){
            printf("+");
            //filters[index].belonged_to_reference_word = 1;
            if(filters[index].in_position[i] != 2)
                filters[index].minimum_number++;
            filters[index].in_position[i] = 1;
            filters[index].exact_number++;
        } else if(word[i] == reference_word[i] && filters[index].in_position[i] == 1){
            printf("+");
            //filters[index].belonged_to_reference_word = 1;
        }
        else{
            if(check_for_presence(reference_word, word[i]) == 0){
                printf("/");
                filters[index].belonged_to_reference_word = 0;
            }
            else{
                c = number_of_occurrences(word, reference_word, i);
                printf("%c", c);
                if(c == '|'){
                    //filters[index].belonged_to_reference_word = 1;
                    filters[index].not_in_position[i] = 1;
                    if(check_incorrect_positions(word, reference_word, i, filters) == 1)
                        filters[index].minimum_number++;
                } else{
                    filters[index].not_in_position[i] = 1;
                }
            }
        }
    }
    printf("\n");
}

int check_incorrect_positions(char* word, char* reference_word, int index, struct filter* filters){
    int index_character = hash(word[index]), i;
    unsigned int end = strlen(word);
    for(i = 0; i < end; i++){
        if(reference_word[i] == word[index] && filters[index_character].in_position[i] == 0 && reference_word[i] != word[i]){
            filters[index_character].in_position[i] = 2;
            return 1;
        }
    }
    return 0;
}

int check_for_presence(char* reference_word, char character){
    int i;
    unsigned int len = strlen(reference_word);
    for(i = 0; i < len; i++){
        if(reference_word[i] == character)
            return 1;
    }
    return 0;
}

char number_of_occurrences(char* word, char* reference_word, int index) {
    int i, n = 0, c = 0, s = 0;
    unsigned int len = strlen(reference_word);
    for (i = 0; i < len; i++) {
        if (reference_word[i] == word[index]) {
            n++;
            if (reference_word[i] == word[i])
                c++;
        }
    }
    for (i = 0; i < index; i++) {
        if (word[i] != reference_word[i] && word[i] == word[index])
            s++;
    }
    if (s >= (n - c))
        return '/';
    else
        return '|';
}
//64 symbols
int hash(char character){
    if(character == '-')
        return 0;
    else if(character == '_')
        return 1;
    else if(character >= '0' && character <= '9')
        return character - 46;
    else if(character >= 'A' && character <= 'Z')
        return character - 53;
    else
        return character - 59;
}

void setup_filters(struct filter* filters, int words_size){
    int i;
    for(i = 0; i < 64; i++){
        filters[i].belonged_to_reference_word = 1;
        filters[i].in_position = calloc(words_size, sizeof(short int));
        filters[i].not_in_position = calloc(words_size, sizeof(short int));
        filters[i].exact_number = 0;
        filters[i].minimum_number = 0;
    }
}

int find_occurrences(char* word, char c){
    int i, n = 0;
    unsigned int end = strlen(word);
    for(i = 0; i < end; i++){
        if(word[i] == c)
            n++;
    }
    return n;
}

void left_rotate(struct node** tree, struct node* element){
    struct node* temp = element->right;
    element->right = temp->left;

    if(temp->left != t_nil)
        temp->left->parent = element;
    temp->parent = element->parent;
    if(element->parent == t_nil)
        *tree = temp;
    else if(element == element->parent->left)
        element->parent->left = temp;
    else
        element->parent->right = temp;
    temp->left = element;
    element->parent = temp;
}

void right_rotate(struct node** tree, struct node* element){
    struct node* temp = element->left;
    element->left = temp->right;

    if(temp->right != t_nil)
        temp->right->parent = element;
    temp->parent = element->parent;
    if(element->parent == t_nil)
        *tree = temp;
    else if(element == element->parent->right)
        element->parent->right = temp;
    else
        element->parent->left = temp;
    temp->right = element;
    element->parent = temp;
}

void rb_insert_fixup(struct node** tree, struct node* element){
    struct node *x, *y;
    if(element == (*tree))
        (*tree)->color = BLACK;
    else{
        x = element->parent;
        if(x->color == RED){
            if (x == x->parent->left) {
                y = x->parent->right;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    rb_insert_fixup(tree, x->parent);
                } else {
                    if (element == x->right) {
                        element = x;
                        left_rotate(tree, element);
                        x = element->parent;
                    }
                    x->color = BLACK;
                    x->parent->color = RED;
                    right_rotate(tree, x->parent);
                }
            } else {
                y = x->parent->left;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    rb_insert_fixup(tree, x->parent);
                } else {
                    if (element == x->left) {
                        element = x;
                        right_rotate(tree, element);
                        x = element->parent;
                    }
                    x->color = BLACK;
                    x->parent->color = RED;
                    left_rotate(tree, x->parent);
                }
            }
        }
    }
}

void rb_insert(struct node** tree, struct node* element){
    struct node* y = t_nil;
    struct node* x = *tree;
    while (x != t_nil) {
        y = x;

        if (strcmp(element->word, x->word) < 0)
            x = x->left;
        else if (strcmp(element->word, x->word) > 0)
            x = x->right;
    }
    element->parent = y;

    if (y == t_nil)
        (*tree) = element;
    else if (strcmp(element->word, y->word) < 0)
        y->left = element;
    else
        y->right = element;

    element->left  = t_nil;
    element->right = t_nil;
    element->color = RED;
    rb_insert_fixup(tree, element);
}

struct node* get_new_node(char* word){
    struct node* new_node = malloc(sizeof(struct node));
    new_node->word = malloc((strlen(word)+1)* sizeof(char));
    strcpy(new_node->word, word);
    new_node->is_valid = 1;
    return new_node;
}