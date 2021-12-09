//Projekt#2 IZP. Tym Iossif Kutateladze a Vladislav Lysenko

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h> 

#define UNIVERZUM "U"
#define MNOZINY "S"
#define PRIKAZ "C"
#define RELACE "R"

#define EMPTY "empty"
#define CARD "card"
#define COMPLEMENT "complement"
#define UNION "union"
#define INTERSECT "intersect"
#define MINUS "minus"
#define SUBSETEQ "subseteq"
#define SUBSET "subset"
#define EQUALS "equals"

#define REFLEXIVE "reflexive"
#define SYMMETRIC "symmetric"
#define ANTISYMMETRIC "antisymmetric"
#define TRANSITIVE "transitive"
#define FUNCTION "function"
#define DOMAIN "domain"
#define CODOMAIN "codomain"
#define INJECTIVE "injective"
#define SURJECTIVE "surjective"
#define BIJECTIVE "bijective"

#define LIMIT 1000
#define MAX_LENGTH 30

typedef struct tree{
    char *key;
    struct tree *left;
    struct tree *right;
    struct tree *parent;
} node;

typedef struct tree_pair{
    char *first_key;
    char *second_key;
    struct tree_pair *left;
    struct tree_pair *right;
    struct tree_pair *parent;
} node_pair;


node *sets_root[LIMIT];
node_pair *seances_root[LIMIT];
int sets_length[LIMIT]; //количество аргументов в множесте
int seances_length[LIMIT]; //количество аргументов в отношении

//node - узел, хранящий в себе элемент множества

// создает корень дерева
node *create(node *root, char *key){
    node *tmp = malloc(sizeof(node));
    tmp -> key = key;
    tmp -> parent = NULL;
    tmp -> left = tmp -> right = NULL;
    root = tmp;
    return root;
}

// добавляет элемент в дерева
node *add(node *root, char *key){
    node *root2 = root, *root3 = NULL;
    node *tmp = malloc(sizeof(node));
    tmp -> key = key;
    while (root2 != NULL){
        root3 = root2;
        if (strcmp(key, root2 -> key) == -1)
            root2 = root2 -> left;
        else
            root2 = root2 -> right;
    }
    tmp -> parent = root3;
    tmp -> left = NULL;
    tmp -> right = NULL;
    if (strcmp(key, root3 -> key) == -1) 
        root3 -> left = tmp;
    else
        root3 -> right = tmp;

    return root;
}

// поиск по дереву
node *search(node * root, char *key){
    if ((root == NULL) || strcmp(root -> key, key) == 0)
        return root;

    if (strcmp(key, root -> key) == -1)
        return search(root -> left, key);
    else
        return search(root -> right, key);
}

// вывод дерево
void preorder(node *root){
    if (root == NULL)
        return;
    if (root -> key)
        printf("%s ", root -> key);
    preorder(root -> left);
    preorder(root -> right);
}

//тоже самое, только для двух элементов
node_pair *create_pair(node_pair *root, char *first_key, char *second_key){
    node_pair *tmp = malloc(sizeof(node_pair));
    tmp -> first_key = first_key;
    tmp -> second_key = second_key;
    tmp -> parent = NULL;
    tmp -> left = tmp -> right = NULL;
    root = tmp;
    return root;
}

node_pair *add_pair(node_pair *root, char *first_key, char *second_key){
    node_pair *root2 = root, *root3 = NULL;
    node_pair *tmp = malloc(sizeof(node_pair));
    tmp -> first_key = first_key;
    tmp -> second_key = second_key;
    while (root2 != NULL){
        root3 = root2;
        if (strcmp(first_key, root2 -> first_key) == -1 ||
           (strcmp(first_key, root2 -> first_key) == 0 && strcmp(second_key, root2->second_key) == -1))
            root2 = root2 -> left;
        else
            root2 = root2 -> right;
    }
    tmp -> parent = root3;
    tmp -> left = NULL;
    tmp -> right = NULL;
    if (strcmp(first_key, root3 -> first_key) == -1 || 
       (strcmp(first_key, root3 -> first_key) == 0 && strcmp(second_key, root3->second_key) == -1))
        root3 -> left = tmp;
    else
        root3 -> right = tmp;

    return root;
}

node_pair *search_pair(node_pair *root, char *first_key, char *second_key){
    if ((root == NULL) || (strcmp(root -> first_key, first_key) == 0 && strcmp(root -> second_key, second_key) == 0))
        return root;

    if (strcmp(first_key, root -> first_key) == -1 || 
        (strcmp(first_key, root -> first_key) == 0 && strcmp(second_key, root->second_key) == -1))
        return search_pair(root -> left, first_key, second_key);
    else
        return search_pair(root -> right, first_key, second_key);
}

void preorder_pair(node_pair *root){
    if (root == NULL)
        return;
    if (strcmp(root -> first_key, ""))
        printf("(%s %s) ", root -> first_key, root -> second_key);
    preorder_pair(root -> left);
    preorder_pair(root -> right);
}

//разделить строку на аргументы
char** str_split(char* a_str, const char a_delim, int *length){
    char buffer[1024];
    *length = 0;
    sprintf(buffer, "%s", a_str);
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    char *token = strtok(buffer, delim);
    while (token) {
        token = strtok(NULL, delim);
        (*length)++;
    }

    char **result = malloc(*length * sizeof(char *));
    sprintf(buffer, "%s", a_str);
    token = strtok(buffer, delim);
    int i = 0;
    result[i] = malloc(sizeof(char)*strlen(token));
    strcpy(result[i], token);
    i++;
    while (token) {
        token = strtok(NULL, delim);
        if(token){
            result[i] = malloc(sizeof(char)*strlen(token));
            strcpy(result[i], token);
            i++;
        }
    }
    return result;
}

//конвертация в int
int to_integer(char *s){
    int number = 0;
    for(size_t i = 0; i < strlen(s); i++){
        number = number*10 + s[i] - '0';
    }
    return number;
}

void _get_all(node *root, int *cur_index, char ** result){
    if(root == NULL){
        return;
    }
    if(strcmp(root -> key, "")){
        result[*cur_index] = malloc(sizeof(char)*strlen(root->key));
        strcpy(result[*cur_index], root->key);
        (*cur_index)++;
    }
    _get_all(root->left, cur_index, result);
    _get_all(root->right, cur_index, result);
}

char ** get_all(int set_index){
    char **result = malloc(sizeof(char *) * (sets_length[set_index]+1));
    int *cur_index = malloc(sizeof(int));
    *cur_index = 0;
    _get_all(sets_root[set_index], cur_index, result);
    return result;
}

char ** get_all_root(node *root, int length){
    char **result = malloc(sizeof(char *) * (length+10));
    int *cur_index = malloc(sizeof(int));
    *cur_index = 0;
    _get_all(root, cur_index, result);
    free(cur_index);
    return result;
}

//ЭТО КОМАНДЫ ПРИ ОПЕРАЦИЯХ (НЕ R)////////////////////
void empty_prikaz_handler(int set_index){
    if(sets_root[set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(set_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    //проверка на пустое множество
    if(sets_length[set_index] == 0){
        printf("true\n");
    } else {
        printf("false\n");
    }
}

void card_prikaz_handler(int set_index){
    if(set_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    
    if(sets_root[set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    //выводит количество аргументов
    printf("%d\n", sets_length[set_index]);
}

void complement_prikaz_handler(int set_index){
    if(sets_root[set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(set_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    char **keys = get_all(1);
    printf("S");
    for(int i = 0; i < sets_length[1]; i++){
        if(search(sets_root[set_index], keys[i]) == NULL){
            printf(" %s", keys[i]);
        }
    }
    printf("\n");
    for(int i = 0; i < sets_length[1]; i++){
        free(keys[i]);
    }
    free(keys);
}

void intersect_prikaz_handler(int first_set_index, int second_set_index){
    if(sets_root[first_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[second_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(first_set_index <= 0 || second_set_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    char ** keys = get_all(first_set_index);
    printf("S");
    for(int i = 0; i < sets_length[first_set_index]; i++){
        if(search(sets_root[second_set_index], keys[i]) != NULL){
            printf(" %s", keys[i]);
        }
    }
    printf("\n");
    for(int i = 0; i < sets_length[first_set_index]; i++){
       free(keys[i]);
    }
    free(keys);
}

void union_prikaz_handler(int first_set_index, int second_set_index){
    if(sets_root[first_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[second_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(first_set_index <= 0 || second_set_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    node *root = malloc(sizeof(node));
    root = create(root, "");
    int length = 0;    
    char ** first_keys = get_all(first_set_index);
    char ** second_keys = get_all(second_set_index);
    for(int i = 0; i < sets_length[first_set_index]; i++){
        add(root, first_keys[i]);
        length++;
    }
    for(int i = 0; i < sets_length[second_set_index]; i++){
        if(search(root, second_keys[i]) == NULL){
            length++;
            add(root, second_keys[i]);
        }
    }

    printf("S");
    char ** keys = get_all_root(root, length);
    for(int i = 0; i < length; i++){
        printf(" %s", keys[i]);
    }
    printf("\n");
    for(int i = 0; i < length; i++){
        free(keys[i]);
    }
    free(keys);

    for(int i = 0; i < sets_length[first_set_index]; i++){
        free(first_keys[i]);
    }
    free(first_keys);

    for(int i = 0; i < sets_length[second_set_index]; i++){
        free(second_keys[i]);
    }
    free(first_keys);

    free(root);
}

void minus_prikaz_handler(int first_set_index, int second_set_index){
    if(sets_root[first_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[second_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(first_set_index <= 0 || second_set_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    char ** keys = get_all(first_set_index);
    printf("S");
    for(int i = 0; i < sets_length[first_set_index]; i++){
        if(search(sets_root[second_set_index], keys[i]) == NULL){
            printf(" %s", keys[i]);
        }
    }
    printf("\n");
    for(int i = 0; i < sets_length[first_set_index]; i++){
        free(keys[i]);
    }
    free(keys);
}

void subseteq_prikaz_handler(int first_set_index, int second_set_index){
    if(sets_root[first_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[second_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(first_set_index == second_set_index){
        printf("true");
        return;
    }
    if(first_set_index <= 0 || second_set_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    char ** keys = get_all(first_set_index);
    int flag = 1;
    for(int i = 0; i < sets_length[first_set_index]; i++){
        if(search(sets_root[second_set_index], keys[i]) == NULL){
            flag = 0;
            break;
        }
    }

    if(flag){
        printf("true\n");
    } else {
        printf("false\n");
    }
    for(int i = 0; i < sets_length[first_set_index]; i++){
        free(keys[i]);
    }
    free(keys);
}

void subset_prikaz_handler(int first_set_index, int second_set_index){
    if(sets_root[first_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[second_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(first_set_index == second_set_index){
        printf("false");
        return;        
    }
    if(sets_length[first_set_index] == 0){
        printf("true");
        return;
    }
    if(first_set_index <= 0 || second_set_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    char ** keys = get_all(first_set_index);
    int flag = 0;
    for(int i = 0; i < sets_length[first_set_index]; i++){
        if(search(sets_root[second_set_index], keys[i]) != NULL){
            flag = 1;
            break;
        }
    }
    if(flag){
        printf("true\n");
    } else {
        printf("false\n");
    }

    for(int i = 0; i < sets_length[first_set_index]; i++){
        free(keys[i]);
    }
    free(keys);
}

void equals_prikaz_handler(int first_set_index, int second_set_index){
    if(sets_root[first_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[second_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(first_set_index <= 0 || second_set_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    char ** keys = get_all(first_set_index);
    int flag = (sets_length[first_set_index] == sets_length[second_set_index]);
    for(int i = 0; i < sets_length[first_set_index] && flag; i++){
        if(search(sets_root[second_set_index], keys[i]) == NULL){
            flag = 0;
            break;
        }
    }
    if(flag){
        printf("true\n");
    } else {
        printf("false\n");
    }

    for(int i = 0; i < sets_length[first_set_index]; i++){
        free(keys[i]);
    }
    free(keys);
}
////////////////////////////////////////////////////////////////////////

void _get_first_keys(node_pair *root, int *cur_index, char ** result){
    if(root == NULL){
        return;
    }
    if(strcmp(root -> first_key, "")){
        result[*cur_index] = malloc(sizeof(char)*strlen(root -> first_key));
        strcpy(result[*cur_index], root -> first_key);
        (*cur_index)++;
    }
    _get_first_keys(root->left, cur_index, result);
    _get_first_keys(root->right, cur_index, result);
}

char ** get_first_keys(int seance_index){
    char **result = malloc(sizeof(char *) * seances_length[seance_index]);
    int *cur_index = malloc(sizeof(int));
    *cur_index = 0;
    _get_first_keys(seances_root[seance_index], cur_index, result);
    return result;
}

void _get_second_keys(node_pair *root, int *cur_index, char ** result){
    if(root == NULL){
        return;
    }
    if(strcmp(root -> second_key, "")){
        result[*cur_index] = malloc(sizeof(char)*strlen(root -> second_key));
        strcpy(result[*cur_index], root -> second_key);
        (*cur_index)++;
    }
    _get_second_keys(root->left, cur_index, result);
    _get_second_keys(root->right, cur_index, result);
}

char ** get_second_keys(int seance_index){
    char **result = malloc(sizeof(char *) * seances_length[seance_index]);
    int *cur_index = malloc(sizeof(int));
    *cur_index = 0;
    _get_second_keys(seances_root[seance_index], cur_index, result);
    return result;
}

///ЭТО КОМАНДЫ ПРИ ОПЕРАЦИЯХ С R /////////////////////////////////////////
void reflexive_command_handler(int seance_index){
    if(seances_root[seance_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(seances_length[seance_index] == 0){
        if(sets_length[1] == 0){
            printf("true\n");
        } else {
            printf("false\n");
        }
    }

    if(seance_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    char ** first_keys = get_first_keys(seance_index);
    char ** second_keys = get_second_keys(seance_index);
    int flag = 1;
    for(int i = 0; i < seances_length[seance_index]; i++){
        if(search_pair(seances_root[seance_index], first_keys[i], first_keys[i]) == NULL ||
           search_pair(seances_root[seance_index], second_keys[i], second_keys[i]) == NULL){
               flag = 0;
               break;
           }
    }
    if(flag){
        printf("true\n");
    } else {
        printf("false\n");
    }
}

void symmetric_command_handler(int seance_index){
    if(seances_root[seance_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(seance_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    char ** first_keys = get_first_keys(seance_index);
    char ** second_keys = get_second_keys(seance_index);
    int flag = 1;
    for(int i = 0; i < seances_length[seance_index]; i++){
        if(search_pair(seances_root[seance_index], second_keys[i], first_keys[i]) == NULL){
            flag = 0;
            break;
        }
    }
    if(flag){
        printf("true\n");
    } else {
        printf("false\n");
    }
}

void antisymmetric_command_handler(int seance_index){
    if(seances_root[seance_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(seance_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    char ** first_keys = get_first_keys(seance_index);
    char ** second_keys = get_second_keys(seance_index);
    int flag = 1;
    for(int i = 0; i < seances_length[seance_index]; i++){
        if(search_pair(seances_root[seance_index], second_keys[i], first_keys[i]) != NULL){
            if(strcmp(first_keys[i], second_keys[i])){
                flag = 0;
                break;
            }
        }
    }
    if(flag){
        printf("true\n");
    } else {
        printf("false\n");
    }
}

void transitive_command_handler(int seance_index){
    if(seances_root[seance_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(seance_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    char ** first_keys = get_first_keys(seance_index);
    char ** second_keys = get_second_keys(seance_index);
    for(int i = 0; i < seances_length[seance_index]; i++){    
        for(int j = 0; j < seances_length[seance_index]; j++){
            if(i == j){
                continue;
            }
            if(strcmp(first_keys[j], second_keys[i]) == 0){
                if(search_pair(seances_root[seance_index], first_keys[i], second_keys[j]) == NULL){
                    printf("false\n");
                    return;
                }
            }
        }
    }
    printf("true\n");
    return;        
    node *root = NULL;
    root = create(root, "");
    int length = 0;
    for(int i = 0; i < seances_length[seance_index]; i++){
        if(search(root, first_keys[i]) == NULL){
            length++;
            add(root, first_keys[i]);
        }
        if(search(root, second_keys[i]) == NULL){
            length++;
            add(root, second_keys[i]);
        }
    }
    char **keys = get_all_root(root, length);
    for(int i = 0; i < length; i++){
        for(int j = i+1; j < length; j++){
            for(int k = j+1; k < length; k++){
                if(search_pair(seances_root[seance_index], keys[i], keys[j]) != NULL && search_pair(seances_root[seance_index], keys[i], keys[k]) != NULL){
                    if(search_pair(seances_root[seance_index], keys[j], keys[k]) == NULL && search_pair(seances_root[seance_index], keys[k], keys[j]) == NULL){
                        printf("false\n");
                        return;        
                    }
                }
            }
        }
    }
    printf("true\n");
}

void function_command_handler(int seance_index){
    if(seances_root[seance_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(seance_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    char ** first_keys = get_first_keys(seance_index);
    int flag = 1;
    for(int i = 0; i < seances_length[seance_index]; i++){
        for(int j = i+1; j < seances_length[seance_index]; j++){
            if(strcmp(first_keys[i], first_keys[j]) == 0){
                flag = 0;
                break;
            }
        }
    }
    if(flag){
        printf("true\n");
    } else {
        printf("false\n");
    }
    for(int i = 0; i < seances_length[seance_index]; i++){
        free(first_keys[i]);
    }
    free(first_keys);
}

void domain_command_handler(int seance_index){
    if(seances_root[seance_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(seance_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    node *root = NULL;
    root = create(root, "");
    int length = 0;
    char ** first_keys = get_first_keys(seance_index);
    for(int i = 0; i < seances_length[seance_index]; i++){
        if(search(root, first_keys[i]) == NULL){
            add(root, first_keys[i]);
            length++;
        }
    }
    printf("S");
    char ** keys = get_all_root(root, length);
    for(int i = 0; i < length; i++){
        printf(" %s", keys[i]);
    }
    printf("\n");
}

void codomain_command_handler(int seance_index){
    if(seances_root[seance_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(seance_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    node *root = NULL;
    root = create(root, "");
    int length = 0;
    char ** second_keys = get_second_keys(seance_index);
    for(int i = 0; i < seances_length[seance_index]; i++){
        if(search(root, second_keys[i]) == NULL){
            add(root, second_keys[i]);
            length++;
        }
    }
    printf("S");
    char ** keys = get_all_root(root, length);
    for(int i = 0; i < length; i++){
        printf(" %s", keys[i]);
    }
    printf("\n");
}

void injective_command_handler(int seance_index, int first_set_index, int second_set_index){
    if(seances_root[seance_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[first_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[second_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(first_set_index <= 0 || second_set_index <= 0 || seance_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    char ** first_keys = get_all(first_set_index);
    char ** second_keys = get_all(second_set_index);
    
    char ** first_keys_ = get_first_keys(seance_index);
    char ** second_keys_ = get_second_keys(seance_index);
    for(int i = 0; i < seances_length[seance_index]; i++){
        if(search(sets_root[first_set_index], first_keys_[i]) == NULL){
            printf("false\n");
            return;
        }
    }
    for(int i = 0; i < seances_length[seance_index]; i++){
        if(search(sets_root[second_set_index], second_keys_[i]) == NULL){
            printf("false\n");
            return;
        }
    }
    for(int i = 0; i < sets_length[first_set_index]; i++){
        if(search(sets_root[second_set_index], first_keys[i]) != NULL){
            printf("false\n");
            return;
        }
        for(int j = 0; j < sets_length[first_set_index]; j++){
            if(search_pair(seances_root[seance_index], first_keys[i], first_keys[j]) != NULL){
                printf("false\n");
                return;
            }
        }
    }
    for(int i = 0; i < sets_length[second_set_index]; i++){
        if(search(sets_root[first_set_index], second_keys[i]) != NULL){
            printf("false\n");
            return;
        }
        for(int j = 0; j < sets_length[second_set_index]; j++){
            if(search_pair(seances_root[seance_index], second_keys[i], second_keys[j]) != NULL){
                printf("false\n");
                return;
            }
        }
    }

    for(int i = 0; i < sets_length[first_set_index]; i++){
        for(int j = i+1; j < sets_length[first_set_index]; j++){
            for(int k = 0; k < sets_length[second_set_index]; k++){
                if(search_pair(seances_root[seance_index], first_keys[i], second_keys[k]) != NULL &&
                   search_pair(seances_root[seance_index], first_keys[j], second_keys[k]) != NULL){
                       if(strcmp(first_keys[i], first_keys[j])){
                           printf("false\n");
                           return;
                       }
                   }
            }
        }
    }

    for(int i = 0; i < sets_length[first_set_index]; i++){
        int cnt = 0;
        for(int j = 0; j < sets_length[second_set_index]; j++){
            if(search_pair(seances_root[seance_index], first_keys[i], second_keys[j]) != NULL){
                cnt++;
            }
        }
        if(cnt == 0){
            printf("false\n");
            return;
        }
    }

    printf("true\n");
}

void surjective_command_handler(int seance_index, int first_set_index, int second_set_index){
    if(seances_root[seance_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[first_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[second_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(first_set_index <= 0 || second_set_index <= 0 || seance_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }

    char ** first_keys = get_all(first_set_index);
    char ** second_keys = get_all(second_set_index);

    if(seances_length[seance_index] != sets_length[first_set_index]){
        printf("false\n");
        return;
    }

    char ** first_keys_ = get_first_keys(seance_index);
    char ** second_keys_ = get_second_keys(seance_index);
    for(int i = 0; i < seances_length[seance_index]; i++){
        if(search(sets_root[first_set_index], first_keys_[i]) == NULL){
            printf("false\n");
            return;
        }
    }

    for(int i = 0; i < sets_length[first_set_index]; i++){
        if(search(sets_root[second_set_index], first_keys[i]) != NULL){
            printf("false\n");
            return;
        }
        for(int j = 0; j < sets_length[first_set_index]; j++){
            if(search_pair(seances_root[seance_index], first_keys[i], first_keys[j]) != NULL){
                printf("false\n");
                return;
            }
        }
    }
    for(int i = 0; i < sets_length[second_set_index]; i++){
        if(search(sets_root[first_set_index], second_keys[i]) != NULL){
            printf("false\n");
            return;
        }
        for(int j = 0; j < sets_length[second_set_index]; j++){
            if(search_pair(seances_root[seance_index], second_keys[i], second_keys[j]) != NULL){
                printf("false\n");
                return;
            }
        }
    }

    for(int i = 0; i < seances_length[seance_index]; i++){
        if(search(sets_root[second_set_index], second_keys_[i]) == NULL){
            printf("false\n");
            return;
        }
    }

    for(int i = 0; i < sets_length[second_set_index]; i++){
        int flag = 0;
        for(int j = 0; j < sets_length[first_set_index]; j++){
            if(search_pair(seances_root[seance_index], first_keys[j], second_keys[i]) != NULL){
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            printf("false\n");
            return;
        }
    }
    printf("true\n");
}

void bijective_command_handler(int seance_index, int first_set_index, int second_set_index){
    if(seances_root[seance_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[first_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(sets_root[second_set_index] == NULL){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    if(first_set_index <= 0 || second_set_index <= 0 || seance_index <= 0){
        fprintf(stderr, "Chyba!\n");
        exit(EXIT_FAILURE);
    }
    char ** first_keys = get_all(first_set_index);
    char ** second_keys = get_all(second_set_index);
    if(sets_length[first_set_index] != sets_length[second_set_index] || sets_length[first_set_index] != seances_length[seance_index]){
        printf("false\n");
        return;
    }
    for(int i = 0; i < sets_length[first_set_index]; i++){
        int cnt = 0;
        for(int j = 0; j < sets_length[second_set_index]; j++){
            if(search_pair(seances_root[seance_index], first_keys[i], second_keys[j]) != NULL){
                cnt++;
            }
        }
        if(cnt != 1){
            printf("false\n");
            return;
        }
    }
    for(int i = 0; i < sets_length[first_set_index]; i++){
        int cnt = 0;
        for(int j = 0; j < sets_length[second_set_index]; j++){
            if(search_pair(seances_root[seance_index], first_keys[j], second_keys[i]) != NULL){
                cnt++;
            }
        }
        if(cnt != 1){
            printf("false\n");
            return;
        }
    }   
    printf("true\n");
}
///////////////////////////////////////////////////////////////////////////////////////////

//Проверяет, заглавный ли указанный знак
int is_alpha(char c){
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

int is_digit(char c){
    return ('0' <= 'c' && c <= '9');
}

//проверка аргумента
int check_word(char *s){
    for(size_t i = 0; i < strlen(s); i++){
        if(!is_alpha(s[i]) && s[i] != '_'){
            return 0;
        }
    }
    return 1;
}

int is_command(char *s){
    int ok = 0;
    ok |= strcmp(s, EMPTY) == 0;
    ok |= strcmp(s, CARD) == 0;
    ok |= strcmp(s, COMPLEMENT) == 0;
    ok |= strcmp(s, UNION) == 0;
    ok |= strcmp(s, INTERSECT) == 0;
    ok |= strcmp(s, MINUS) == 0;
    ok |= strcmp(s, SUBSETEQ) == 0;
    ok |= strcmp(s, SUBSET) == 0;
    ok |= strcmp(s, EQUALS) == 0;
    ok |= strcmp(s, REFLEXIVE) == 0;
    ok |= strcmp(s, SYMMETRIC) == 0;
    ok |= strcmp(s, ANTISYMMETRIC) == 0;
    ok |= strcmp(s, TRANSITIVE) == 0;
    ok |= strcmp(s, FUNCTION) == 0;
    ok |= strcmp(s, DOMAIN) == 0;
    ok |= strcmp(s, CODOMAIN) == 0;
    ok |= strcmp(s, INJECTIVE) == 0;
    ok |= strcmp(s, SURJECTIVE) == 0;
    ok |= strcmp(s, BIJECTIVE) == 0;
    return ok;
}

int is_true_false(char *s){
    return strcmp(s, "true") == 0 || strcmp(s, "false") == 0;
}

void run(char *command, int row_index){
    sets_root[row_index] = NULL;
    seances_root[row_index] = NULL;
    if(command[strlen(command)-1] == '\n'){
        command[strlen(command)-1] = 0;
    }
    int *length = malloc(sizeof(int)); //malloc выделяет *length столько памяти из оперативки, сколько нужно int, команда (sizeof(int))
    char **result = str_split(command, ' ', length); //делим строку на аргументы, т.к. они разделены пробелом
    
    //если первый аргумент строки = "U", "S", "R", то выводим это строку
    if(strcmp(result[0], UNIVERZUM) == 0 || strcmp(result[0], MNOZINY) == 0 || strcmp(result[0], RELACE) == 0){
        printf("%s\n", command);
    }
    assert(*length > 0); //assert.h ловит некоторые ошибки

    //если это идет первая строка в файле
    if(row_index == 1){
        assert(strcmp(result[0], UNIVERZUM) == 0);
        sets_length[1] = 0;
        sets_root[1] = create(sets_root[1], "");

        //тут проверяем аргументы на соответствие заданию
        for(int i = 1; i < *length; i++){
            if(!check_word(result[i])){
                fprintf(stderr, "Neplatny znak\n");
                exit(EXIT_FAILURE);
            }
            if(strlen(result[i]) > MAX_LENGTH){
                fprintf(stderr, "MAX_LENGTH chyba!\n");
                exit(EXIT_FAILURE);
            }
            if(is_command(result[i])){
                fprintf(stderr, "Toto neni prikaz!\n");
                exit(EXIT_FAILURE);
            }
            if(is_true_false(result[i])){
                fprintf(stderr, "Nemuze byt true/false!\n");
                exit(EXIT_FAILURE);
            }
            if(search(sets_root[1], result[i]) != NULL){
                fprintf(stderr, "Opakujici se slovo!\n");
                exit(EXIT_FAILURE);
            }
            add(sets_root[1], result[i]);
            sets_length[1]++;
        }
    } else {
        //если это множества S
        if(strcmp(result[0], MNOZINY) == 0){
            sets_length[row_index] = 0;
            sets_root[row_index] = create(sets_root[row_index], "");
            for(int j = 1; j < *length; j++){
                assert(search(sets_root[1], result[j]) != NULL);
                if(search(sets_root[row_index], result[j]) != NULL){
                    fprintf(stderr, "Opakujici se slovo!\n");
                    exit(EXIT_FAILURE);
                }
                add(sets_root[row_index], result[j]);
                sets_length[row_index]++;
            }
        } 
        //если это отношения R
        else if(strcmp(result[0], RELACE) == 0){
            seances_length[row_index] = 0;
            seances_root[row_index] = create_pair(seances_root[row_index], "", "");
            for(int i = 1; i < *length; i += 2){
                if(i+1 >= *length){
                    fprintf(stderr, "Nespravny format");
                    exit(EXIT_FAILURE);
                }
                assert(result[i][0] == '(');
                for(size_t j = 0; j < strlen(result[i])-1; j++){
                    result[i][j] = result[i][j+1];
                }
                result[i][strlen(result[i])-1] = 0;
                assert(result[i+1][strlen(result[i+1])-1] == ')');
                result[i+1][strlen(result[i+1])-1] = 0;
                assert(search(sets_root[1], result[i]) != NULL);
                assert(search(sets_root[1], result[i+1]) != NULL);
                assert(search_pair(seances_root[row_index], result[i], result[i+1]) == NULL);
                add_pair(seances_root[row_index], result[i], result[i+1]);
                seances_length[row_index]++;
            }
        } 
        //если это команда C
        else if(strcmp(result[0], PRIKAZ) == 0){

            //если аргументов = 1, то ошибка
            if(*length == 1){
                fprintf(stderr, "Neplatny prikaz!");
                exit(EXIT_FAILURE);
            }

            char *command = result[1];

            if(strcmp(command, EMPTY) == 0){
                if(*length != 3){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int set_index = to_integer(result[2]);
                empty_prikaz_handler(set_index);
            } else if (strcmp(command, CARD) == 0){
                if(*length != 3){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int set_index = to_integer(result[2]);
                card_prikaz_handler(set_index);
            } else if(strcmp(command, COMPLEMENT) == 0){
                if(*length != 3){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int set_index = to_integer(result[2]);
                complement_prikaz_handler(set_index);
            } else if(strcmp(command, UNION) == 0){
                if(*length != 4){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int first_set_index = to_integer(result[2]);
                int second_set_index = to_integer(result[3]);
                union_prikaz_handler(first_set_index, second_set_index);
            } else if(strcmp(command, INTERSECT) == 0){
                if(*length != 4){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int first_set_index = to_integer(result[2]);
                int second_set_index = to_integer(result[3]);
                intersect_prikaz_handler(first_set_index, second_set_index);
            } else if(strcmp(command, MINUS) == 0){
                if(*length != 4){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int first_set_index = to_integer(result[2]);
                int second_set_index = to_integer(result[3]);
                minus_prikaz_handler(first_set_index, second_set_index);
            } else if(strcmp(command, SUBSETEQ) == 0){
                if(*length != 4){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int first_set_index = to_integer(result[2]);
                int second_set_index = to_integer(result[3]);
                subseteq_prikaz_handler(first_set_index, second_set_index);
            } else if(strcmp(command, SUBSET) == 0){
                if(*length != 4){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int first_set_index = to_integer(result[2]);
                int second_set_index = to_integer(result[3]);
                subset_prikaz_handler(first_set_index, second_set_index);
            } else if(strcmp(command, EQUALS) == 0){
                if(*length != 4){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int first_set_index = to_integer(result[2]);
                int second_set_index = to_integer(result[3]);
                equals_prikaz_handler(first_set_index, second_set_index);
            } else if(strcmp(command, REFLEXIVE) == 0){
                if(*length != 3){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int seance_index = to_integer(result[2]);
                reflexive_command_handler(seance_index);
            } else if(strcmp(command, SYMMETRIC) == 0){
                if(*length != 3){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int seance_index = to_integer(result[2]);
                symmetric_command_handler(seance_index);
            } else if(strcmp(command, ANTISYMMETRIC) == 0){
                if(*length != 3){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int seance_index = to_integer(result[2]);
                antisymmetric_command_handler(seance_index);
            } else if(strcmp(command, TRANSITIVE) == 0){
                if(*length != 3){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int seance_index = to_integer(result[2]);
                transitive_command_handler(seance_index);
            } else if(strcmp(command, FUNCTION) == 0){
                if(*length != 3){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int seance_index = to_integer(result[2]);
                function_command_handler(seance_index);
            } else if(strcmp(command, DOMAIN) == 0){
                if(*length != 3){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int seance_index = to_integer(result[2]);
                domain_command_handler(seance_index);
            } else if(strcmp(command, CODOMAIN) == 0){
                if(*length != 3){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int seance_index = to_integer(result[2]);
                codomain_command_handler(seance_index);
            } else if(strcmp(command, INJECTIVE) == 0){
                if(*length != 5){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int seance_index = to_integer(result[2]);
                int first_set_index = to_integer(result[3]);
                int second_set_index = to_integer(result[4]);
                injective_command_handler(seance_index, first_set_index, second_set_index);
            } else if(strcmp(command, SURJECTIVE) == 0){
                if(*length != 5){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int seance_index = to_integer(result[2]);
                int first_set_index = to_integer(result[3]);
                int second_set_index = to_integer(result[4]);
                surjective_command_handler(seance_index, first_set_index, second_set_index);
            } else if(strcmp(command, BIJECTIVE) == 0){
                if(*length != 5){
                    fprintf(stderr, "Nespravny pocet argumentu!\n");
                    exit(EXIT_FAILURE);
                }
                int seance_index = to_integer(result[2]);
                int first_set_index = to_integer(result[3]);
                int second_set_index = to_integer(result[4]);
                bijective_command_handler(seance_index, first_set_index, second_set_index);
            } else {
                fprintf(stderr, "Neplatny prikaz!\n");
                exit(EXIT_FAILURE);
            }
        } 
        
        //в остальных случаях - ошибка
        else {
            fprintf(stderr, "Neplatny znak!\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argv, char ** args){
    assert(argv == 2);
    FILE *fp; //создаем переменную файла

    //проверяем идет ли вторым аргументов ссылка на файл и если нет, то выводим ошибку и завершаем программу
    if ((fp = fopen(args[1], "r")) == NULL){
        fprintf(stderr, "Soubor nenalezen!\n");
        exit(EXIT_FAILURE);
    }


    char command[LIMIT]; //набор символов, максимум LIMIT (1000) строк, читает строки из файла
    int cnt_c = 0; //счетчик строк с символом С (команда)
    int cnt_s_r = 0; //счетчик строк с символами S или R

    //fgets считывает по одной строке, пока не начнется новая строка (\n)
    while(fgets(command, LIMIT, fp)){

        //если длина строки = 0 или если первый знак не с большой буквы то возвращает ошибку
        if(strlen(command) == 0 || !is_alpha(command[0])){
            fprintf(stderr, "ERROR!\n");
            exit(EXIT_FAILURE);
        }

        //если начало строки это S или R, то прибавляем к счетчику cnt_s_r +1
        if(command[0] == 'S' || command[0] == 'R'){
            cnt_s_r++;

            //но если у нас уже была команда, а потом идет S или R, то это ошибка
            if(cnt_c > 0){
                fprintf(stderr, "ERROR!\n");
                exit(EXIT_FAILURE);
            }
        }

        //если команда C, то прибавляем +1 к счетчику команд
        if(command[0] == 'C'){
            cnt_c++;
        }
    }

    //если нет команды C или R/S, то ошибка
    if(cnt_s_r == 0 || cnt_c == 0){
        fprintf(stderr, "Nespravny format!\n");
        exit(EXIT_FAILURE);
    }

    //закрываем файл, чтобы не было проблем с памятью
    fclose(fp);
    
    // открываем файл
    fp = fopen(args[1], "r");

    //счетчик рядов начинаем с 1 из-за особенностей fgets
    int row_index = 1;
    
    while(fgets(command, LIMIT, fp)){
        assert(row_index <= LIMIT);
        //запускаем по одной строке функцию run (строка, номер ряда)
        run(command, row_index);

        //увеличиваем число ряда
        row_index++;
    }
    fclose(fp);

    //если функция не void, то должна сделать return. Т.к. это int(числа), возвращает 0
    return 0;
}