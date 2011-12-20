#include "worditerator.h"

#include <string>
#include <math.h>
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace boost;

const int BUFFER_SIZE = 255;

WordIterator::WordIterator(const vector< bucket_t >& r_dictionary, bool end) :
  impl_dict(r_dictionary),
  key(0)
{
  if (end) {
    GotoLastRecord();
    key++; //end is normally one PAST the last record of course...
  }
}

WordIterator::WordIterator(const WordIterator& original) : //Copy constructor
  impl_dict(original.impl_dict),
  key(original.key)
{}

void WordIterator::GotoLastRecord() {
  bucket_t last_byte = impl_dict.size() - 1;

  while (impl_dict[last_byte] == 0) //will only happen on uncompacted dictionary.
    last_byte--;

  //get the last byte that has been set.
  word_key_t bit = WORDS_PER_BUCKET - 1;
  word_key_t mask = pow(2, bit);
  
  while ((impl_dict[last_byte] & mask) == 0) {
    bit--;
    mask = pow(2, bit);
  }
    
  key = last_byte * WORDS_PER_BUCKET + bit;
  cout << "last key: " << key << endl;
  return; //found it!
}

WordIterator& WordIterator::operator++()
{

  cout << "previous key: " << key;

  key++; //increment at least once, then scan to next valid word
  
  bucket_t bucket_address = key / WORDS_PER_BUCKET;
  word_key_t bit = key % WORDS_PER_BUCKET;

  while (impl_dict[bucket_address] == 0 && bucket_address < impl_dict.size()) { //scan whole buckets (skip 32 words per loop!)
    bucket_address++;
    bit = 0;
    key = bucket_address * WORDS_PER_BUCKET;
  }

  if (bucket_address == impl_dict.size()) {
    //end of word iterator list. may as well stop
    return *this;
  }

  //scan for the next bit in the bucket.
  word_key_t mask = pow(2, bit);
  word_key_t bucket_value = impl_dict[bucket_address];

  while ((bucket_value & mask) == 0) {

    if (bit == WORDS_PER_BUCKET) {
      bucket_address++;  //go to next bucket
      bit = 0;

      if (bucket_address + 1 >= impl_dict.size())
	return *this;

      key = bucket_address * WORDS_PER_BUCKET;
      bucket_value = impl_dict[bucket_address];
      
      while (impl_dict[bucket_address] == 0 && bucket_address < impl_dict.size()) { //scan forward.
	bucket_address++;
	bucket_value = impl_dict[bucket_address];
	bit = 0;
	key = bucket_address * WORDS_PER_BUCKET;
      }

      if (bucket_address + 1 >= impl_dict.size())
	return *this;

    }
    
    key++;
    bit++;
    mask = pow(2, bit);
  }

  cout << ", new key: " << key << endl;

  return *this;
}

WordIterator& WordIterator::operator++(int )
{
  ++(*this);
  return *this;
}

boost::shared_ptr< string > WordIterator::operator*()
{
  string s(reverseKey(key));
  shared_ptr<string> retval(new string(s));
  return retval;
}

WordIterator& WordIterator::operator=(const WordIterator& src) //move operator
{
  if (this != &src) {             //redundant, but am aware of typical issue in assignment to self.

   if (impl_dict != src.impl_dict)
     //more a case of not wanting to copy entire vectors.
     throw new runtime_error("Word iterator points to different dictionary");

   this->key = src.key;
   return *this;
  } else
    return *this;
}

bool WordIterator::operator==(const WordIterator& other)
{
  return (impl_dict == other.impl_dict && key == other.key);
}

bool WordIterator::operator!=(const WordIterator& other)
{
  return (impl_dict == other.impl_dict && key != other.key);
}

bool WordIterator::operator<(const WordIterator& other)
{
  return (impl_dict == other.impl_dict && key < other.key);
}

bool WordIterator::operator<=(const WordIterator& other)
{
  return (impl_dict == other.impl_dict && key <= other.key);
}

bool WordIterator::operator>(const WordIterator& other)
{
  return (impl_dict == other.impl_dict && key > other.key);
}

bool WordIterator::operator>=(const WordIterator& other)
{
  return (impl_dict == other.impl_dict && key >= other.key);
}

string WordIterator::reverseKey(const word_key_t key) const
{

  //REFACTOR: think about assigning the shared_ptr here to avoid
  //returning a blank string! string is put into a shared_ptr anyway
  //by the calling function.
  bool valid = validKey();
  if (!valid) {
    string s("");
    return s;
  }

  char chr, buffer[BUFFER_SIZE];
  int isolated, pos = 0, keyc = key;

  while (keyc > 0) {
    cout << "old key: " << keyc;

    isolated = (keyc - 1) % CHARS_IN_ALPHABET;
    chr = 'a' + isolated;
    buffer[pos++] = chr;
    keyc = (word_key_t) ((keyc - isolated) / CHARS_IN_ALPHABET);

    cout << ", isolated: " << isolated << ", chr: " << chr << ", new key: " << keyc << ", pos: " << pos << endl;
  }

  buffer[pos] = '\0';
  string retval(buffer);
  return retval;
}

bool WordIterator::validKey() const
{
  bucket_t bucket = key / WORDS_PER_BUCKET;
  word_key_t bit = key % WORDS_PER_BUCKET;

  if (bucket >= impl_dict.size())
    return false;

  word_key_t bucket_value = impl_dict[bucket];
  word_key_t mask = pow(2, bit);

  return ((bucket_value & mask) > 0);
}
