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

private slots:
    void clientIsConnected();
    void dataComing();

};

#endif // SERVEUR_H
