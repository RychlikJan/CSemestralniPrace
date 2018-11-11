#include <stdio.h>
#include <string.h>
#include "HashMap.h"

#define MAP_SIZE 1000
int readFromFile(HashMap *p_map, char *file);
unsigned char *modifyWord(unsigned char *p_word);


unsigned char *modifyWord(unsigned char *p_word){
    //printf("--------------------------\n");
   // printf("%s \n",p_word);

    unsigned char c, *p_modifedWord;
    int i, len, falutChar;
    len = strlen((char *)p_word);
    unsigned char modifedWord[len];

    falutChar = 0;
    for(i = 0;i < len;i++){
        c = p_word[i];
        if((64 < c && c < 91)|| (96 < c && c < 123) || (c == 138) || (c == 141) || (c == 142) || (c == 154) || (c == 157) || (c == 158) || (191 < c && c < 254)) {
           modifedWord[i-falutChar] = c;

        } else{
            falutChar++;
        }
        //printf("len = %d, i = %d, falutChar = %d, char = %c \n", len,i,falutChar,c);
    }
    modifedWord[len - falutChar] = '\0';
   // printf("%s \n",modifedWord);
    p_modifedWord = &modifedWord[0];
    printf(" v modify :%s \n",p_modifedWord);
    return p_modifedWord;
}

int readFromFile(HashMap *p_map, char *file){
    int insertStat;
    FILE *fp;
    fp = fopen(file, "r");
    unsigned char buffer[1024];

    if(fp == NULL){
        printf("Error with opening file\n");
        return -1;
    }
    while (fscanf(fp, " %1023s", buffer) != EOF) {
       // printf("\n %s ",buffer);
        //modifyWord(buffer);
        printf("tady :::%s\n",modifyWord(buffer));
        insertStat = insert(p_map,buffer);
        if(insertStat != 0){
            printf("Error with add word to file\n");
            return -1;
        }
        //printf("Pridano %s\n",buffer);
        printf("konec cyklu \n\n\n\n\n");
    }

    if(fclose(fp) == EOF){
        printf("Error with closeing file\n");
        return -1;
    }
    return 0;
}

int main() {
    int status = 0;
    printf("Hello, World!\n");

    HashMap *p_map = createHashMap(10);
    if(p_map == NULL){
        printf("Problem with map create");
        return -1;
    }

    status = readFromFile(p_map,"C:\\Users\\Jan\\CLionProjects\\StemmerSemestralniPrace\\test.txt");
    if(status != 0){
        freeMap(p_map);
        return -1;
    }
    // predelat to do unsigned, a pouzi metodu memcmp();
    insert(p_map,"pes");
    insert(p_map,"kocka");
    insert(p_map,"mys");
    insert(p_map,"pes");
    insert(p_map,"pe");
    insert(p_map,"přespřílišžluťoučkýkůňúpěldábelskéódy");
    //insert(p_map,"Š");
    insert(p_map,"PŘESPŘÍLIŠŽLUŤOUČKÝKŮŇÚPĚLĎÁELSKÉÓDY");

    showMap(p_map);
    freeMap(p_map);

    return 0;
}