#include "DictionaryApp.h"

#include <QString>
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <inttypes.h>
#include <boost/shared_ptr.hpp>

#include "Dictionary.h"
#include <stdexcept>
#include "worditerator.h"

typedef uint32_t word_key_t;

DictionaryApp::DictionaryApp() :
  txtWord(),
  lblSize(),
  dictionary(),
  lstWords()
{
    QWidget* mainWidget = new QWidget();
    QWidget* subWidget = new QWidget();
    QVBoxLayout* vlayout = new QVBoxLayout();
    QHBoxLayout* hlayout = new QHBoxLayout();

    QPushButton* btnIsValid = new QPushButton("Is Valid");
    connect(btnIsValid, SIGNAL(clicked()), this, SLOT(onIsWordValid()));

    QPushButton* btnAddWord = new QPushButton("Add Word");
    connect(btnAddWord, SIGNAL(clicked()), this, SLOT(onAddWord()));

    QPushButton* btnRemoveWord = new QPushButton("Remove Word");
    connect(btnRemoveWord, SIGNAL(clicked()), this, SLOT(onRemoveWord()));

    QPushButton* btnEncodeDecode = new QPushButton("Encode / Decode");
    connect(btnEncodeDecode, SIGNAL(clicked()), this, SLOT(onEncodeDecode()));
    
    QPushButton* btnCompress = new QPushButton("Compress");
    connect(btnCompress, SIGNAL(clicked()), this, SLOT(onCompressDictionary()));

    hlayout->addWidget(subWidget);
    hlayout->addWidget(&lstWords);
    
    vlayout->addWidget(&txtWord);
    vlayout->addWidget(&lblSize);
    vlayout->addWidget(btnIsValid);
    vlayout->addWidget(btnAddWord);
    vlayout->addWidget(btnRemoveWord);
    vlayout->addWidget(btnCompress);
    vlayout->addWidget(btnEncodeDecode); // button purely for testing.

    updateDictionarySize();

    subWidget->setLayout(vlayout);
    mainWidget->setLayout(hlayout);
    setCentralWidget( mainWidget );
}

DictionaryApp::~DictionaryApp()
{}

void DictionaryApp::showMsg(const char* msg)
{
  QString q(msg);
  QMessageBox mb;
  mb.setText(q);
  mb.exec();
}

void DictionaryApp::onIsWordValid()
{
  std::string s = txtWord.text().toStdString();

  if (s.length() == 0)
    showMsg("Not a word");
  else {
    if (dictionary.isWordValid(s)) {
      showMsg("Word is valid");
    } else {
      showMsg("Word is not valid");
    }
  }
}

void DictionaryApp::onAddWord()
{
  if (txtWord.text().length() > 0) {
      try {
	dictionary.addWord(txtWord.text().toStdString());
      } catch (std::runtime_error* re) {
	showMsg(re->what());
	return;
      }
    showMsg("Word Added");
    updateDictionarySize();
    updateWordList();
  } else
    showMsg("Not a valid word");
}

void DictionaryApp::onRemoveWord()
{
  if (txtWord.text().length() > 0) {
    dictionary.removeWord(txtWord.text().toStdString());
    updateWordList();
    showMsg("Word Removed");
  } else
    showMsg("Not a valid word");
}

void DictionaryApp::onCompressDictionary()
{
  dictionary.compressDictionary();
  updateDictionarySize();
}

void DictionaryApp::onEncodeDecode()
{

  if (txtWord.text().length() > 0) {
    word_key_t wordkey = txtWord.text().toUInt();

  } else
    showMsg("Not a valid word");
}

void DictionaryApp::updateDictionarySize()
{
  QString text("Dictionary Size (Bytes): ");
  text.append(QString::number(dictionary.getDictionarySizeBytes()));
  lblSize.setText(text);
}

void DictionaryApp::updateWordList()
{
  lstWords.clear();
  //my very own iterator!
  int x = 0;
  for (WordIterator wi = dictionary.begin(); wi < dictionary.end(); wi++, x++) {

    boost::shared_ptr<std::string> word_ptr = *wi;
    if (word_ptr->length() > 0) {
      QString word = word_ptr->c_str();
      QListWidgetItem *lwi = new QListWidgetItem(word);
      lstWords.addItem(word);
    }

  }

}

#include "DictionaryApp.moc"
