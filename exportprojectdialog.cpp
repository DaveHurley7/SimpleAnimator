#include "exportprojectdialog.h"
#include "ui_exportprojectdialog.h"
#include <QDebug>
#include <QFileDialog>

ExportProjectDialog::ExportProjectDialog(QString *p, QString *t, QString *f, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportProjectDialog){
    ui->setupUi(this);
    this->setWindowTitle("Export Project");
    bgT.addButton(ui->exportZip);
    bgT.addButton(ui->exportSpritesheet);
    bgF.addButton(ui->fmtPNG);
    bgF.addButton(ui->fmtJPG);
    ui->exportZip->setChecked(true);
    ui->fmtPNG->setChecked(true);
    exportUrl = p;
    exportType = t;
    format = f;
    recentUrl = defaultUrl;
    ui->exportPath->setText(defaultUrl);
    connect(ui->browsePath,SIGNAL(clicked()),this,SLOT(setPath()));
}

ExportProjectDialog::~ExportProjectDialog(){
    delete ui;
}

void ExportProjectDialog::accept(){
    *exportUrl = ui->exportPath->text();
    if(ui->exportZip->isChecked()){
        *exportType = QString("ZIP");
    }else{
        *exportType = QString("Spritesheet");
    }
    if(ui->fmtPNG->isChecked()){
        *format = ".png";
    }else{
        *format = ".jpg";
    }
    this->done(QDialogButtonBox::Ok);
}

void ExportProjectDialog::reject(){
    this->done(QDialogButtonBox::Cancel);
}

void ExportProjectDialog::setPath(){
    QString imgUrl = QFileDialog::getExistingDirectory(this,"Choose directory to export project",defaultUrl,QFileDialog::ShowDirsOnly);
    if(imgUrl == ""){
        ui->exportPath->setText(recentUrl);
    }else{
        ui->exportPath->setText(imgUrl);
    }
}
