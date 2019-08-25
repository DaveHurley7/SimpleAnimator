#include "dbconnection.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include "project.h"

bool DBCon::addProject(QString name, QString path){
    bool success = false;
    if(db_conn.open()){
        time_t res = time(nullptr);
        q.prepare("INSERT INTO projects (name, path, last_opened, active) VALUES (?,?,?,?);");
        q.bindValue(0,name);
        q.bindValue(1,path);
        q.bindValue(2,res);
        q.bindValue(3,1);
        if(q.exec()){
            success = true;
        }
        return success;
    }else{
        return success;
    }
}

project DBCon::activeProjAvailable(){
    if(db_conn.open()){
        q = QSqlQuery(db_conn);
        if(q.exec("SELECT * FROM projects")){
            while(q.next()){
                bool active = q.value(3).toBool();
                if(active){
                    QString name = q.value(0).toString();
                    QString path = q.value(1).toString();
                    long long dt = q.value(2).toLongLong();
                    project proj = project(path,name,dt,active);
                    return proj;
                }
            }
        }
    }
    return project("","",0,0);
}

bool DBCon::projectExists(QString path, QString name){
    if(db_conn.open()){
        q.prepare("SELECT * FROM projects WHERE path = ? AND name = ?");
        q.bindValue(0,path);
        q.bindValue(1,name);
        if(q.exec()){
            bool count = false;
            while(q.next()){count=true;}
            return count;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

void DBCon::updateProjectInfo(QString path, QString name){
    if(db_conn.open()){
        time_t t = time(nullptr);
        q.prepare("UPDATE projects SET last_opened = ?, active = ? WHERE path = ? AND name = ?");
        q.addBindValue(t);
        q.addBindValue(1);
        q.addBindValue(path);
        q.addBindValue(name);
        q.exec();
    }
}

project DBCon::getProject(QString path, QString name){
    if(db_conn.open()){
        q.prepare("SELECT * FROM projects WHERE path = ? AND name = ?");
        q.bindValue(0,path);
        q.bindValue(1,name);
        if(q.exec()){
            while(q.next()){
                long long dt = q.value(2).toLongLong();
                bool active = q.value(3).toBool();
                return project(path,name,dt,active);
            }
        }
        return project("","",0,0);
    }else{
        return project("","",0,0);
    }
}

void DBCon::closeProject(QString path, QString name){
    if(db_conn.open()){
        time_t t = time(nullptr);
        q.prepare("UPDATE projects SET last_opened = ?, active = 0 WHERE path = ? AND name = ?");
        q.bindValue(0,t);
        q.bindValue(1,path);
        q.bindValue(2,name);
        q.exec();
    }
}
bool DBCon::otherProjsActive(QString path, QString name){
    if(db_conn.open()){
        q.prepare("SELECT active FROM projects WHERE active = 1 AND NOT (path = ? AND name = ?) ");
        q.bindValue(0,path);
        q.bindValue(1,name);
        if(q.exec()){
            bool count = false;
            while(q.next()){
                count=true;
            }
            return count;
        }else{
            return false;
        }
    }else{
        return false;
    }
}
void DBCon::createTableIfNotExists(){
    if(db_conn.open()){
        q = QSqlQuery(db_conn);
        q.exec("CREATE TABLE IF NOT EXISTS projects(name varchar(30), path varchar(50), last_opened integer primary key, active integer)");
    }
}
QList<rpItem> DBCon::getRecentProjList(){
    if(db_conn.open()){
        QList<rpItem> recentItems;
        if(q.exec("SELECT name, path FROM projects WHERE active = 0 ORDER BY last_opened DESC")){
            while(q.next()){
                rpItem item = {q.value(0).toString(),q.value(1).toString()};
                recentItems.append(item);
            }
            return recentItems;
        }else{
            return QList<rpItem>();
        }
    }else{
        return QList<rpItem>();
    }
}
DBCon* DBCon::db = nullptr;
