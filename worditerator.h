#ifndef WORDITERATOR_H
#define WORDITERATOR_H

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

#include "Dictionary.h"
#include "DictTypes.h"

class WordIterator
{
  public:
    WordIterator(const std::vector<bucket_t>& r_dict_impl, bool end=false);
    WordIterator(const WordIterator& original);
    WordIterator& operator=(const WordIterator& src);
    WordIterator& operator++();
    WordIterator& operator++(int);

    boost::shared_ptr<std::string> operator*();

    bool operator==(const WordIterator& other);
    bool operator!=(const WordIterator& other);
    bool operator>(const WordIterator& other);
    bool operator>=(const WordIterator& other);
    bool operator<(const WordIterator& other);
    bool operator<=(const WordIterator& other);
    
    ~WordIterator() {};
  private:
    std::string reverseKey(const word_key_t key) const;
    bool validKey() const;
    void GotoLastRecord();
    
    const std::vector<bucket_t>& impl_dict;

    word_key_t key; //the current word we are on
};

#endif // WORDITERATOR_H
