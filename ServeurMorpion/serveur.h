#ifndef SERVEUR_H
#define SERVEUR_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Serveur : public QObject
{
    Q_OBJECT
public:
    explicit Serveur(QObject *parent = nullptr);

signals:

private:
    QTcpServer* mServeur;
    QList<QTcpSocket*> mClients;
    QChar currentPlayer; // Pour declarer 'X' et 'O'
    QVector<QVector<QChar>> gameGrid;

private slots:
    void clientIsConnected();
    void dataComing();
    void updateGameState();

};

#endif // SERVEUR_H
