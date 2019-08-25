#ifndef COMPTYPE_H
#define COMPTYPE_H

#include <QString>
#include <QImage>

class CompType{
public:
    CompType(){}
    CompType(QString *n, QImage *i, QString *s){
        name = n;
        img = i;
        shape = s;
    }
    QString *name;
    QImage *img;
    QString *shape;
    bool modified = false;
    bool isRoot = false;
};

#endif // COMPTYPE_H
