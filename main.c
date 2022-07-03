#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define HASH_SIZE 51
#define INCREMENT_SIZE_ARRAY 50
#define FORBIDDEN_SYMBOL '$'
#define ALREADY_NOTED '&'

struct filter{
    short int* not_in_position;
    int minimum_number;
    int exact_number;
};

struct hash_element{
    char* word;
    struct hash_element* next;
};

struct hash_table{
    struct hash_element** list;
};

char number_of_occurrences(const char* word, const char* reference_word, int index, int word_size);
int hash(char character);
int fast_atoi(char* str);
struct hash_table* create_table(int table_size);
void hash_insert(struct hash_table** table, char* word, int words_size);
void setup_words(struct hash_table** table, int words_size);
void add_words(struct hash_table** table, int words_size);
void free_hash(struct hash_table** table, int table_size);
void new_game(struct hash_table** table, int words_size);
int hash_function(char* word);
int find_word(struct hash_table** table, char* word_to_find);
char* check_words_first_time(char* word, char* reference_word, struct filter* filters, int word_size, char* model_word);
int count_occurrences(char* word, char ch);
char** get_filtered_words(char*** old_array, struct filter* filters, char* modified_characters, int word_size, const char* model_word, int* number_of_words, int* array_size);
char** first_get_filtered_words(struct hash_table** table, struct filter* filters, char* modified_characters, int word_size, const char* model_word, int* number_of_words, int* array_size);
void add_words_during_game(struct hash_table** table, char*** game_words, struct filter* filters, int words_size, char* reference_word, char* model_word, int* number_of_words, int* array_size);
void quicksort(char*** game_words, int start, int end);

int main(){
    int words_size, c, next_instruction;
    int index = 0;
    char* char_words_size = malloc(11* sizeof(char));
    struct hash_table* table = create_table(HASH_SIZE);

    while((c = getchar()) != '\n' && c != EOF){
        char_words_size[index] = c;
        index++;
    }
    char_words_size[index] = '\0';
    words_size = fast_atoi(char_words_size);
    free(char_words_size);
    setup_words(&table, words_size);
    do{
        next_instruction = getchar();
        while((c = getchar()) != '\n' && c!= EOF);
        if(next_instruction == 'n'){
            new_game(&table, words_size);
            c = getchar();
        } else if(next_instruction == 'i'){
            add_words(&table, words_size);
            c = getchar();
        }
    } while(c != '\n' && c!= EOF);
    free_hash(&table, HASH_SIZE);
    free(table);
    return 0;
}

struct hash_table* create_table(int size){
    int i;
    struct hash_table* table = (struct hash_table*) malloc(sizeof(struct hash_table));
    table->list = (struct hash_element**) calloc(size, sizeof(struct hash_element*));
    for(i = 0; i < size; i++)
        table->list[i] = NULL;
    return table;
}

void setup_words(struct hash_table** table, int words_size){
    int c, index;
    char word_to_add[words_size + 1];
    while ((c = getchar()) != '+'){
        index = 0;
        word_to_add[index] = c;
        index++;
        while ((c = getchar()) != '\n' && c != EOF){
            word_to_add[index] = c;
            index++;
        }
        word_to_add[index] = '\0';
        hash_insert(table, word_to_add, words_size);
    }
}

void hash_insert(struct hash_table** table, char* word, int words_size){
    int index = hash_function(word);
    struct hash_element* new_element = malloc(sizeof(struct hash_element));
    new_element->word = malloc((words_size + 1) * sizeof(char));
    strcpy(new_element->word, word);
    new_element->next = NULL;
    if((*table)->list[index] == NULL){
        (*table)->list[index] = new_element;
    }else{
        new_element->next = (*table)->list[index];
        (*table)->list[index] = new_element;
    }
}

void add_words(struct hash_table** table, int words_size){
    int c, index;
    short int end = 0;
    char word_to_add[words_size + 1];
    do{
        c = getchar();
        if(c == '+'){
            end = 1;
            while ((c = getchar()) != '\n' && c != EOF);
        }else{
            index = 0;
            word_to_add[index] = c;
            index++;
            while ((c = getchar()) != '\n' && c != EOF){
                word_to_add[index] = c;
                index++;
            }
            word_to_add[index] = '\0';
            hash_insert(table, word_to_add, words_size);
        }
    } while (end == 0);
}

void free_element(struct hash_element* element){
    if(element != NULL){
        free_element(element->next);
        free(element->word);
        free(element);
    }
}

void free_hash(struct hash_table** table, int table_size){
    int i;
    for(i = 0; i < table_size; i++){
        free_element((*table)->list[i]);
    }
    free((*table)->list);
}

int check_if_add_to_game(char* word, char* reference_word, struct filter* filters, int words_size, const char* model_word){
    int i, index, index_ref, n;
    for(i = 0; i < words_size; i++){
        index = hash(word[i]);
        if(filters[index].exact_number == 0 || filters[index].not_in_position[i] == 1 || (model_word[i] == reference_word[i] && reference_word[i] != word[i]))
            return 0;
        else{
            index_ref = hash(reference_word[i]);
            n = count_occurrences(word, reference_word[i]);
            if(n < filters[index_ref].minimum_number || (filters[index_ref].exact_number != -1 && n != filters[index_ref].exact_number))
                return 0;
        }
    }
    return 1;
}

void add_words_during_game(struct hash_table** table, char*** game_words, struct filter* filters, int words_size, char* reference_word, char* model_word, int* number_of_words, int* array_size){
    int c, index;
    short int end = 0;
    char word_to_add[words_size + 1];
    do{
        c = getchar();
        if(c == '+'){
            end = 1;
            while ((c = getchar()) != '\n' && c != EOF);
        }else{
            index = 0;
            word_to_add[index] = c;
            index++;
            while ((c = getchar()) != '\n' && c != EOF){
                word_to_add[index] = c;
                index++;
            }
            word_to_add[index] = '\0';
            hash_insert(table, word_to_add, words_size);
            if(check_if_add_to_game(word_to_add, reference_word, filters, words_size, model_word) == 1){
                if((*number_of_words) >= *array_size){
                    (*array_size) += INCREMENT_SIZE_ARRAY;
                    (*game_words) = (char**)realloc(*game_words, *array_size*sizeof(char*));
                }
                (*game_words)[*number_of_words] = malloc((words_size + 1)*sizeof(char));
                strcpy((*game_words)[*number_of_words], word_to_add);
                (*number_of_words)++;
            }
        }
    } while (end == 0);
}

void new_game(struct hash_table** table, int words_size){
    char reference_word[words_size + 1], *modified_characters, word_to_analyze[words_size + 1];
    char* char_number_of_controls = malloc(11* sizeof(char));
    int c, number_of_controls, counter, next_instruction, i;
    short int end = 0;
    int index = 0;
    int array_size = INCREMENT_SIZE_ARRAY;
    int number_of_words = 0;
    char** game_words;
    struct filter filters[64];
    struct hash_element* temp;
    char* model_word = malloc((words_size + 1)* sizeof(char));

    for(i = 0; i < words_size; i++){
        model_word[i] = FORBIDDEN_SYMBOL;
    }

    for(i = 0; i < 64; i++){
        filters[i].not_in_position = calloc(words_size, sizeof(short int));
        filters[i].minimum_number = 0;
        filters[i].exact_number = -1;
    }

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
    number_of_controls = fast_atoi(char_number_of_controls);
    free(char_number_of_controls);
    counter = number_of_controls;
    do{
        index = 0;
        c = getchar();
        if(c == '+'){
            next_instruction = getchar();
            while ((c = getchar()) != '\n' && c != EOF);
            if(next_instruction == 'i'){
                if(counter == number_of_controls)
                    add_words(table, words_size);
                else
                    add_words_during_game(table, &game_words, filters, words_size, reference_word, model_word, &number_of_words, &array_size);
            } else {
                if(counter == number_of_controls){
                    game_words = malloc(array_size*sizeof(char*));
                    for(i = 0; i < HASH_SIZE; i++){
                        temp = (*table)->list[i];
                        while(temp != NULL){
                            if(number_of_words >= array_size){
                                array_size += INCREMENT_SIZE_ARRAY;
                                game_words = realloc(game_words, array_size*sizeof(char*));
                            }
                            game_words[number_of_words] = malloc((words_size + 1)*sizeof(char));
                            strcpy(game_words[number_of_words], temp->word);
                            number_of_words++;
                            temp = temp->next;
                        }
                    }
                    quicksort(&game_words, 0, number_of_words - 1);
                    for(i = 0; i < number_of_words; i++){
                        printf("%s\n", game_words[i]);
                        free(game_words[i]);
                    }
                    free(game_words);
                    number_of_words = 0;
                    array_size = 50;
                } else {
                    quicksort(&game_words, 0, number_of_words - 1);
                    for(i = 0; i < number_of_words; i++){
                        printf("%s\n", game_words[i]);
                    }
                }
            }
        } else {
            word_to_analyze[index] = c;
            index++;
            while ((c = getchar()) != '\n' && c != EOF){
                word_to_analyze[index] = c;
                index++;
            }
            word_to_analyze[index] = '\0';
            if(find_word(table, word_to_analyze) == 1){
                if(strcmp(reference_word, word_to_analyze) == 0){
                    printf("ok\n");
                    end = 1;
                }else{
                    modified_characters = check_words_first_time(word_to_analyze, reference_word, filters, words_size, model_word);
                    if(counter == number_of_controls){
                        game_words = first_get_filtered_words(table, filters, modified_characters, words_size, model_word, &number_of_words, &array_size);
                    } else {
                        game_words = get_filtered_words(&game_words, filters, modified_characters, words_size, model_word, &number_of_words, &array_size);
                    }
                    counter--;
                    free(modified_characters);
                    printf("%d\n", number_of_words);
                }
            }else{
                printf("not_exists\n");
            }
        }
    } while(end == 0 && counter > 0);

    if(counter == 0 && end == 0)
        printf("ko\n");

    if(counter != number_of_controls){
        for(i = 0; i < number_of_words; i++){
            free(game_words[i]);
        }
        free(game_words);
    }
    for(i = 0; i < 64; i++){
        free(filters[i].not_in_position);
    }
    free(model_word);
}

int find_word(struct hash_table** table, char* word_to_find){
    int index = hash_function(word_to_find);
    struct hash_element* temp = (*table)->list[index];
    if((*table)->list[index] == NULL)
        return 0;
    while (temp != NULL){
        if(strcmp(temp->word, word_to_find) == 0)
            return 1;
        temp = temp->next;
    }
    return 0;
}

char* check_words_first_time(char* word, char* reference_word, struct filter* filters, int word_size, char* model_word){
    int i, n, index, count = 0;
    char* modified_characters = malloc((word_size + 1)* sizeof(char));
    int *temp_min = calloc(64, sizeof(int));
    short int *temp_exactly = calloc(64, sizeof(short int));
    char c;
    for(i = 0; i < word_size; i++){
        index = hash(word[i]);
        if(word[i] == reference_word[i]){
            if(temp_min[index] < filters[index].minimum_number){
                temp_min[index]++;
            } else {
                temp_min[index]++;
                filters[index].minimum_number++;
            }
            if(temp_exactly[index] == 1){
                filters[index].exact_number = temp_min[index];
            }
            printf("+");
            model_word[i] = word[i];
            modified_characters[count] = word[i];
            count++;
        }else{
            n = count_occurrences(reference_word, word[i]);
            if(n == 0){
                printf("/");
                filters[index].exact_number = 0;
                modified_characters[count] = word[i];
                count++;
            } else {
                c = number_of_occurrences(word, reference_word, i, word_size);
                printf("%c", c);
                filters[index].not_in_position[i] = 1;
                if(c == '|'){
                    if(temp_min[index] < filters[index].minimum_number){
                        temp_min[index]++;
                    } else {
                        temp_min[index]++;
                        filters[index].minimum_number++;
                    }
                    if(temp_exactly[index] == 1){
                        filters[index].exact_number = temp_min[index];
                    }
                } else {
                    temp_exactly[index] = 1;
                    filters[index].exact_number = temp_min[index];
                }
                modified_characters[count] = word[i];
                count++;
            }
        }
    }
    free(temp_min);
    free(temp_exactly);
    modified_characters[count] = '\0';
    printf("\n");
    return modified_characters;
}

char** get_filtered_words(char*** old_array, struct filter* filters, char* modified_characters, int word_size, const char* model_word, int* number_of_words, int* array_size){
    int i, index, correct, n, j;
    int new_number_of_words = 0, new_array_size = 50;
    char word_to_add[word_size + 1];
    char** new_array_words = malloc(new_array_size*sizeof(char*));
    for(j = 0; j < *number_of_words; j++){
        correct = 1;
        for(i = 0; i < word_size; i++){
            index = hash(modified_characters[i]);
            if(((*old_array)[j][i] == modified_characters[i] && filters[index].not_in_position[i] == 1) || (model_word[i] != FORBIDDEN_SYMBOL && model_word[i] != ALREADY_NOTED && model_word[i] != (*old_array)[j][i])){
                correct = 0;
                break;
            } else {
                n = count_occurrences((*old_array)[j], modified_characters[i]);
                if(n < filters[index].minimum_number || (filters[index].exact_number != -1 && n != filters[index].exact_number)){
                    correct = 0;
                    break;
                }
            }
        }
        if(correct == 1){
            strcpy(word_to_add, (*old_array)[j]);
            free((*old_array)[j]);
            if(new_number_of_words >= new_array_size){
                new_array_size += INCREMENT_SIZE_ARRAY;
                new_array_words = (char**)realloc(new_array_words, new_array_size*sizeof(char*));
            }
            new_array_words[new_number_of_words] = malloc((word_size + 1)*sizeof(char));
            strcpy(new_array_words[new_number_of_words], word_to_add);
            new_number_of_words++;
        }else{
            free((*old_array)[j]);
        }
    }
    (*number_of_words) = new_number_of_words;
    (*array_size) = new_array_size;
    free(*old_array);
    return new_array_words;
}

char** first_get_filtered_words(struct hash_table** table, struct filter* filters, char* modified_characters, int word_size, const char* model_word, int* number_of_words, int* array_size){
    char** game_words = malloc((*array_size)*sizeof(char*));
    int i, j, index, n;
    short int correct;
    struct hash_element* temp;
    for(i = 0; i < HASH_SIZE; i++){
        temp = (*table)->list[i];
        while (temp != NULL){
            correct = 1;
            for(j = 0; j < word_size; j++){
                index = hash(modified_characters[j]);
                if((temp->word[j] == modified_characters[j] && filters[index].not_in_position[j] == 1) || (model_word[j] != FORBIDDEN_SYMBOL && model_word[j] != ALREADY_NOTED && temp->word[j] != model_word[j])){
                    correct = 0;
                    break;
                } else {
                    n = count_occurrences(temp->word, modified_characters[j]);
                    if(n < filters[index].minimum_number || (filters[index].exact_number != -1 && n != filters[index].exact_number)){
                        correct = 0;
                        break;
                    }
                }
            }
            if(correct == 1){
                if(*number_of_words >= *array_size){
                    (*array_size) = (*array_size) + INCREMENT_SIZE_ARRAY;
                    game_words = (char**)realloc(game_words, (*array_size)*sizeof(char*));
                }
                game_words[(*number_of_words)] = malloc((word_size + 1)*sizeof(char));
                strcpy(game_words[(*number_of_words)], temp->word);
                (*number_of_words)++;
            }
            temp = temp->next;
        }
    }
    return game_words;
}

char number_of_occurrences(const char* word, const char* reference_word, int index, int word_size) {
    int i, n = 0, c = 0, s = 0;
    for (i = 0; i < word_size; i++) {
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

int count_occurrences(char* word, const char ch){
    int c = 0;
    char n;
    while ((n=*word++), ((n==ch)? ++c : 0), n);
    return c;
}

int hash_function(char* word){
    unsigned long hash = 5381;
    int c;
    while((c = (unsigned char)*word++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_SIZE;
}

int fast_atoi(char* str){
    int val = 0;
    while (*str){
        val = val*10 + (*str++ - '0');
    }
    return val;
}

void swap(char** a, char** b){
    char* temp = *a;
    *a = *b;
    *b = temp;
}

int partition(char*** game_words, int start, int end){
    char* pivot = (*game_words)[end];
    int i = start - 1;
    int j;

    for(j = start; j < end; j++){
        if(strcmp((*game_words)[j], pivot) <= 0){
            i++;
            swap(&(*game_words)[i], &(*game_words)[j]);
        }
    }
    swap(&(*game_words)[i+1], &(*game_words)[end]);
    return (i+1);
}

int randomized_partition(char*** game_words, int start, int end){
    srand(time(0));
    int random = start + rand() % (end - start);
    char* temp = (*game_words)[random];
    (*game_words)[random] = (*game_words)[start];
    (*game_words)[start] = temp;
    return partition(game_words, start, end);
}

void quicksort(char*** game_words, int start, int end){
    if(start < end){
        int p = randomized_partition(game_words, start, end);
        quicksort(game_words, start, p - 1);
        quicksort(game_words, p + 1, end);
    }
}