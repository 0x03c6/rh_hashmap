// demo main for rh_hashmap
#include <stdio.h>

#include "rh_hashmap/rh_hashmap.h"

int main(int argc, char** argv) {
  rh_hashmap* map = hashmap_create();


  hashmap_destroy(map);
  return 0;
}

