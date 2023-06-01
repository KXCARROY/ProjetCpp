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

    //Initialilisation des boutons et connexion à la méthode onButtonClicked
    buttons = QVector<QVector<QPushButton*>>(7,QVector<QPushButton*>(6));
    QSignalMapper* mapper = new QSignalMapper(this);
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 6; ++j) {
            buttons[i][j] = new QPushButton(this);

            // Connexion du signal clicked du bouton au slot map du mapper
            connect(buttons[i][j], SIGNAL(clicked()), mapper, SLOT(map()));

            // Association du bouton avec un index dans le mapper et convertir des coordonnées bidimensionnelles en index unidimensionnel
            mapper->setMapping(buttons[i][j], i * 7 + j);
        }
    }
    // Connexion du signal du mapper au slot onButtonClicked
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(onButtonClicked(int)));
}


Morpion::~Morpion(){
    delete ui;  // Destruction de l'interface utilisateur
}


// Slot appelé lorsqu'un bouton est cliqué
void Morpion::onButtonClicked(int index) {
    int row = index / 7;
    int column = index % 7;

    // Envoi d'une action au serveur
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);

    // Sérialisation de l'action dans le message
    out << "place" << row << column;

    socket.write(block);  // Envoi du message au serveur
}
