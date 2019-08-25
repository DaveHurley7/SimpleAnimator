#ifndef PROJECTINFO_H
#define PROJECTINFO_H

#include <QSize>

struct projectinfo{
    int frameRate;
    QSize cvsSize;
    projectinfo(){}
    projectinfo(int f, QSize s){
        frameRate = f;
        cvsSize = s;
    }
};

#endif // PROJECTINFO_H
