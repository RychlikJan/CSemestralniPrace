//
// Created by Jan on 24. 10. 2018.
//
#include <stdio.h>
#include <windef.h>
#include <malloc.h>
#include "HashMap.h"

int insert(HashMap *p_map,unsigned char *p_word) {
    int key;
    size_t newWord;
    Node *p_listOnPos,*p_newNode, *p_tmp;

    key = hashFunction(p_map->size,p_word);
    newWord = sizeof(char) * (strlen((char *)p_word)+1);
    p_listOnPos = p_map->list[key];
    p_tmp = p_listOnPos;

    while(p_tmp != NULL){
        if(!memcmp(p_word,p_tmp->p_word,(max(newWord,p_tmp->wordSize)-1))){
            p_tmp->count++;
            return 0;
        }
        p_tmp=p_tmp->p_next;
    }

    p_newNode = (Node *)malloc(sizeof(Node));
    if(p_newNode == NULL){
        freeMap(p_map);
        printf("Error with malloc memory for new Node,\n");
        return -1;
    }
    p_newNode->count = 1;
    p_newNode->p_word =(unsigned char *) malloc(strlen((char *)p_word)+1);
    if(p_newNode->p_word == NULL){
        freeMap(p_map);
        printf("Error with malloc memory for new char in node,\n");
        return -1;
    }
    p_newNode->wordSize = newWord;
    memcpy(p_newNode->p_word,p_word,p_newNode->wordSize);
    p_newNode->p_next = p_listOnPos;
    p_map->list[key] = p_newNode;
    p_map->count++;
    return 0;
}

HashMap *createHashMap(int size) {
    int i;
    HashMap *p_map = (HashMap *)malloc(sizeof(HashMap));

    if(p_map == NULL){
        return NULL;
    }
    p_map->size = 0;
    p_map->list = (Node**)malloc(sizeof(Node) * size);
    if(p_map->list == NULL){
        freeMap(p_map);
        return NULL;
    }
    p_map->size = size;
    for(i = 0; i < size;i++){
        p_map->list[i] = NULL;
    }
    p_map->count=0;
    return p_map;
}

int hashFunction(int mapSize,unsigned char *p_word) {
    int hash, coef,i, len;
    len = strlen((char *)p_word);
    coef = 19;
    hash = 0;

    for(i = 0; i < len; i++){
        hash = ((coef * hash)+(int)p_word[i])%mapSize;
    }
    hash = abs(hash);
    return hash;
}

void showMap(HashMap *p_map) {
    int i;
    Node *p_tmp;

    printf("Mapa \n");
    for(i =0; i<p_map->size;i++) {
        printf("Line %d: \n", i);
        p_tmp = p_map->list[i];
        while (p_tmp) {
            if(p_tmp->p_word != NULL) {
                printf("           Slovo %s , pocet %d, size slova %d \n",p_tmp->p_word,p_tmp->count,p_tmp->wordSize);
            }
            p_tmp = p_tmp->p_next;
        }
    }
    printf("Map contains %d unique chars", p_map->count);
}

void freeMap(HashMap *p_map) {
    int i;
    Node *p_node, *p_tmp;

    for(i =0; i<p_map->size;i++) {
        p_tmp = p_map->list[i];
        while (p_tmp) {
            p_node = p_tmp;
            p_tmp = p_tmp->p_next;
            if(p_node->p_word != NULL) {
                free(p_node->p_word);
            }
            free(p_node);
        }
    }
    if(p_map->list != NULL) {
        free(p_map->list); // ?///////////////??????????????????????????????????????????????????
    }
    free(p_map);
}
int compareNodes(const void* va, const void* vb){
    int i;
    int c1, c2, eq;
    size_t size;
     Node *a = *(Node **)va;
     Node *b = *(Node **)vb;

    if(a->wordSize < b->wordSize){
        size = a->wordSize;
    } else{
        size = b->wordSize;
    }
    for(i = 0;i < size;i++) {
        c1 = a->p_word[i];
        c2 = b->p_word[i];
        eq = c1 -c2;
        if(eq!= 0){
            return eq;
        }
    }
    return 0;
}

void saveToFile(HashMap *p_map){
    Node *list[p_map->count];
    int i,j;
    Node *p_tmp;
    j=0;

    for(i =0; i<p_map->size;i++) {
        p_tmp = p_map->list[i];
        while (p_tmp) {
            list[j] = p_tmp;
            j++;
            p_tmp = p_tmp->p_next;
        }
    }
    /*for(i=0;i<p_map->count;i++){
        printf("%s ,", list[i]->p_word);
    }*/
    qsort(list,(size_t)p_map->count, sizeof(Node*),&compareNodes);
    FILE *f = fopen("C:\\Users\\Jan\\CLionProjects\\StemmerSemestralniPrace\\stems.dat", "w");
    if(f == NULL){
        printf("File writing filed\n");
        exit(1);
    }
    for(i=0;i<p_map->count;i++){
        fprintf(f,"%s %d\n", list[i]->p_word, list[i]->count);
    }
    fclose(f);
}
int findStems(HashMap *p_map, HashMap *p_stems, int sizeStem){
    Node *list[p_map->count];
    int i,j;
    Node *p_tmp;
    j=0;

    for(i =0; i<p_map->size;i++) {
        p_tmp = p_map->list[i];
        while (p_tmp) {
            list[j] = p_tmp;
            j++;
            p_tmp = p_tmp->p_next;
        }
    }
    for(i = 0; i < p_map->count;i++){
        for(j =i+1; j < p_map->count;j++){
            LCS(p_stems,list[i]->p_word,list[j]->p_word,sizeStem);
        }
    }
}

int LCS(HashMap *p_stems, unsigned char *p_word1, unsigned char *p_word2, int sizeStem){
    int i,j;
    int m = strlen((char *)p_word1);
    int n = strlen((char *)p_word2);

    if(m==n) {
        if((memcmp(p_word1, p_word2, (size_t) m))== 0){
            return 0;
        }

    }
    int LCSuff[m + 1][n + 1];
    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                LCSuff[i][j] = 0;
            }
            else if (p_word1[i - 1] == p_word2[j - 1]) {
                LCSuff[i][j] = LCSuff[i - 1][j - 1] + 1;
            }
            else {
                LCSuff[i][j] = 0;
            }
        }
    }

    unsigned char *stem;
    int o,len;
   for(i = m; 0 <= i; i--){
    for(j = n; 0<= j; j--){
     if(LCSuff[i][j] >= sizeStem){
        len = LCSuff[i][j];
         stem = (unsigned char*)malloc((len + 1) * sizeof(char));
         memcpy(stem, p_word1 + (i-len), (size_t) len);
         stem[len]='\0';
         insert(p_stems,stem);
         o=0;

         while(o < len){
             LCSuff[i-o][j-o] =0;
             o++;
         }
     }
    }
   }
    return 0;
}