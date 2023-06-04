#include "morpion.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Morpion w;
    w.show();
    return a.exec();
}
