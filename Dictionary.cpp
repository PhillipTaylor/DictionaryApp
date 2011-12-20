#include "Dictionary.h"
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <memory>
#include <math.h>
#include <QtGui/QMessageBox>
#include <QString>
#include "worditerator.h"

using namespace std;

const int BUFFER_SIZE = 255;

EnglishDictionary::EnglishDictionary() :
  longestWord(0),
  dictionary()
{}

void EnglishDictionary::emptyDictionary()
{ 
    longestWord = 0;
    dictionary.empty();
}

bool EnglishDictionary::isWordValid(const string& word) const
{
    word_key_t key;
    bucket_t bucket;
    word_key_t bucket_address;
    bucket_t bit;
    bucket_t mask;

    key = this->getKey(word);

    //remember each array element is a 32 bit int.
    //so we have 32 words described in one array element.
    //use a mask and bitwise operations to extract the bit
    //representing our word in question.
    bucket_address = key / WORDS_PER_BUCKET;
    cout << "BUCKET ADDRESS: " << bucket_address << endl;

    if (bucket_address >= dictionary.size()) {
      cout << "dictionary too small to possible allow word" << endl;
      return 0;
    }
    
    bucket = dictionary[bucket_address];

    cout << "contenst of bucket " << bucket << endl;
    bit = key % WORDS_PER_BUCKET;

    mask = pow(2, bit);

    return (bucket & mask);
}

word_key_t EnglishDictionary::getKey(const std::string& word) const
{
    int pos;
    word_key_t char_key;
    word_key_t key = 0;
    const char *raw = word.c_str();

    cout << "Entering getKey loop with " << raw << endl;
    for (pos = 0; raw[pos] != '\0'; pos++) {
        if (raw[pos] >= 'A' && raw[pos] <= 'Z') {
            char_key = raw[pos] - 'A';
	} else if (raw[pos] >= 'a' && raw[pos] <= 'z') {
            char_key = raw[pos] - 'a';
	} else {
            throw new runtime_error("Unrecognised Character");
        }

        key += (char_key + 1) * (pow(CHARS_IN_ALPHABET, pos));
    }

    cout << "word: " << raw << " ,score: " << key << endl;
    return key;
}

void EnglishDictionary::addWord(const string& word)
{
    word_key_t key;
    bucket_t bucket;
    bucket_t bucket_address;
    bucket_t bit_address;
    bucket_t mask;

    key = this->getKey(word);

    bucket_address = key / WORDS_PER_BUCKET;
    cout << "BUCKET ADDRESS: " << bucket_address << endl;

    if (bucket_address > 100000)
      throw new runtime_error("memory required too big");
    
    //extend the dictionary!
    if (dictionary.size() <= bucket_address) {
      dictionary.resize(bucket_address+1, 0); cout << "resizing to " << (bucket_address + 1) << endl;
    }
    bucket = dictionary[bucket_address];
    cout << "contents of bucket:" << bucket << endl;
    bit_address = key % WORDS_PER_BUCKET;
    mask = pow(2, bit_address);

    // switch bit to 1
    dictionary[bucket_address] = (mask | bucket);
    cout << "set to " << (mask | bucket) << endl;
}

void EnglishDictionary::removeWord(const string& word)
{
    word_key_t key;
    bucket_t bucket_address;
    bucket_t bucket;
    bucket_t bit_address;
    bucket_t mask;

    key = this->getKey(word);

    bucket_address = key / WORDS_PER_BUCKET;

    if (dictionary.size() <= bucket_address)
      return; //word was never in the dictionary.
    
    bucket = dictionary[bucket_address];
    bit_address = key % WORDS_PER_BUCKET;
    mask = pow(2, bit_address);

    // switch bit to 0
    cout << "BUCKET SET TO " << bucket << " and mask set to " << mask << endl;
    dictionary[bucket_address] = (bucket ^ mask);
}

const long EnglishDictionary::getDictionarySizeBytes() const
{
  return dictionary.size() * sizeof(bucket_t);
}

void EnglishDictionary::compressDictionary()
{
  // delete all the elements at the end of
  // the array that are totally zero.
  int current_end = dictionary.size();

  if (current_end == 0)
    return;
  
  int new_end = current_end;

  while (dictionary[--new_end] == 0 && new_end >= 0) //cell is zero filled
    ; //no body

  if (new_end < current_end)
    dictionary.resize(new_end + 1);
}

WordIterator EnglishDictionary::begin() const
{
  cout << "returning word iterator..." << endl;
  WordIterator wi(dictionary);
  return wi;
}

WordIterator EnglishDictionary::end() const
{
  WordIterator wi(dictionary, true);
  return wi;
}
