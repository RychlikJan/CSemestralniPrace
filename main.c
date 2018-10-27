#include <stdio.h>

#include "HashMap.h"

int main() {
    printf("Hello, World!\n");
    HashMap *p_map = createHashMap(10);
    insert(p_map,"pes");
    insert(p_map,"kocka");
    insert(p_map,"mys");
    insert(p_map,"pes");
    insert(p_map,"pe");
    insert(p_map,"přespřílišžluťoučkýkůňúpěldábelskéódy");
    showMap(p_map);
    freeMap(p_map);

    return 0;
}