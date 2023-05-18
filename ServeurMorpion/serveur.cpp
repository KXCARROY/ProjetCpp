
#include "serveur.h"

Serveur::Serveur(QObject *parent)
    : QObject{parent}
{
    mServeur = new QTcpServer(this);
    connect(mServeur,SIGNAL(newConnection()),
            this,SLOT(clientIsConnected()));
    mServeur->listen(QHostAddress::Any,9999);

}

void Serveur::clientIsConnected()
{
    QTcpSocket* sock = mServeur->nextPendingConnection();
    connect(sock, SIGNAL(disconnected()),sock,SLOT(deleteLater()));
    connect(sock,SIGNAL(readyRead()),this,SLOT(dataComing()));
    sock->write("Bonjour !");
    qDebug() << "qqun se connecte depuis" << sock->peerAddress().toString();
}

void Serveur::dataComing()
{
    QTcpSocket* sock = (QTcpSocket*)sender();
    QByteArray b = sock->readAll();
    qDebug() << "recu de :"<< sock->peerAddress().toString() << b;
}
