#include "mathshandler.h"
#include <cmath>
#include <QDebug>

MathsHandler::MathsHandler(){}

QPoint MathsHandler::rotatePoint(QPoint pivot, QPoint p, int a){
    p-=pivot;
    double angle = a+(180/M_PI);
    p = QPoint(int(p.x()*cos(angle)-p.y()*sin(angle)),int(p.y()*cos(angle)+p.x()+sin(angle)));
    p+=pivot;
    return p;
}
