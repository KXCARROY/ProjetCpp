#ifndef MORPION_H
#define MORPION_H

#include <QWidget>
#include <QObject>
#include <QTcpSocket>
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui { class Morpion; }
QT_END_NAMESPACE


class Morpion : public QWidget

{
    Q_OBJECT

public:
    explicit Morpion(QWidget *parent = nullptr);
    ~Morpion();

private:
    Ui::Morpion *ui;
    QTcpSocket socket;
    QVector<QVector<QPushButton*>> button;
    QChar currentPlayer;
    bool checkWin(QChar player);



private slots :
    void resetGame();

    void readyRead();
    void onButtonClicked(int index);
    void onConnected();
    void updateUI(const QVector<QVector<QChar>>& gameGrid, QChar newCurrentPlayer);
signals:
    void gameStateUpdated(const QVector<QVector<QChar>>& gameGrid, QChar currentPlayer);


};

#endif // MORPION_H
