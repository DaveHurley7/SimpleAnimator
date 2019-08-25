#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QString>
#include <QSqlDatabase>
#include "project.h"
#include <QDebug>
#include <QSqlQuery>
#include <QAction>
#include <QList>
#include "recentprojitem.h"

class DBCon
{
private:
    DBCon(){}
    QSqlDatabase db_conn;
    time_t t;
    static DBCon* db;
    QSqlQuery q;

public:
    ~DBCon(){
        db_conn.close();
    }
    bool addProject(QString name, QString path);
    project activeProjAvailable();
    static DBCon* DB(){
        if(db == nullptr){
            db = new DBCon();
            db->connectDB();
        }
        return db;
    }
    void connectDB(){
        db_conn = QSqlDatabase::addDatabase("QSQLITE","projDB");
        db_conn.setDatabaseName("projectlist.db");
        db_conn.open();
    }
    void createTableIfNotExists();
    bool projectExists(QString path, QString name);
    void updateProjectInfo(QString path, QString name);
    project getProject(QString path, QString name);
    void closeProject(QString path, QString name);
    bool otherProjsActive(QString path, QString name);
    QList<rpItem> getRecentProjList();
};

#endif // DBCONNECTION_H
