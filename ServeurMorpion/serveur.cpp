#include "serveur.h"
#include <QList>
#include <QDebug>
#include <QDataStream>

Serveur::Serveur(QObject *parent)
    : QObject{parent}
{
    mServeur = new QTcpServer(this);

    connect(mServeur,SIGNAL(newConnection()), this,SLOT(clientIsConnected()));
    mServeur->listen(QHostAddress::Any,8080);

    // Initialisation de l'état du jeu
    gameGrid = QVector<QVector<QChar>>(6, QVector<QChar>(7, ' '));  // grille 6x7 remplie de cases vides
    currentPlayer = 'X';  // le joueur X commence
}

void Serveur::clientIsConnected()
{
    // Récupération du socket de la connexion entrante
    QTcpSocket* sock = mServeur->nextPendingConnection();

    mClients.append(sock);  // Ajout du socket à la liste des clients connectés

    // Connexion du signal disconnected du socket au slot deleteLater
    connect(sock, SIGNAL(disconnected()),sock,SLOT(deleteLater()));

    // Connexion du signal readyRead du socket au slot dataComing
    connect(sock,SIGNAL(readyRead()),this,SLOT(dataComing()));

    sock->write("Bonjour !");  // Envoi d'un message de bienvenue au client

    // Affichage de l'adresse du client qui se connecte
    qDebug() << "quelqu'un se connecte depuis" << sock->peerAddress().toString();
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
        QByteArray b = sock->readAll();  // Lecture de toutes les données reçues
        // Affichage des données reçues
        qDebug() << "recu de :"<< sock->peerAddress().toString() << b;

        // Echo du message à tous les clients connectés
        for (QTcpSocket* client : qAsConst(mClients)) {
            client->write(b);
        }
        updateGameState();
    }
}


