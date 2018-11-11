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
    unsigned char *p_word;
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
typedef struct node Node;
typedef struct hashMap HashMap;



/**
 * Vlozi prvek do mapy
 * @param map ukazatel na mapu
 * @param word
 * @return 0 pokud funkce dobehne do konce, jinak -1
 */
int insert(HashMap *p_map,unsigned char *p_word);

/**
 * Vytvori HashMap
 * @param size velikost mapy
 * @return pointer na zacatek mapy
 */
HashMap *createHashMap(int size);

/**
 * Vrati hodnotu hashovaci funkce
 * @param mapSize velikost mapy
 * @param word slovo k hash
 * @return index v mape
 */
int hashFunction(int mapSize,unsigned char *p_word);

/**
 * Uvolni pamet mapy
 * @param map velikost mapy
 * @return 0/1 uspech/neuspech
 */
void freeMap(HashMap *p_map);

/**
 * Pomocna fce k zobrazeni mapy
 * @param map hashMap
 */
void showMap(HashMap *p_map);
#endif //STEMMERSEMESTRALNIPRACE_HASHMAP_H
