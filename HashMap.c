//
// Created by Jan on 24. 10. 2018.
//
#include <stdio.h>
#include <windef.h>
#include <malloc.h>
#include "HashMap.h"

void wordByChar(unsigned char *word){
    //printf(" start\n");
    unsigned char c;
    int i = 0;
    while(word[i] != '\0'){
        c = word[i];
        //printf("char %d + ", c);
        i++;
    }
    //printf(" konec\n");
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
    //wordByChar(p_word);
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
    // Size 1000, max h = 999, 999*32 = 31968 + 255 = 32223; 32 223 < 32 767
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
            //printf("\n vysl = %d \n", eq);
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
    printf("\n\n\n\nMapa \n");
    for(i =0; i<p_map->size;i++) {
        p_tmp = p_map->list[i];
        while (p_tmp) {

            //memcpy(list[j],p_tmp, sizeof(Node));
            list[j] = p_tmp;
            j++;
            p_tmp = p_tmp->p_next;
        }
    }

    for(i=0;i<p_map->count;i++){
        printf("%s ,", list[i]->p_word);
    }
    qsort(list,(size_t)p_map->count, sizeof(Node*),&compareNodes);

    printf("\n\n\n");
    for(i=0;i<p_map->count;i++){
        printf("%s ,", list[i]->p_word);
    }
    printf("Map contains %d unique chars", p_map->count);


    FILE *f = fopen("C:\\Users\\Jan\\CLionProjects\\StemmerSemestralniPrace\\stems.dat", "w");
    if(f == NULL){
        printf("File writing filed");
        exit(1);
    }
    for(i=0;i<p_map->count;i++){
        fprintf(f,"%s %d\n", list[i]->p_word, list[i]->count);
    }
    fclose(f);

    //printf("Saved");
}
int findStems(HashMap *p_map, HashMap *p_stems, int sizeStem){
    int countCompare =0;
  /*  int i,j;
    Node *p_tmp1, *p_tmp2;
    for(i =0; i<p_map->size;i++) {
        p_tmp1 = p_map->list[i];
        while (p_tmp1) {

            for(j =0; j<p_map->size;j++) {
                p_tmp2 = p_map->list[j];
                while (p_tmp2) {
                    countCompare++;
                    LCS(p_stems,p_tmp1->p_word,p_tmp2->p_word,sizeStem);
                    p_tmp2 = p_tmp2->p_next;
                    //printf("\nPocet porovnani je = %d",countCompare);
                }
            }
           // printf("Provede se posledni prikaz?");
            p_tmp1 = p_tmp1->p_next;
            //printf("Ano");
        }
    }*/
    Node *list[p_map->count];
    int i,j;
    Node *p_tmp;
    j=0;
    printf("\n\n\n\nMapa \n");
    for(i =0; i<p_map->size;i++) {
        p_tmp = p_map->list[i];
        while (p_tmp) {

            //memcpy(list[j],p_tmp, sizeof(Node));
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
printf("\nPocet porovnani je = %d",countCompare);


}


// strlen vraci vcetne
int LCS(HashMap *p_stems, unsigned char *p_word1, unsigned char *p_word2, int sizeStem){
   // printf("\n Vstup do LCS");
    int i,j;
    int m = strlen((char *)p_word1);
    int n = strlen((char *)p_word2);
   // printf("\n m = %d a N = %d, (pozn size stem = %d) ",m,n,sizeStem);
    if(m==n) {
        //printf("Slova uvnitr ifu Slovo 1 = %s, Slovo 2 = %s",p_word1,p_word2);
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
 //   printf("\n matrix created \n");
/*    printf("\nSlova uvnitr Slovo 1 = %s, Slovo 2 = %s \n",p_word1,p_word2);
    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++){
            printf("%d ,",LCSuff[i][j]);
        }
    printf("\n");
    }*/




    unsigned char *stem;
    int o,len;
   for(i = m; 0 <= i; i--){
    for(j = n; 0<= j; j--){
       // printf("projizdi se to tady ? ");
     if(LCSuff[i][j] >= sizeStem){
        len = LCSuff[i][j];
     //    printf("\n Len =  %d, kde je problem ?", len);
         stem = (unsigned char*)malloc((len + 1) * sizeof(char));
      //   printf("\n Mozna pred memcpy");
         memcpy(stem, p_word1 + (i-len), (size_t) len);
        // printf("\n Nebo je to insert");
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
  //  printf("\n----------------------------------------");
    return 0;
}