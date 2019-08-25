#ifndef PROJECT_H
#define PROJECT_H
#include <QString>

struct project{
    QString path;
    QString name;
    long long dt;
    bool active;
    project(){}
    project(QString p, QString n, long long d, bool a){
        path = p;
        name = n;
        dt = d;
        active = a;
    }
};

#endif // PROJECT_H
