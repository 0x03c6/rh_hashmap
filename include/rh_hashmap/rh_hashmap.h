#ifndef RH_HASHMAP_H_
#define RH_HASHMAP_H_

#define HASHMAP_INIT (unsigned int)(16)
#define HASHMAP_LOAD (double)(0.7)

#define ALLOC(type, ...) \
  (type *)memdup((type []){__VA_ARGS__}, sizeof(type))

#define HASHMAP_ERROR -1

typedef struct rh_hashmap rh_hashmap;

/* create an empty hashmap with an initial capacity of 16 */
rh_hashmap* hashmap_create(void);

/* destroy a hashmap */
void hashmap_destroy(rh_hashmap* map);

/* find index of a key within hashmap, returns index for success and -1 for failure */
int hashmap_find(rh_hashmap* map, const char* key);

/* inserts an index into hashmap, returns index of entry inserted */
int hashmap_insert(rh_hashmap* map, const char* key, void* value);

/* retrieves value from entry by key, returns value on success and -1 on error */
void* hashmap_retrieve(rh_hashmap* map, const char* key);

/* removes an entry from a hashmap, returns the index of the removed entry */
int hashmap_remove(rh_hashmap* map, const char* key);

/* resizes the hashmap given a new capacity, does not return */
void hashmap_resize(rh_hashmap* map, unsigned int capacity);

/* return size of hashmap */
unsigned int hashmap_size(rh_hashmap* map);

/* return capacity of hashmap */
unsigned int hashmap_capacity(rh_hashmap* map);
#endif // RH_HASHMAP_H_
