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
    ~Serveur();

private slots:
    void clientIsConnected();
    void dataComing();

private:
    QTcpServer* mServeur;
    QList<QTcpSocket*> mClients;
    QTcpSocket* currentClient = nullptr;
    QVector<QVector<QChar>> gameGrid;
    QChar currentPlayer = 'X';

    void updateGameState();
};

#endif // SERVEUR_H
