
#ifndef MORPION_H
#define MORPION_H

#include <QWidget>



QT_BEGIN_NAMESPACE
namespace Ui { class Morpion; }
QT_END_NAMESPACE

class Morpion : public QWidget

{
    Q_OBJECT

public:
    Morpion(QWidget *parent = nullptr);
    ~Morpion();

private:
    Ui::Morpion *ui;
};

#endif // MORPION_H
