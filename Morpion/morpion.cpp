#include "morpion.h"
#include "ui_morpion.h"
#include <QDataStream>
#include <QSignalMapper>


Morpion::Morpion(QWidget *parent) : QWidget(parent), ui(new Ui::Morpion)
{
    ui->setupUi(this);

    currentPlayer = 'X';

    // Connexion du signal connected de ma socket au slot onConnected de Morpion
    connect(&socket, &QTcpSocket::connected, this, &Morpion::onConnected);
    // Connexion du signal readyRead de ma socket au slot readyRead de Morpion
    connect(&socket, &QTcpSocket::readyRead, this, &Morpion::readyRead);
    socket.connectToHost("127.0.0.1", 9999);  // Connexion au serveur

    // Vérification de la connexion au serveur
    if (socket.state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Erreur de connexion au serveur";
    } else {
        qDebug() << "Le client est connecté au serveur";
    }

    //Initialilisation des boutons et connexion à la méthode onButtonClicked
    button = QVector<QVector<QPushButton*>>(6,QVector<QPushButton*>(7));
    QSignalMapper* mapper = new QSignalMapper(this);

    button[0][0] = ui->button00;
    button[0][1] = ui->button01;
    button[0][2] = ui->button02;
    button[0][3] = ui->button03;
    button[0][4] = ui->button04;
    button[0][5] = ui->button05;
    button[0][6] = ui->button06;
    button[1][0] = ui->button10;
    button[1][1] = ui->button11;
    button[1][2] = ui->button12;
    button[1][3] = ui->button13;
    button[1][4] = ui->button14;
    button[1][5] = ui->button15;
    button[1][6] = ui->button16;
    button[2][0] = ui->button20;
    button[2][1] = ui->button21;
    button[2][2] = ui->button22;
    button[2][3] = ui->button23;
    button[2][4] = ui->button24;
    button[2][5] = ui->button25;
    button[2][6] = ui->button26;
    button[3][0] = ui->button30;
    button[3][1] = ui->button31;
    button[3][2] = ui->button32;
    button[3][3] = ui->button33;
    button[3][4] = ui->button34;
    button[3][5] = ui->button35;
    button[3][6] = ui->button36;
    button[4][0] = ui->button40;
    button[4][1] = ui->button41;
    button[4][2] = ui->button42;
    button[4][3] = ui->button43;
    button[4][4] = ui->button44;
    button[4][5] = ui->button45;
    button[4][6] = ui->button46;
    button[5][0] = ui->button50;
    button[5][1] = ui->button51;
    button[5][2] = ui->button52;
    button[5][3] = ui->button53;
    button[5][4] = ui->button54;
    button[5][5] = ui->button55;
    button[5][6] = ui->button56;

    // Connexion des boutons au mapper
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            // Connexion du signal clicked du bouton au slot map du mapper
            connect(button[i][j], SIGNAL(clicked()), mapper, SLOT(map()));

            // Association du bouton avec un index dans le mapper et convertir des coordonnées bidimensionnelles en index unidimensionnel
            mapper->setMapping(button[i][j], i * 7 + j);

            qDebug() << "Bouton [" << i << "][" << j << "] connecté.";  // Ajoutez cette ligne
        }
    }

    // Connexion du signal du mapper au slot onButtonClicked
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(onButtonClicked(int)));

}






Morpion::~Morpion(){
    delete ui;  // Destruction de l'interface utilisateur
}

void Morpion::onConnected() {
    qDebug() << "Le client est connecté au serveur";

        // Vérification de ma socket avant d'envoyer des données
        if (socket.state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Erreur : le socket n'est pas connecté avant d'envoyer des données";
        return;
    }
}


// Slot appelé lorsqu'un bouton est cliqué
void Morpion::onButtonClicked(int index) {
    qDebug() << "Button clicked, index: " << index;


    int row = index / 7;
    int column = index % 7;

    // Vérification des limites
    if (row < 0 || row >= 6 || column < 0 || column >= 7) {
        qDebug() << "L'index est hors limite";
        return;
    }

    // Vérification si la case est vide
    if (button[row][column]->text() != "") {
        qDebug() << "La case est déjà occupée";
        return;
    }

    // Envoi d'une action au serveur
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);


    // Sérialisation de l'action dans le message
    out << "place" << row << column;

    qDebug() << "Sending action to server: place" << row << column;

    socket.write(block);  // Envoi du message au serveur

    // Changement du texte du bouton selon le joueur qui a cliqué
    button[row][column]->setText(currentPlayer);

    // Modification du joueur pour le prochain tour
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';

    button[row][column]->setText(QString(currentPlayer));

}

// Slot appelé lorsque des données sont prêtes à être lues sur le socket
void Morpion::readyRead() {
    QByteArray data = socket.readAll();  // Lecture de toutes les données disponibles

    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_15);

    // Vérification de la quantité de données
    if (data.size() < (int)(sizeof(QVector<QVector<QChar>>) + sizeof(QChar))) {
        qDebug() << "Erreur : données insuffisantes pour la désérialisation";
        return;
    }

    QVector<QVector<QChar>> gameGrid;
    QChar currentPlayer;

    // Tentez de désérialiser les données
    try {
        in >> gameGrid >> currentPlayer;
    } catch (...) {
        qDebug() << "Erreur lors de la désérialisation des données";
        return;
    }

    // Mise à jour de la grille de jeu
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            // Mise à jour du texte du bouton correspondant avec l'état de la case
            if (gameGrid[i][j] != ' ') {
                button[i][j]->setText(QString(gameGrid[i][j]));
                button[i][j]->setEnabled(false);
            }
        }
    }
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';

}







//Code tiktaktoe local

//#include "morpion.h"
//#include "ui_morpion.h"
//#include <QDataStream>
//#include <QSignalMapper>

//Morpion::Morpion(QWidget *parent) : QWidget(parent), ui(new Ui::Morpion)
//{
//    ui->setupUi(this);

//    // Initialisation des boutons et connexion à la méthode onButtonClicked
//    button = QVector<QVector<QPushButton*>>(6,QVector<QPushButton*>(7));
//    QSignalMapper* mapper = new QSignalMapper(this);
//    // (Initialisation des boutons avec les références du fichier ui...)

//    button[0][0] = ui->button00;
//    button[0][1] = ui->button01;
//    button[0][2] = ui->button02;
//    button[0][3] = ui->button03;
//    button[0][4] = ui->button04;
//    button[0][5] = ui->button05;
//    button[0][6] = ui->button06;
//    button[1][0] = ui->button10;
//    button[1][1] = ui->button11;
//    button[1][2] = ui->button12;
//    button[1][3] = ui->button13;
//    button[1][4] = ui->button14;
//    button[1][5] = ui->button15;
//    button[1][6] = ui->button16;
//    button[2][0] = ui->button20;
//    button[2][1] = ui->button21;
//    button[2][2] = ui->button22;
//    button[2][3] = ui->button23;
//    button[2][4] = ui->button24;
//    button[2][5] = ui->button25;
//    button[2][6] = ui->button26;
//    button[3][0] = ui->button30;
//    button[3][1] = ui->button31;
//    button[3][2] = ui->button32;
//    button[3][3] = ui->button33;
//    button[3][4] = ui->button34;
//    button[3][5] = ui->button35;
//    button[3][6] = ui->button36;
//    button[4][0] = ui->button40;
//    button[4][1] = ui->button41;
//    button[4][2] = ui->button42;
//    button[4][3] = ui->button43;
//    button[4][4] = ui->button44;
//    button[4][5] = ui->button45;
//    button[4][6] = ui->button46;
//    button[5][0] = ui->button50;
//    button[5][1] = ui->button51;
//    button[5][2] = ui->button52;
//    button[5][3] = ui->button53;
//    button[5][4] = ui->button54;
//    button[5][5] = ui->button55;
//    button[5][6] = ui->button56;
//    // Connexion des boutons au mapper
//    for (int i = 0; i < 6; ++i) {
//        for (int j = 0; j < 7; ++j) {
//            connect(button[i][j], &QPushButton::clicked, [i,j]() {
//                qDebug() << "Button " << i << "," << j << " clicked";
//            });

//            // Connexion du signal clicked du bouton au slot map du mapper
//            connect(button[i][j], SIGNAL(clicked()), mapper, SLOT(map()));

//            // Association du bouton avec un index dans le mapper et convertir des coordonnées bidimensionnelles en index unidimensionnel
//            mapper->setMapping(button[i][j], i * 7 + j);

//        }
//    }
//    // Connexion du signal du mapper au slot onButtonClicked
//    connect(mapper, SIGNAL(mapped(int)), this, SLOT(onButtonClicked(int)));
//}


//Morpion::~Morpion(){
//    delete ui;  // Destruction de l'interface utilisateur
//}

//// Slot appelé lorsqu'un bouton est cliqué
//void Morpion::onButtonClicked(int index) {
//    int row = index / 7;
//    int column = index % 7;

//    // Vérification des limites
//    if (row < 0 || row >= 6 || column < 0 || column >= 7) {
//        qDebug() << "L'index est hors limite";
//        return;
//    }

//    // Vérification si la case est vide
//    if (button[row][column]->text() != "") {
//        qDebug() << "La case est déjà occupée";
//        return;
//    }


//    // Change le texte du bouton pour l'action du joueur local
//    button[row][column]->setText("X");
//    button[row][column]->setEnabled(false);
//}



