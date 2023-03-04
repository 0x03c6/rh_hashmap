#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rh_hashmap/rh_hashmap.h"

// TODO: maybe implement distance function/macro? ugly to inline calc each time

typedef struct entry {
  const char* key;
  void* value;
} entry;

typedef struct rh_hashmap {
  entry* buffer;
  unsigned int capacity;
  unsigned int size;
} rh_hashmap;

// for more expressive dyanmic memory allocation
static void* memdup(const void* src, size_t len) {
  return memcpy(malloc(len), src, len);
}

// fowler noll vo hashing algorithm (32 bit)
unsigned int fnv1a32_hash(const char* src) {
  int hash = 0x811c9dc5;
  for (int i = 0; src[i] != 0; ++i) {
    hash ^= src[i];
    hash *= 0x01000193;
  }
  return hash;
}

rh_hashmap* hashmap_create(void) {
  return ALLOC(rh_hashmap, {
    .buffer = calloc(HASHMAP_INIT, sizeof(entry)),
    .capacity = HASHMAP_INIT,
    .size = 0
  });
}

void hashmap_destroy(rh_hashmap* map) {
  if (map) {
    free(map->buffer);
    free(map);
  }
  return;
}

int hashmap_find(rh_hashmap* map, const char* key) {
  unsigned int index = fnv1a32_hash(key) % map->capacity;
  int d = 0;

  while (map->buffer[index].key) {
    // check to see if first index is correct
    if (strcmp(map->buffer[index].key, key) == 0)
      return index;

    // calculate current distance
    int c = (index + map->capacity - fnv1a32_hash(map->buffer[index].key)
      % map->capacity) % map->capacity;

    if (c < d) {
      // if current distance is less than distance, swap
      entry tmp = map->buffer[index];
      map->buffer[index] = map->buffer[(index + c) % map->capacity];
      map->buffer[(index + c) % map->capacity] = tmp;
      d = c;
    }
    index = (index + 1) % map->capacity;
    ++d;
  }
  return HASHMAP_ERROR;
}

int hashmap_insert(rh_hashmap* map, const char* key, void* value) {
  // if load is larger than defined, resize to double capacity
  if ((double)map->size / map->capacity >= HASHMAP_LOAD)
    hashmap_resize(map, map->capacity * 2);

  unsigned int index = hashmap_find(map, key);
  if (index == -1)
    index = fnv1a32_hash(key) % map->capacity;

  // insert values into map
  map->buffer[index].key = key;
  map->buffer[index].value = value;
  map->size++;

  return index;
}

void* hashmap_retrieve(rh_hashmap* map, const char* key) {
  unsigned int index = hashmap_find(map, key);

  return (index != -1)
    ? map->buffer[index].value
    : (void *)-1;  
}

int hashmap_remove(rh_hashmap* map, const char* key) {
  unsigned int index = hashmap_find(map, key);
  if (index != -1) {
    // if index is correct, zero out the entry on the map
    map->buffer[index].key = 0;
    map->buffer[index].value = 0;
    map->size--;

    for (int c = (index+1) % map->capacity, d=1; map->buffer[c].key && d > 0; ++d) {
      // calculate current distance
      int cd = (c + map->capacity - fnv1a32_hash(map->buffer[c].key)
        % map->capacity) % map->capacity;

      // if current distance is less than distance, swap
      if (cd < d) {
        entry tmp = map->buffer[index];
        map->buffer[index] = map->buffer[c];
        map->buffer[c] = map->buffer[index];

        index = c;
        d = cd;
      }
      c = (c + 1) % map->capacity;
    }
  }

  // if load factor is too small, reduce size of map by capacity / 2
  if (map->size > HASHMAP_INIT && (double)map->size / map->capacity < HASHMAP_LOAD/2)
    hashmap_resize(map, map->capacity / 2);

  return index;
}

void hashmap_resize(rh_hashmap* map, unsigned int capacity) {
  // allocate initial buffer to hold new entries
  entry* buffer = calloc(capacity, sizeof(entry));

  for (int i = 0; i < map->capacity; ++i) {
    if (map->buffer[i].key) {
      // calculate index for key on hashmap
      int index = fnv1a32_hash(map->buffer[i].key) % capacity;

      for (int d = 0; buffer[index].key != 0; ++d) {
        // calculate new current distance with new values
        int cd = (index + capacity - fnv1a32_hash(buffer[index].key)
          % capacity) % capacity;

        if (cd < d) {
          // if current distance is less than distance, swap
          entry tmp = buffer[index];
          buffer[index] = map->buffer[i];
          map->buffer[i] = tmp;

          d = cd;
        }
        // increment index within bounds of capacity
        index = (index + 1) % capacity;
      }
      // update value
      buffer[index] = map->buffer[i];
    }
  }
  // destroy old buffer for entries and replace values on map
  free(map->buffer);
  map->buffer = buffer;
  map->capacity = capacity;
  return;
}

