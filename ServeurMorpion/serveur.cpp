#include "serveur.h"
#include <QList>
#include <QDebug>
#include <QDataStream>

Serveur::Serveur(QObject *parent)
    : QObject{parent}
{
    mServeur = new QTcpServer(this);

    connect(mServeur,SIGNAL(newConnection()), this,SLOT(clientIsConnected()));


    // Vérification de la mise en place du serveur
    if (!mServeur->listen(QHostAddress::Any,9999)) {
        qDebug() << "Serveur n'a pas pu démarrer!";
            qDebug() << "Erreur: " << mServeur->errorString();
    } else {
        qDebug() << "Serveur démarré sur le port 9999";
    }

//    // Initialisation de l'état du jeu
    gameGrid = QVector<QVector<QChar>>(6, QVector<QChar>(7, ' '));

//    // grille 7*6 remplie de cases vides
   currentPlayer = 'X';  // le joueur X commence
}

void Serveur::clientIsConnected()
{
    // Récupération du socket de la connexion entrante
    QTcpSocket* sock = mServeur->nextPendingConnection();

    // Vérification du socket
    if (!sock) {
        qDebug() << "Erreur lors de la récupération du socket de la connexion entrante";
        return;
    }

    mClients.append(sock);  // Ajout du socket à la liste des clients connectés

    // Connexion du signal disconnected du socket au slot deleteLater
    connect(sock, SIGNAL(disconnected()),sock,SLOT(deleteLater()));

    // Connexion du signal readyRead du socket au slot dataComing
    connect(sock,SIGNAL(readyRead()),this,SLOT(dataComing()));

    sock->write("Bonjour !");  // Envoi d'un message de bienvenue au client

    // Affichage de l'adresse du client qui se connecte
    qDebug() << "quelqu'un se connecte depuis" << sock->peerAddress().toString();

    // Envoi de l'état initial du jeu au nouveau client
    updateGameState();
}

void Serveur::updateGameState() {

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);  // set the same version on the client side

    // Serialize the gameGrid and currentPlayer into the message
    out << gameGrid << currentPlayer;

    // Send the message to all clients
    for (QTcpSocket* client : qAsConst(mClients)) {
        client->write(block);
    }
}

void Serveur::dataComing()
{
    // Récupération du socket qui a envoyé les données
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());
    if (sock)  // Si le cast a réussi
    {
        QByteArray data = sock->readAll();  // Lecture de toutes les données reçues
        QDataStream in(&data, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_15);

        // Vérification de la quantité de données
        if (in.atEnd()) {
            qDebug() << "Erreur : données insuffisantes pour la désérialisation";
            return;
        }

        // Désérialisation de l'action envoyée par le client
        QString action;
        int row;
        int column;
        in >> action >> row >> column;

        // Mise à jour de l'état du jeu en fonction de l'action
        if (action == "place" && gameGrid[row][column] == ' ') {
            gameGrid[row][column] = currentPlayer;
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';  // changement de joueur
        }

        // Affichage des données reçues
        qDebug() << "recu de :"<< sock->peerAddress().toString() << data;

        // Echo du message à tous les clients connectés
        for (QTcpSocket* client : qAsConst(mClients)) {
            client->write(data);
        }
        updateGameState();


    }else {
        qDebug() << "Erreur : le cast du socket a échoué";
    }
}
