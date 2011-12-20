#ifndef DictionaryApp_H
#define DictionaryApp_H

#include <QString>
#include <QtGui/QMainWindow>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include "Dictionary.h"

class DictionaryApp : public QMainWindow
{
Q_OBJECT
public:
    DictionaryApp();
    virtual ~DictionaryApp();
private:
    QLineEdit txtWord;
    QLabel lblSize;
    QListWidget lstWords;
    
    EnglishDictionary dictionary;
    void showMsg(const char* msg);
    void updateDictionarySize();
    void updateWordList();
    
private slots:
    void onIsWordValid();
    void onAddWord();
    void onRemoveWord();
    void onCompressDictionary();
    void onEncodeDecode();
};

#endif // DictionaryApp_H
