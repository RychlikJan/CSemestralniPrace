#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HashMap.h"

#define MAP_SIZE 1000
#define STEM_SIZE 3

int readFromFile(HashMap *p_map, char *file, int stemSize);

int modifyWord(unsigned char *p_word, int stemSize, HashMap *p_map);

int findStemsFromText(int countStems);

int readSteams(HashMap *p_map, int countStems);


int modifyWord(unsigned char *p_word, int stemSize, HashMap *p_map) {

    unsigned char c;
    int i, j, len, falutChar, stat;
    len = strlen((char *) p_word);
    unsigned char modifedWord[len];
    j = 0;
    falutChar = 0;
    stat = 0;
    // printf("tested word = %s", p_word);
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
                return -1;
            }
        }
    }
    if (j >= stemSize) {
        modifedWord[j] = '\0';
        insert(p_map, modifedWord);
    }
    return 0;
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
        printf("%s,:%s", newString[0], newString[1]);
    }
    fclose(fp);
    return 0;

}

int findStemsFromText(int countStems) {
    HashMap *p_map = createHashMap(9973);
    if (p_map == NULL) {
        printf("Problem with map create");
        return -1;
    }

    readSteams(p_map, countStems);
    printf("\n Printf map");
    showMap(p_map);

}

int main() {
    int countStems = 0;
    int status = 0;
    printf("Hello, World!\n");

    HashMap *p_map = createHashMap(9973);
    if (p_map == NULL) {
        printf("Problem with map create");
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

    HashMap *p_stems = createHashMap(1000);
    if (p_stems == NULL) {
        printf("Problem with map create");
        freeMap(p_map);
        return -1;
    }
    findStems(p_map, p_stems, STEM_SIZE);

    //showMap(p_stems);
    printf("Pred save");
    saveToFile(p_stems);
    printf("po save\n");
    freeMap(p_map);
    freeMap(p_stems);

    findStemsFromText(countStems);

    return 0;
}