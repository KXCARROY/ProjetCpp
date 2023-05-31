#include "client.h"
#include <QDataStream>

Client::Client(QObject *parent) : QObject(parent) {
    connect(&socket, &QIODevice::readyRead, this, &Client::readyRead);
    socket.connectToHost("localhost", 9999);
    buttons = QVector<QVector<QPushButton*>>(6, QVector<QPushButton*>(7));  // initialize buttons
    // TODO: create the QPushButton objects and add them to the UI
}

void Client::readyRead() {
    QByteArray data = socket.readAll();
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_5_15);

    QVector<QVector<QChar>> gameGrid;
    in >> gameGrid;  // deserialize gameGrid from the data received from the server

    updateGameGrid(gameGrid);
}

void Client::updateGameGrid(const QVector<QVector<QChar>> &gameGrid) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            buttons[i][j]->setText(QString(gameGrid[i][j]));
        }
    }
}
