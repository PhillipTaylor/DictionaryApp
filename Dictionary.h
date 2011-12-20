#ifndef ENGLISHDICTIONARY_H
#define ENGLISHDICTIONARY_H

#include <vector>
#include <string>
#include "DictTypes.h"

class WordIterator;

class EnglishDictionary
{
  public:
    EnglishDictionary();

    void emptyDictionary();
    
    void addWord(const std::string& word);
    void removeWord(const std::string& word);
    bool isWordValid(const std::string& word) const;

    void compressDictionary();

    const long getDictionarySizeBytes() const;

    WordIterator begin() const;
    WordIterator end() const;

  private:
    //No pointers or anything here. Satisfying the default
    //assignment, destructor and copy constructor will be
    //acceptable.
    
    std::vector<bucket_t> dictionary;
    word_key_t longestWord;

    word_key_t getKey(const std::string& word) const;
    //std::string reverseKey(const word_key_t key) const;
};

#endif // ENGLISHDICTIONARY_H
