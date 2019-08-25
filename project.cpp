#include "project.h"

Project::Project(QString projName, QString projPath)
{
    name = projName.toUtf8().constData();
    path = projPath.toUtf8().constData();
}
