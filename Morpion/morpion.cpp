
#include "morpion.h"
#include "ui_morpion.h"


Morpion::Morpion(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Morpion)
{
    ui->setupUi(this);
}

Morpion::~Morpion()
{
    delete ui;
}


