#include <QCoreApplication>
#include "serveur.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Serveur serveur;

    return a.exec();
}
