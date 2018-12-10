#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include "HashMap.h"

#define MAP_SIZE 1000
#define STEM_SIZE 3
#define STEM_COUNT 10

int readFromFile(HashMap *p_map, char *file, int stemSize);

int modifyWord(unsigned char *p_word, int stemSize, HashMap *p_map);

int findStemsFromText(char *testedStr,int msl);

int makeStems();

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
    p_aktWord = p_mapTestedStrig->list[0];
    while (p_aktWord != NULL) {
        size_t sizeFinalSteem = 0;
        finalStem[0] = '0';
        finalStem[1] = '\0';
        p_aktStem = p_map->list[0];
        while (p_aktStem != NULL) {
            if (strstr((char *) p_aktWord->p_word, (char *) p_aktStem->p_word) != NULL) {
                if (p_aktStem->wordSize >= sizeFinalSteem) {
                    sizeFinalSteem = p_aktStem->wordSize;
                    memcpy(finalStem, p_aktStem->p_word, sizeFinalSteem);
                    finalStem[sizeFinalSteem] = '\0';
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

    fp = fopen("C:\\Users\\Jan\\CLionProjects\\StemmerSemestralniPrace\\stems.dat", "r");
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
            insert(p_map, newString[0]);
        }
    }
    fclose(fp);
    return 0;

}

int findStemsFromText(char *testedStr,int msl) {
    int countStems = msl;
    //unsigned char testedStr[100] = "Šel pes do lesa a potkal dlažební kostku sel pes do lesa a potkal dlazebni kostku";

    HashMap *p_map = createHashMap(1); // -> Linked list
    HashMap *p_mapTestedStrig = createHashMap(1);
    if (p_map == NULL) {
        printf("Problem with map create\n");
        return -1;
    }

    readSteams(p_map, countStems);
    modifyWord((unsigned char *)testedStr, 1, p_mapTestedStrig);
    stemsFinding(p_map, p_mapTestedStrig);
    freeMap(p_map);
    freeMap(p_mapTestedStrig);
}

int makeStems() {
    int status = 0;
    HashMap *p_map = createHashMap(MAP_SIZE);
    if (p_map == NULL) {
        printf("Problem with map create\n");
        return -1;
    }
    //dasenka_cili_zivot_stenete
    status = readFromFile(p_map,
                          "C:\\Users\\Jan\\CLionProjects\\StemmerSemestralniPrace\\dasenka_cili_zivot_stenete.txt",
                          STEM_SIZE);
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
    findStems(p_map, p_stems, STEM_SIZE);
    saveToFile(p_stems);
    freeMap(p_map);
    freeMap(p_stems);
}


int main(int argc, char *argv[]) {
    int msl, msf, len,argLen,i;
    char number[100];
    argLen =4;

    if (argc == 2 || argc == 3) {

        FILE *f = fopen("C:\\Users\\Jan\\CLionProjects\\StemmerSemestralniPrace\\stems.dat", "r");
        if (f != NULL) { // file exist, find stems in text
            printf("Find stems in text\n");
            if (argc == 2) {
                msl = STEM_COUNT;
            } else {
                len = strlen(argv[2]);
                i = 0;
                while(i < len){
                    number[i] = argv[2][i+argLen-1];
                    i++;
                }
                number[i] = '\0';
                msl = atoi(number);
            }

            findStemsFromText(argv[1],msl);
        } else {
            fclose(f);
            printf("Make new stems\n ");
            //makeStems();
        }
    } else {
        printf("Wrong number of parameters\n Program have to run with:\n programName fileWithLerning (sizeOfStem) or \n  programName testingString (sizeOfStem)");
    }

    return 0;
}