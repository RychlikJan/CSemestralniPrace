//
// Created by Jan on 24. 10. 2018.
//
#include <stdio.h>
#include <windef.h>
#include <malloc.h>
#include "HashMap.h"

void wordByChar(unsigned char *word){
    printf(" start\n");
    unsigned char c;
    int i = 0;
    while(word[i] != '\0'){
        c = word[i];
        printf("char %d + ", c);
        i++;
    }
    printf(" konec\n");
    //printf("\n %s \n", word);

}

int insert(HashMap *p_map,unsigned char *p_word) {
    int key;
    size_t newWord;
    Node *p_listOnPos,*p_newNode, *p_tmp;

    key = hashFunction(p_map->size,p_word);
    newWord = sizeof(char) * (strlen((char *)p_word)+1);
    p_listOnPos = p_map->list[key];
    p_tmp = p_listOnPos;
    wordByChar(p_word);
    while(p_tmp != NULL){
        if(!memcmp(p_word,p_tmp->p_word,(max(newWord,p_tmp->wordSize)-1))){
            p_tmp->count++;
            return 0;
            // slovo uz tam existuje
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
    return p_map;
}

int hashFunction(int mapSize,unsigned char *p_word) {
    // Size 1000, max h = 999, 999*32 = 31968 + 255 = 32223; 32 223 < 32 767
    int hash, coef,i, len;
    len = strlen((char *)p_word);
    coef = 32;
    hash = 0;
    for(i = 0; i < len; i++){
        hash = ((coef * hash) + (int)p_word[i])%mapSize;
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
