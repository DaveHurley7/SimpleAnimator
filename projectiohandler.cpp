#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QApplication>
#include <QJsonArray>
#include <QMessageBox>
#include <cmath>
#include "component.h"
#include "bone.h"
#include "frame.h"
#include "projectinfo.h"
#include "projectiohandler.h"
#include "project.h"
#include "mathshandler.h"
#include "createprojwindow.h"
#include "editscreen.h"
#include "dbconnection.h"

ProjectIOHandler::ProjectIOHandler(){}

bool ProjectIOHandler::load(QString projName, QString projPath, QList<Frame> *fList, QList<CompType*> *comps, projectinfo *projInfo){
    ErrorStatus error = NONE;
    QFile projF(projPath+"/"+projName+".smpanim");
    QFile f(projPath+"/coordinates.smpcoords");
    if(projF.open(QIODevice::ReadOnly)){
        QString content = projF.readAll();
        QJsonParseError *jpe = new QJsonParseError;
        QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8(),jpe);
        if(jpe->error!=0){
            error = CORRUPTED_JSON;
        }else{
            QJsonObject obj = doc.object();
            QJsonObject cvsObj = obj["canvas_size"].toObject();
            projInfo->frameRate = obj["framerate"].toInt();
            projInfo->cvsSize = QSize(cvsObj["width"].toInt(),cvsObj["height"].toInt());
            QJsonObject components = obj["components"].toObject();
            for(QJsonValue v : components.keys()){
                QString *name = new QString;
                *name = v.toString();
                QImage *img = new QImage(projPath+"/components/"+name+".png");
                QString *shape = new QString;
                *shape = components[*name].toString();
                comps->append(new CompType(name,img,shape));
            }
        }
    }else{
        error = MISSING;
    }
    projF.close();
    if(f.open(QIODevice::ReadOnly)){
        QString content = f.readAll();
        QJsonParseError *jpe = new QJsonParseError;
        QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8(),jpe);
        if(jpe->error!=0){
            error = CORRUPTED_JSON;
        }else{
            QJsonObject obj = doc.object();
            QJsonArray frames = obj["frames"].toArray();
            for(QJsonValue v: frames){
                v.toObject();
                QList<CompInfo*> *compsInfo = new QList<CompInfo*>;
                QJsonArray components = v["components"].toArray();
                for(QJsonValue c : components){
                    int cIndex = c["cIndex"].toInt();
                    QJsonArray xy = c["coords"].toArray();
                    QPoint point(xy.at(0).toInt(),xy.at(1).toInt());
                    int angle = c["angle"].toInt();
                    LineEq l = LineEq(tan(angle*(M_PI/180)),point);
                    CompInfo *comp = new CompInfo(cIndex,point,angle,l);
                    compsInfo->append(comp);
                }
                QList<Bone*> *bones = new QList<Bone*>;
                QJsonArray bs = v["bones"].toArray();
                for(QJsonValue b : bs){}
                Frame f = Frame(compsInfo,bones);
                fList->append(f);
            }
        }
    }else{
        error = MISSING;
    }
    f.close();
    if(error!=NONE){
        QMessageBox errorMsg;
        errorMsg.setIcon(QMessageBox::Critical);
        errorMsg.setStandardButtons(QMessageBox::Ok);
        switch(error){
            case NONE:
                break;
            case MISSING:
                errorMsg.setText("Error: Project file missing");
                errorMsg.setInformativeText("The JSON file for the project is missing");
                break;
            case CORRUPTED_JSON:
                errorMsg.setText("Error: JSON is corrupted");
                errorMsg.setInformativeText("The data stored in JSON has an incorrect structure");
                break;
        }
        QApplication::beep();
        errorMsg.adjustSize();
        errorMsg.exec();
        return false;
    }else{
        return true;
    }
}

void ProjectIOHandler::save(QString projPath, QList<Frame> *frames){
    QJsonArray frameAr;
    QJsonObject frameObj;
    for(Frame f : *frames){
        QJsonArray comps;
        QJsonObject compObj;
        for(CompInfo *c : *(f.components)){
            QJsonObject comp;
            QJsonArray coordAr;
            comp["cIndex"] = c->cIndex;
            comp["angle"] = c->angle;
            coordAr.append(c->coord.x());
            coordAr.append(c->coord.y());
            comp["coords"] = coordAr;
            comps.append(comp);
        }
        compObj["components"] = comps;
        frameAr.append(compObj);
    }
    frameObj["frames"] = frameAr;
    QJsonDocument jdoc(frameObj);
    QFile framesF(projPath+"/coordinates.smpcoords");
    if(framesF.open(QIODevice::WriteOnly)){
        framesF.write(jdoc.toJson());
    }
    framesF.close();
}

bool ProjectIOHandler::newProject(){
    CreateProjDialog* pWin = new CreateProjDialog();
    int res = pWin->exec();
    return (res) ? true : false;
}

bool ProjectIOHandler::openProject(QWidget *window){
    QString filename = QFileDialog::getOpenFileName(window,"Open Project",homeUrl,"Simple Animator project (*.smpanim)");
    if(!filename.isEmpty()){
        QFileInfo fi(filename);
        QDir fp(fi.absolutePath());
        fp.cdUp();
        QString filePath = fp.absolutePath();
        QString fileName = fi.baseName();
        if(DBCon::DB()->projectExists(filePath,fileName)){
            DBCon::DB()->updateProjectInfo(filePath,fileName);
        }else{
            DBCon::DB()->addProject(fileName,filePath);
        }
        project proj = DBCon::DB()->getProject(filePath,fileName);
        EditScreen* es = new EditScreen(proj);
        es->show();
        return true;
    }else{
        return false;
    }
}

void ProjectIOHandler::openRecentProject(QWidget *w, bool saved, rpItem p){
    QMessageBox msgBox;
    msgBox.setText("Open Project");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setInformativeText("Would you like to open the project in this window or a new one?");
    msgBox.addButton("Current",QMessageBox::YesRole); //res = 0
    msgBox.addButton("New",QMessageBox::AcceptRole);  //res = 1
    msgBox.addButton("Cancel",QMessageBox::ResetRole);//res = 2
    msgBox.adjustSize();
    int res = msgBox.exec();
    if(res!=2){
        if(!res){
            if(saved){
                DBCon::DB()->updateProjectInfo(p.path,p.name);
                project proj = DBCon::DB()->getProject(p.path,p.name);
                EditScreen* es = new EditScreen(proj);
                es->show();
                if(!res){w->close();}
            }else{
                QMessageBox msgBox;
                msgBox.setText("Project Not Saved");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setInformativeText("There are unsaved changes in this project");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.adjustSize();
                msgBox.exec();
            }
        }else{
            DBCon::DB()->updateProjectInfo(p.path,p.name);
            project proj = DBCon::DB()->getProject(p.path,p.name);
            EditScreen* es = new EditScreen(proj);
            es->show();
        }
    }
}

QString ProjectIOHandler::homeUrl = QDir::homePath()+"/Documents/SimpleAnimatorProjects";
