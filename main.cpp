#include <QtGui/QApplication>
#include "DictionaryApp.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    DictionaryApp foo;
    foo.show();
    return app.exec();
}
