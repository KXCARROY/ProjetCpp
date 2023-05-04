
#ifndef SERVEURTCP_H
#define SERVEURTCP_H


#include <QObject>


class ServeurTCP : public QObject
{
    Q_OBJECT
public:
    explicit ServeurTCP(QObject *parent = nullptr);

signals:

};

#endif // SERVEURTCP_H
