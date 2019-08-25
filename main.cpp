#include "startscreen.h"
#include "editscreen.h"
#include <QApplication>
#include "dbconnection.h"
#include "project.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DBCon::DB()->createTableIfNotExists();
    project proj = DBCon::DB()->activeProjAvailable();
    if(proj.path.isEmpty()){
        StartDialog w;
        w.exec();
    }else{
        EditScreen* es = new EditScreen(proj);
        es->show();
    }
    int resCode = a.exec();
    DBCon::DB()->~DBCon();
    return resCode;
}
