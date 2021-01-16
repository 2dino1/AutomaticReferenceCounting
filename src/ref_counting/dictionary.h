#ifndef DICTIONARY
#define DICTIONARY

typedef struct Dictionary dictionary_t;

#endif

dictionary_t *init_dictionary();
void insert_value(dictionary_t *dictionary, void *key, const pthread_mutex_t mutex);
void release_dictionary(dictionary_t *dictionary);