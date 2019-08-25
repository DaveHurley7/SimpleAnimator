#include <QFileDialog>
#include <QDebug>
#include "exportframedialog.h"
#include "ui_exportframedialog.h"

ExportFrameDialog::ExportFrameDialog(QString *u, QString n, QString fT, QPixmap *p, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportFrameDialog){
    ui->setupUi(this);
    this->setWindowTitle("Export Frame As Image");
    gs = new QGraphicsScene(ui->imagePreview);
    gpi = new QGraphicsPixmapItem(*p);
    gs->addItem(gpi);
    ui->imagePreview->setScene(gs);
    ui->imgPath->setText(defaultUrl+n);
    connect(ui->pathBrowse,SIGNAL(clicked()),this,SLOT(setPath()));
    recentUrl = defaultUrl;
    exportUrl = u;
    acceptedType = fT;
}

ExportFrameDialog::~ExportFrameDialog(){
    delete ui;
}

void ExportFrameDialog::setPath(){
    QFileDialog *fd = new QFileDialog;
    QString imgUrl = fd->getSaveFileName(this,"Choose directory to save image",defaultUrl, acceptedType);
    if(imgUrl == ""){
        ui->imgPath->setText(recentUrl);
    }else{
        ui->imgPath->setText(imgUrl);
    }
}

void ExportFrameDialog::accept(){
    *exportUrl = ui->imgPath->text();
    this->done(QDialogButtonBox::Ok);
}

void ExportFrameDialog::reject(){
    this->done(QDialogButtonBox::Cancel);
}
