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

    qDebug() << "Le client est connecté au serveur";

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
    for (int i = 1; i < 6; ++i) {
        for (int j = 1; j < 7; ++j) {

            // Connexion du signal clicked du bouton au slot map du mapper
            connect(button[i][j], SIGNAL(clicked()), mapper, SLOT(map()));

            // Association du bouton avec un index dans le mapper et convertir des coordonnées bidimensionnelles en index unidimensionnel
            mapper->setMapping(button[i][j], i * 7 + j);
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

// Slot appelé lorsque des données sont prêtes à être lues sur le socket
void Morpion::readyRead() {
    QByteArray data = socket.readAll();  // Lecture de toutes les données disponibles
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_5_15);

    QVector<QVector<QChar>> gameGrid;
    // Désérialisation de gameGrid à partir des données reçues du serveur
    in >> gameGrid;

    // Mise à jour de la grille de jeu
    for (int i = 1; i < 6; ++i) {
        for (int j = 1; j < 7; ++j) {
            // Mise à jour du texte du bouton correspondant avec l'état de la case
            button[i][j]->setText(QString(gameGrid[i][j]));
        }
    }
}
