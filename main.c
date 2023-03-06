// demo main for rh_hashmap
#include <stdio.h>

#include "rh_hashmap/rh_hashmap.h"

int main(int argc, char** argv) {
  const char* names[] = {
    "jeremy",
    "todd",
    "samantha",
    "johnathan",
    "anthony",
    "brian",
    "leo",
    "miriam",
    "katarina",
    "harland",
    "anzo",
    "kenzo",
    "eshe",
    "esther",
    "kristian",
    "cathleen",
    "ivett",
    "kalevi",
    "rodrick",
    "carmen"
  };

  rh_hashmap* map = hashmap_create();

  int n = sizeof(names)/sizeof(*names);

  for (int i = 0; i < n; ++i)
    printf("%d\n", hashmap_insert(map, names[i], (void *)i+1));

  for (int i = 0; i < n; ++i)
    printf("%s: %d\n", names[i], hashmap_retrieve(map, names[i]));

  for (int i = 0; i < n; ++i)
    hashmap_remove(map, names[i]);

  printf("\nSize: %d\nCapacity: %d\n",
    hashmap_size(map),
    hashmap_capacity(map));

  hashmap_destroy(map);
  return 0;
}

