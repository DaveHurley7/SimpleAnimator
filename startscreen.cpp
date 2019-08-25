#include <QFileDialog>
#include <QDebug>
#include "startscreen.h"
#include "ui_startscreen.h"
#include "createprojwindow.h"
#include "editscreen.h"
#include "projectiohandler.h"

StartDialog::StartDialog(QWidget *parent) :QDialog(parent),ui(new Ui::StartDialog){
    ui->setupUi(this);
    connect(ui->projCreate,SIGNAL(clicked()),this,SLOT(createProj()));
    connect(ui->projOpen,SIGNAL(clicked()),this,SLOT(openProj()));
}

StartDialog::~StartDialog(){
    delete ui;
}

void StartDialog::createProj(){
    ProjectIOHandler::newProject();
    this->close();
}

void StartDialog::openProj(){
    if(ProjectIOHandler::openProject(this)){
        this->close();
    }
}
