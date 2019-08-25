#ifndef RECENTPROJITEM_H
#define RECENTPROJITEM_H

#include <QString>
#include <QAction>

struct rpItem{
    QString name;
    QString path;
    rpItem(QString n, QString p){
        name = n;
        path = p;
    }
};

#endif // RECENTPROJITEM_H
