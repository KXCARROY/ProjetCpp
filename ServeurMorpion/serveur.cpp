#include "serveur.h"
#include <QList>
#include <QDebug>

Serveur::Serveur(QObject *parent)
    : QObject{parent}
{
    mServeur = new QTcpServer(this);

    connect(mServeur,SIGNAL(newConnection()), this,SLOT(clientIsConnected()));
    mServeur->listen(QHostAddress::Any,8080);
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
    }
}