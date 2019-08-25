#include "clickablegraphic.h"
#include <QDebug>

ClickableGraphic::ClickableGraphic(QWidget *parent): QGraphicsView(parent){
    this->setStyleSheet("background-color:transparent;border:0px");
}

ClickableGraphic::~ClickableGraphic(){}

void ClickableGraphic::mousePressEvent(QMouseEvent *ev){
    (void) ev;
    emit clicked();
}

void ClickableGraphic::setIcon(QObject *p, QPixmap pmp){
    gs = new QGraphicsScene(p);
    this->setScene(gs);
    gpi.setPixmap(pmp);
    gs->addItem(&gpi);
}
