#include "previewdialog.h"
#include "ui_previewdialog.h"
#include <QTimer>
#include <QDebug>

PreviewDialog::PreviewDialog(QList<QPixmap*> *imgs, QString projName, QWidget *parent): QDialog(parent), ui(new Ui::PreviewDialog){
    ui->setupUi(this);
    this->setWindowTitle("Preview of "+projName);
    initialFrame = new QAction("Last frame before preview",this);
    currentFrame = new QAction("Current frame",this);
    initialFrame->setObjectName("initialFrame");
    currentFrame->setObjectName("currentFrame");
    buttonMenu = new QMenu;
    buttonMenu->addAction(initialFrame);
    buttonMenu->addAction(currentFrame);
    ui->exitPreview->setMenu(buttonMenu);
    ui->exitPreview->setDefaultAction(initialFrame);
    ui->exitPreview->setText("Back");
    frameNum = 0;
    isPaused = true;
    images = imgs;
    gs = new QGraphicsScene;
    gpi = new QGraphicsPixmapItem;
    gs->addItem(gpi);
    ui->scene->setScene(gs);
    gpi->setPixmap(*images->at(frameNum));
    ui->playPause->setIcon(this,QPixmap(imgPath+"playmedia.png"));
    ui->stop->setIcon(this,QPixmap(imgPath+"stopmedia.png"));
    ui->skipBack->setIcon(this,QPixmap(imgPath+"skipbackframe.png"));
    ui->skipForward->setIcon(this,QPixmap(imgPath+"skipforwardframe.png"));
    connect(initialFrame,SIGNAL(triggered()),this,SLOT(stopPreview()));
    connect(currentFrame,SIGNAL(triggered()),this,SLOT(stopPreview()));
    connect(ui->playPause,SIGNAL(clicked()),this,SLOT(playMedia()));
    connect(ui->stop,SIGNAL(clicked()),this,SLOT(stopMedia()));
    connect(ui->skipBack,SIGNAL(clicked()),this,SLOT(skipBackFrame()));
    connect(ui->skipForward,SIGNAL(clicked()),this,SLOT(skipForwardFrame()));
    QTimer *t = new QTimer(this);
    connect(t,SIGNAL(timeout()),this,SLOT(display()));
    t->start(1000/24);
}

PreviewDialog::~PreviewDialog()
{
    delete ui;
}

void PreviewDialog::stopPreview(){
    if(sender()->objectName().compare("initialFrame") == 0){
        this->done(-1);
    }else if(sender()->objectName().compare("currentFrame") == 0){
        this->done(frameNum);
    }
}

void PreviewDialog::display(){
    if(!isPaused){
        gpi->setPixmap(*images->at(frameNum));
        frameNum++;
        if(frameNum >= images->size()){
            frameNum = 0;
        }
    }
}

void PreviewDialog::playMedia(){
    if(isPaused){
        isPaused = false;
        ui->playPause->setIcon(this,QPixmap(imgPath+"pausemedia.png"));
        qDebug() << "Media is playing";
    }else{
        isPaused = true;
        ui->playPause->setIcon(this,QPixmap(imgPath+"playmedia.png"));
        qDebug() << "Media paused";
    }
}

void PreviewDialog::stopMedia(){
    isPaused = true;
    frameNum = 0;
    gpi->setPixmap(*images->at(frameNum));
    qDebug() << "Media stopped";
}

void PreviewDialog::skipBackFrame(){
    frameNum--;
    if(frameNum < 0){
        frameNum = images->size()-1;
    }
    gpi->setPixmap(*images->at(frameNum));
    qDebug() << "Skipped back a frame";
}

void PreviewDialog::skipForwardFrame(){
    frameNum++;
    if(frameNum >= images->size()){
        frameNum = 0;
    }
    gpi->setPixmap(*images->at(frameNum));
    qDebug() << "Skipped forward a frame";
}
