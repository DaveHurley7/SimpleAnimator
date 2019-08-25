#ifndef BONE_H
#define BONE_H

#include <QPoint>
#include <QList>

class Bone{
public:
    Bone(QPoint p, int f, QList<int> t){
        coords = p;
        from = f;
        to = t;
    }
    QPoint coords;
    int from;
    QList<int> to;
};

#endif // BONE_H
