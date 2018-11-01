#include <stdio.h>

#include "HashMap.h"

#define MAP_SIZE 1000
int readFromFile(HashMap *p_map, char *file);


int readFromFile(HashMap *p_map, char *file){
    int insertStat;
    FILE *fp;
    fp = fopen(file, "r");
    char buffer[1024];

    if(fp == NULL){
        printf("Error with opening file\n");
        return -1;
    }
    while (fscanf(fp, " %1023s", buffer) != EOF) {
        printf("%s ",buffer);
        insertStat = insert(p_map,buffer);
        if(insertStat != 0){
            printf("Error with add word to file\n");
            return -1;
        }
        //printf("Pridano %s\n",buffer);
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

    //status = readFromFile(p_map,"C:\\Users\\Jan\\CLionProjects\\StemmerSemestralniPrace\\test.txt");
    if(status != 0){
        freeMap(p_map);
        return -1;
    }

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