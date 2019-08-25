#ifndef PROJECTREADER_H
#define PROJECTREADER_H

#include <QFile>
#include <QDir>
#include "comptype.h"
#include "frame.h"
#include "recentprojitem.h"

class ProjectIOHandler
{
public:
    ProjectIOHandler();
    static bool load(QString projName, QString projPath, QList<Frame> *fList, QList<CompType*> *comps, struct projectinfo *projInfo);
    static void save(QString projPath, QList<Frame> *frames);
    static bool newProject();
    static bool openProject(QWidget *window);
    static void openRecentProject(QWidget *w, bool saved, rpItem p);
    enum ErrorStatus{
        NONE,
        MISSING,
        CORRUPTED_JSON
    };

private:
    static QString homeUrl;
};
#endif // PROJECTREADER_H
