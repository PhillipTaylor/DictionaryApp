
#ifndef DICTIONARY_TYPES
#define DICTIONARY_TYPES

#include <inttypes.h>

typedef uint32_t word_key_t;
typedef unsigned int bucket_t;

const int CHARS_IN_ALPHABET = 26;
const int WORDS_PER_BUCKET = sizeof(bucket_t) * 8;

#endif