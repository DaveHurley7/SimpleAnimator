#include "createprojwindow.h"
#include "ui_createprojwindow.h"
#include <QFileDialog>
#include "editscreen.h"
#include "project.h"
#include "dbconnection.h"
#include <QDebug>
#include <QRegularExpressionMatch>
#include <ctime>
#include <QStandardPaths>
#include <QImage>
#include <QGraphicsPixmapItem>

CreateProjDialog::CreateProjDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::CreateProjDialog)
{
    ui->setupUi(this);
    ui->createProjError->hide();
    ui->newProjPath->setText(homeUrl);
    gs = new QGraphicsScene(this);
    ui->imagePreview->setScene(gs);
    gs->setBackgroundBrush(QBrush(Qt::gray,Qt::SolidPattern));
    previewH = ui->imagePreview->height();
    cvsImg = nullptr;
    connect(ui->browsePathForProj,SIGNAL(clicked()),this,SLOT(browseProjPath()));
    connect(ui->createProj,SIGNAL(clicked()),this,SLOT(createProject()));
    connect(ui->newEmptyProj,SIGNAL(clicked()),this,SLOT(toggleImageFind()));
    connect(ui->newProjWithImage,SIGNAL(clicked()),this,SLOT(toggleImageFind()));
    connect(ui->chooseImage,SIGNAL(clicked()),this,SLOT(chooseImageInDialog()));
}

CreateProjDialog::~CreateProjDialog()
{ 
    delete ui;
}

void CreateProjDialog::closeEvent(QCloseEvent *ev){
    (void) ev;
    this->done(1);
}

void CreateProjDialog::browseProjPath(){
    recentPath = ui->newProjPath->text();
    QFileDialog* fd = new QFileDialog();
    filename = fd->getExistingDirectory(this,"Choose Directory",homeUrl);
    if(filename.isEmpty()){
        ui->newProjPath->setText(recentPath);
    }else{
        ui->newProjPath->setText(filename.url());
    }
}

void CreateProjDialog::createProject(){
    QString projName = ui->newProjName->text();
    QString projPath = ui->newProjPath->text();
    if(fileRegex.match(projName).hasMatch()){
        QDir projDir(projPath + "/" + projName);
        if(QDir(projDir).exists()){
            displayError("A folder called '"+projName+"' already exists in that directory");
        }else{
            if(projDir.mkpath(".")){
                projDir.mkdir("components");
                QFile coordFile(projDir.path()+"/coordinates.smpcoords");
                if(coordFile.open(QIODevice::ReadWrite)){
                    QString coordsContent = "{\"frames\":[{}]}";
                    coordFile.write(coordsContent.toUtf8());
                    coordFile.close();
                }
                QFile projFile(projDir.path()+"/"+projName+".smpanim");
                if(projFile.open(QIODevice::ReadWrite)){
                    QString projContent = "{\"name\":\"" + projName + "\",\"framerate\":24,\"canvas_size\":{\"width\":300,\"height\":300},\"components\":{}}";
                    projFile.write(projContent.toUtf8());
                    projFile.close();
                }
                DBCon::DB()->addProject(projName, projPath);
                time_t t = time(nullptr);
                struct project proj = project(projPath,projName,t,true);
                EditScreen* es = new EditScreen(proj,cvsImg);
                this->close();
                es->show();
            }
        }
    }else{
        displayError("The project name contains invalid characters");
    }
}

void CreateProjDialog::toggleImageFind(){
    if(sender()->objectName().compare("newEmptyProj")){
        ui->chooseImage->setEnabled(true);
    }else if(sender()->objectName().compare("newProjWithImage")){
        ui->chooseImage->setEnabled(false);
    }
}

void CreateProjDialog::chooseImageInDialog(){
    QFileDialog* fd = new QFileDialog();
    QUrl imgUrl = fd->getOpenFileName(this,"Choose an existing image for your project",QString(QStandardPaths::PicturesLocation),"PNG (*.png);; JPEG (*.jpg);; GIF (*.gif)");
    if(!imgUrl.isEmpty()){
        if(gs->items().size() > 0){
            gs->removeItem(&gpi);
        }
        QPixmap img = QPixmap(imgUrl.url());
        if(img.height() > previewH){
            img = img.scaledToHeight(previewH);
        }
        gpi.setPixmap(img);
        gs->addItem(&gpi);
        cvsImg = new QImage(imgUrl.url());
    }
}

void CreateProjDialog::displayError(QString error){
    ui->createProjError->setText(error);
    ui->createProjError->setStyleSheet("QLabel#createProjError{color:#F00;}");
    ui->createProjError->show();
}
