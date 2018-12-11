#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HashMap.h"

#define MAP_SIZE 1000
#define STEM_LENGTH 3
#define STEM_FREQ 10

const char *FILE_NAME = "stems.dat";
//const char *FILE_NAME = "C:\\Users\\Jan\\CLionProjects\\StemmerSemestralniPrace\\stems.dat";

int readFromFile(HashMap *p_map, char *file, int stemSize);

int modifyWord(unsigned char *p_word, int stemSize, HashMap *p_map);

int findStemsFromText(char *testedStr, int msl);

int makeStems(char *file, int msl);

int readSteams(HashMap *p_map, int countStems);

int stemsFinding(HashMap *p_map, HashMap *p_mapTestedStrig);


int modifyWord(unsigned char *p_word, int stemSize, HashMap *p_map) {
    unsigned char c;
    int i, j, len, falutChar, stat;
    len = strlen((char *) p_word);
    unsigned char modifedWord[len];
    j = 0;
    falutChar = 0;
    stat = 0;

    for (i = 0; i < len; i++) {
        c = p_word[i];
        // char C e {<A,Z>,<a,z>, ==Š, ==Ť,==Ž,==š,==ť,==ž, <Velke interp.><male interp>}
        if ((64 < c && c < 91) || (96 < c && c < 123) || (c == 138) || (c == 141) || (c == 142) || (c == 154) ||
            (c == 157) || (c == 158) || ((191 < c && c < 254) && c != 215 && c != 247)) {
            if ((64 < c && c < 91) || (191 < c && c <= 223)) {
                c += 32;
            } else if (137 < c && c < 143) {
                c += 16;
            }
            modifedWord[j] = c;
            j++;
        } else {
            if (j >= stemSize) {
                modifedWord[j] = '\0';
                stat = insert(p_map, modifedWord);
            }
            j = 0;
            falutChar++;
            if (stat != 0) {
                freeMap(p_map);
                return -1;
            }
        }
    }
    if (j >= stemSize) {
        modifedWord[j] = '\0';
        stat = insert(p_map, modifedWord);
    }
    if (stat != 0) {
        freeMap(p_map);
        return -1;
    }
    return 0;
}

int stemsFinding(HashMap *p_map, HashMap *p_mapTestedStrig) {

    Node *p_aktWord, *p_aktStem;
    unsigned char finalStem[100];
    finalStem[0] = '0';
    finalStem[1] = '\0';
    p_mapTestedStrig->list[0] = reverse(p_mapTestedStrig->list[0]);
    p_aktWord = p_mapTestedStrig->list[0];
    while (p_aktWord != NULL) {
        size_t sizeFinalSteem = 0;
        finalStem[0] = '0';
        finalStem[1] = '\0';
        p_aktStem = p_map->list[0];
        while (p_aktStem != NULL) {
            //printf("%s -> %s\n", p_aktWord->p_word,p_aktStem->p_word);
            if (strstr((char *) p_aktWord->p_word, (char *) p_aktStem->p_word) != NULL) {
                if (p_aktStem->wordSize >= sizeFinalSteem) {
                    sizeFinalSteem = p_aktStem->wordSize;
                    memcpy(finalStem, p_aktStem->p_word, sizeFinalSteem);
                    finalStem[sizeFinalSteem+1] = '\0';
                   // printf("%s -> %s\n", p_aktWord->p_word, finalStem);
                }
            }
            p_aktStem = p_aktStem->p_next;
        }
        printf("%s -> %s\n", p_aktWord->p_word, finalStem);
        p_aktWord = p_aktWord->p_next;
    }

}

int readFromFile(HashMap *p_map, char *file, int stemSize) {
    int insertStat;
    FILE *fp;
    fp = fopen(file, "r");
    unsigned char buffer[1024];

    if (fp == NULL) {
        printf("Error with opening file\n");
        return -1;
    }
    while (fscanf(fp, " %1023s", buffer) != EOF) {
        insertStat = modifyWord(buffer, stemSize, p_map);
        if (insertStat != 0) {
            printf("Error with add word to file\n");
            return -1;
        }
    }

    if (fclose(fp) == EOF) {
        printf("Error with closeing file\n");
        return -1;
    }
    return 0;
}

int readSteams(HashMap *p_map, int countStems) {
    FILE *fp;
    char line[100];
    int len = 0;
    int stemC = 0;
    unsigned char c;
    unsigned char newString[2][100];
    int i, j, k;

    fp = fopen(FILE_NAME, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while (fgets(line, sizeof line, fp) != NULL) {
        len = strlen(line);
        j = 0;
        k = 0;
        if (line != "") {
            for (i = 0; i < len; i++) {
                c = (unsigned char) line[i];
                if (c == ' ' || c == '\0') {
                    newString[j][k] = '\0';
                    k = 0;
                    j++;
                } else {
                    newString[j][k] = c;
                    k++;
                }
            }
        }
        stemC = atoi(newString[1]);
        if (stemC >= countStems) {
            //printf("pridano %s ",newString[0]);
            insert(p_map, newString[0]);
        }
    }
    fclose(fp);
    return 0;

}

int findStemsFromText(char *testedStr, int msl) {
    int countStems = msl;
    //unsigned char testedStr[100] = "Šel pes do lesa a potkal dlažební kostku sel pes do lesa a potkal dlazebni kostku";

    HashMap *p_map = createHashMap(1); // -> Linked list
    HashMap *p_mapTestedStrig = createHashMap(1);
    if (p_map == NULL) {
        printf("Problem with map create\n");
        return -1;
    }

    readSteams(p_map, countStems);
    modifyWord((unsigned char *) testedStr, 1, p_mapTestedStrig);
    stemsFinding(p_map, p_mapTestedStrig);
    freeMap(p_map);
    freeMap(p_mapTestedStrig);
}

int makeStems(char *file, int msl) {
    int status = 0;
    HashMap *p_map = createHashMap(MAP_SIZE);
    if (p_map == NULL) {
        printf("Problem with map create\n");
        return -1;
    }
    //dasenka_cili_zivot_stenete
    status = readFromFile(p_map, file, msl);
    if (status != 0) {
        freeMap(p_map);
        return -1;
    }
    HashMap *p_stems = createHashMap(MAP_SIZE);
    if (p_stems == NULL) {
        printf("Problem with hash map create\n");
        freeMap(p_map);
        return -1;
    }
    findStems(p_map, p_stems, msl);
    saveToFile(p_stems);
    freeMap(p_map);
    freeMap(p_stems);
}


int main(int argc, char *argv[]) {
    int msl, msf, len, argLen, i;
    char number[100], *argMsl, *argMsf, argRead[100];
    char *p_end;
    argMsf = "-msf=";
    argMsl = "-msl=";
    argLen = 6;
    char *err = "Wrong program parameters.\nRun with: <wordSeuence|| fileWithStems> <-msf=stemsFrequency|| -msl=stemLength >\n";
    char *s2 = ".txt";
    char *result = malloc(strlen(argv[1]) + strlen(s2) + 1);
    if (result == NULL) {
        printf("Err with malloc");
        return 1;
    }
    strcpy(result, argv[1]);
    strcat(result, s2);
    FILE *f = fopen(FILE_NAME, "r");
    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(result, "r");
    if (argc == 2) {
        msl = STEM_LENGTH;
        msf = STEM_FREQ;

        if (f1 != NULL || f2 != NULL) {
            if (f1 != NULL) {

                //printf("makeStems(resoult =%s, msl = %d)",argv[1],msl);
                makeStems(argv[1], msl);
            }else{
                //printf("makeStems(resoult =%s, msl = %d)",result,msl);
                makeStems(result, msl);
            }

        } else {
            if (f == NULL) {
                printf("%s", err);
                return 1;
            }
            //printf("findStemsFromText(argv[1] = %s,msf = %d)",argv[1],msf);
            findStemsFromText(argv[1],msf);
        }
    } else if (argc == 3) {
        len = strlen(argv[2]);
        if(len <6){
            printf("%s",err);
            return 1;
        }
        i = 0;
        while (i < len) {
            argRead[i] = argv[2][i];
            number[i] = argv[2][i + argLen - 1];
            i++;
        }
        number[i] = '\0';
        argRead[5] = '\0';
        if (!(strncmp(argRead, argMsl, 5))) {
            msl = strtol(number, &p_end, 10);
            if (f1 != NULL || f2 != NULL) {
                if (f1 != NULL || p_end != NULL) {
                    //printf("makeStems(resoult =%s, msl = %d)", argv[1], msl);
                    makeStems(argv[1], msl);
                } else if (p_end != NULL) {
                    //printf("makeStems(resoult =%s, msl = %d)", result, msl);
                    makeStems(result, msl);
                }
                else{
                    printf("%s\n", err);
                    return 1;
                }
            }
            else{
                printf("%s\n", err);
                return 1;
            }
            } else if (!(strncmp(argRead, argMsf, 5))) {
                msf = strtol(number, &p_end, 10);
                if(p_end != NULL && f != NULL) {
                    //printf("findStemsFromText(argv[1] = %s,msf = %d)", argv[1], msf);
                    findStemsFromText(argv[1],msf);
                } else{
                    printf("%s\n", err);
                    return 1;
                }
            } else {
                printf("%s\n", err);
                return 1;
            }

        } else {
            printf("%s", err);
            return 1;
        }
if (f != NULL){
fclose(f);
}
if (f1 != NULL){
fclose(f1);
}
if (f2 != NULL){
fclose(f2);
}

//fclose(f);
//fclose(f1);
//fclose(f2);
free(result);
//free(p_end);
        return 0;
    }

