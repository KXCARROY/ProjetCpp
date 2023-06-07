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

    gameGrid = QVector<QVector<QChar>>(6, QVector<QChar>(7, ' '));
    currentPlayer = 'X';  // le joueur X commence
    qDebug() << "Nombre de client connecté: " << mClients.count();

}

void Serveur::clientIsConnected()
{
    QTcpSocket* sock = mServeur->nextPendingConnection();

    if (!sock) {
        qDebug() << "Erreur lors de la récupération du socket de la connexion entrante";
        return;
    }

    mClients.append(sock);

    if (currentClient == nullptr) {
        currentClient = sock;
        qDebug() << "Le premier client à se connecter est :" << sock->peerAddress().toString();
    } else if (mClients.size() == 2 && mClients[0] == currentClient) {
        qDebug() << "Le premier client à se connecter est bien le même que mClients[0]";
    }

    connect(sock, SIGNAL(disconnected()),sock,SLOT(deleteLater()));
    connect(sock,SIGNAL(readyRead()),this,SLOT(dataComing()));

    sock->write("Bonjour !");
    qDebug() << "quelqu'un se connecte depuis" << sock->peerAddress().toString();

    updateGameState();
}

void Serveur::updateGameState() {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);

    out << gameGrid << currentPlayer;

    for (QTcpSocket* client : qAsConst(mClients)) {
        client->write(block);
    }
    qDebug() << "Mise à jour du jeu envoyée à tous les clients";
}


void Serveur::dataComing()
{
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());
    if (sock)
    {
        if (sock != currentClient) {
            qDebug() << "Ce n'est pas le tour de ce client.";
            return;
        }

        QByteArray data = sock->readAll();
        QDataStream in(&data, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_15);

        if (in.atEnd()) {
            qDebug() << "Erreur : données insuffisantes pour la désérialisation";
            return;
        }

        QString action;
        int row;
        int column;
        in >> action >> row >> column;

        if (action == "place" && gameGrid[row][column] == ' ') {
            gameGrid[row][column] = currentPlayer;
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            currentClient = (currentClient == mClients[0]) ? mClients[1] : mClients[0];
        }

        qDebug() << "recu de :"<< sock->peerAddress().toString() << data;

        for (QTcpSocket* client : qAsConst(mClients)) {
            client->write(data);
        }
        updateGameState();
    }
    else {
        qDebug() << "Erreur : le cast du socket a échoué";
    }
}



Serveur::~Serveur() {
    delete mServeur;
}
