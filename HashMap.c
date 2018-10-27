//
// Created by Jan on 24. 10. 2018.
//
#include <stdio.h>
#include <windef.h>
#include <malloc.h>
#include "HashMap.h"

void insert(HashMap *p_map, char *p_word) {
    int key;
    Node *p_listOnPos,*p_newNode, *p_tmp;

    key = hashFunction(p_map->size,p_word);
    p_listOnPos = p_map->list[key];
    p_tmp = p_listOnPos;

    while(p_tmp != NULL){
       // printf("           Slovo %s , pocet %d, size slova %zu \n",p_tmp->p_word,p_tmp->count,p_tmp->wordSize);
        if(!strncmp(p_word,p_tmp->p_word,(p_tmp->wordSize-1))){
            p_tmp->count++;
            return;
        }
        p_tmp=p_tmp->p_next;
    }
    p_newNode = (Node *)malloc(sizeof(Node));
    if(p_newNode == NULL){
        freeMap(p_map);
        return;
    }
    p_newNode->count = 1;
    p_newNode->p_word =(char *) malloc(strlen(p_word)+1);
    if(p_newNode->p_word == NULL){
        freeMap(p_map);
        return;
    }
    p_newNode->wordSize = sizeof(char) * (strlen(p_word)+1);
    strncpy(p_newNode->p_word,p_word,p_newNode->wordSize);
    p_newNode->p_next = p_listOnPos;
    p_map->list[key] = p_newNode;
    printf("Pridani prvku probehlo v poradku\n");
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
    printf("Vytvoreni mapy probehlo v poradku\n");
    return p_map;
}

int hashFunction(int mapSize, char *p_word) {
    return 0;
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
            p_tmp = (Node *) p_tmp->p_next;
        }
    }

}

/**
 * Zeptat se na pointer v p_tmp -> next ??????????????????????????????????????????????
 * @param p_map
 */

void freeMap(HashMap *p_map) {
    int i;
    Node *p_node, *p_tmp;
    for(i =0; i<p_map->size;i++) {
        p_tmp = p_map->list[i];
        while (p_tmp) {
            p_node = p_tmp;
            p_tmp = (Node *) p_tmp->p_next;
            if(p_node->p_word != NULL) {
                free(p_node->p_word);
            }
            free(p_node);
        }
    }

    free(p_map);
}
