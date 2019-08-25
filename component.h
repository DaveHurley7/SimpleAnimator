#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>
#include <QImage>
#include "lineeq.h"

class CompInfo{
public:
    CompInfo(int c, QPoint p, int a, LineEq l){
        cIndex = c;
        coord = p;
        angle = a;
        leq = l;
    }
    int cIndex;
    QPoint coord;
    int angle;
    LineEq leq;
    bool isRoot;
};

#endif // COMPONENT_H
