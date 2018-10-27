//
// Created by Jan on 24. 10. 2018.
//

//http://www.kaushikbaruah.com/posts/data-structure-in-c-hashmap/
//https://github.com/petewarden/c_hashmap/blob/master/hashmap.c
//https://github.com/rxi/map/blob/master/src/map.c


//Pointery
//https://www.itnetwork.cz/cplusplus/cecko/dynamicka-prace-s-pameti/c-tutorial-dynamicke-textove-retezce-a-struktury

#ifndef STEMMERSEMESTRALNIPRACE_HASHMAP_H
#define STEMMERSEMESTRALNIPRACE_HASHMAP_H

/**
 * Uzel spojoveho seznamu
 */
struct node{
    char *p_word;
    size_t wordSize;
    int count;
    struct node *p_next;
};

/**
 * Struktur hashMapy
 */
struct hashMap{
    int size;
    struct node **list; // *list = pole ukazatelu plne ukazatelu na Node
};

/**
 * Vytvori HashMap
 * @param size velikost mapy
 * @return pointer na zacatek mapy
 */
struct hashMap *createHashMap(int size);

/**
 * Vlozi prvek do mapy
 * @param map ukazatel na mapu
 * @param word
 */
void insert(struct hashMap *p_map, char *p_word);

/**
 * Vrati hodnotu hashovaci funkce
 * @param mapSize velikost mapy
 * @param word slovo k hash
 * @return index v mape
 */
int hashFunction(int mapSize, char *p_word);

/**
 * Uvolni pamet mapy
 * @param map velikost mapy
 * @return 0/1 uspech/neuspech
 */
void freeMap(struct hashMap *p_map);

/**
 * Pomocna fce k zobrazeni mapy
 * @param map hashMap
 */
void showMap(struct hashMap *p_map);
#endif //STEMMERSEMESTRALNIPRACE_HASHMAP_H
