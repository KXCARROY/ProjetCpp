#include "morpion.h"
#include "ui_morpion.h"
#include <QDataStream>
#include <QSignalMapper>


Morpion::Morpion(QWidget *parent) : QWidget(parent), ui(new Ui::Morpion)
{
    ui->setupUi(this);

    // Connexion du signal readyRead de ma socket au slot readyRead de Morpion
    connect(&socket, &QTcpSocket::readyRead, this, &Morpion::readyRead);
    socket.connectToHost("localhost", 9999);  // Connexion au serveur

}


Morpion::~Morpion(){

}
