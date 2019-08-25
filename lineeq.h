#ifndef LINEEQ_H
#define LINEEQ_H

#include <cmath>
#include <QPoint>
#include <QDebug>

struct LineEq{
    double slope;
    double constant;
    LineEq(){}
    LineEq(double s, QPoint p){
        slope = s;
        constant = (s*-p.x())+p.y();
    }
    int calcY(int x){
        return int(round((x*slope)+constant));
    }
    int calcX(int y){
        return int(round((y-constant)/slope));
    }
    QPoint getIntersection(LineEq le){
        double m = slope - le.slope;
        double c = le.constant - constant;
        int x = int(round(c/m));
        return QPoint(x,calcY(x));
    }
};

#endif // LINEEQ_H
