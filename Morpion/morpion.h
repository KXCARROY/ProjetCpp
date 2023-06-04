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

private slots :
    void readyRead();
    void onButtonClicked(int index);
    void onConnected();


};

#endif // MORPION_H
