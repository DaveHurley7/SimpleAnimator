#include <QDebug>
#include <QTimer>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFileDialog>
#include "dbconnection.h"
#include "startscreen.h"
#include "editscreen.h"
#include "ui_editscreen.h"
#include "animframe.h"
#include "component.h"
#include "previewdialog.h"
#include "exportframedialog.h"
#include "exportprojectdialog.h"
#include "JlCompress.h"
#include "pixmappainter.h"
#include "recentprojitem.h"
#include "projectiohandler.h"

EditScreen::EditScreen(project proj, QImage *img, QWidget *parent): QMainWindow(parent),
    ui(new Ui::EditScreen){
    ui->setupUi(this);
    ui->canvas->addHelper(&h);
    projDetails = proj;
    imgs = new QList<QPixmap*>;
    frames = new QList<AnimFrame*>;
    this->setWindowTitle("Simple Animator - "+proj.name+" - \""+proj.path+"\"");
    ui->timeLine->setStyleSheet("QWidget#timeLine{border-top:1px solid black;border-bottom:1px solid black;}");
    ui->frameScroll->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->panelFrame->setVisible(false);
    ui->propertyPanel->setVisible(false);
    ui->lumSld->setStyleSheet("QSlider::groove:vertical{background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #FFFFFF,stop:.5 #08ECF8,stop:1 #000000);width:6px;} QSlider::handle:vertical{border:3px solid #A1A1A1;border-radius:3px;height:6px;margin:0 -8px;}");
    ui->colourDisplay->setStyleSheet("background-color:#08ECF8");
    setupIcons();
    connectActions();
    connectColourModifiers();
    setupDefinedColours();
    if(ui->canvas->loadComponents(projDetails,&projInfo)){
        if(ui->canvas->components->size() > 0){
            ui->canvas->createPixmaps(imgs,projInfo.cvsSize);
        }
        if(imgs->size() > 0){
            ui->canvas->getHelper()->setImage(nullptr);
            for(int p = 0; p < imgs->size(); p++){
                addFrameToTimeLine(imgs->at(p),(p+1));
            }
        }else{
            QPixmap *pmp = new QPixmap;
            if(img==nullptr){
                ui->canvas->getHelper()->setImage(nullptr);
                pmp = new QPixmap(128,128);
                pmp->fill(Qt::white);
            }else{
                ui->canvas->getHelper()->setImage(img);
                *pmp = QPixmap::fromImage(*img);
            }
            imgs->append(pmp);
            addFrameToTimeLine(pmp);
        }
        ui->frameScroll->setCurrentRow(0);
        QTimer* t = new QTimer(this);
        connect(t,SIGNAL(timeout()),ui->canvas,SLOT(animate()));
        t->start(50);
    }else{
        this->close();
    }
}

EditScreen::~EditScreen(){
    delete ui;
}

void EditScreen::closeProject(){
    DBCon::DB()->closeProject(projDetails.path,projDetails.name);
    StartDialog* sd = new StartDialog();
    sd->show();
    this->close();
}

void EditScreen::saveProject(){
    ui->canvas->saveProject(projDetails.path+"/"+projDetails.name);
}

void EditScreen::closeEvent(QCloseEvent *ev){
    (void)ev;
    if(DBCon::DB()->otherProjsActive(projDetails.path,projDetails.name)){
        DBCon::DB()->closeProject(projDetails.path,projDetails.name);
        this->close();
    }
}

void EditScreen::addFrameToTimeLine(QPixmap *pmp, int index){
    AnimFrame *f = nullptr;
    if(pmp->height() > 128){
        f = new AnimFrame(pmp->scaledToHeight(128),index);
    }else{
        f = new AnimFrame(*pmp,index);
    }
    frames->append(f);
    QListWidgetItem *lwi = new QListWidgetItem();
    lwi->setSizeHint(f->sizeHint());
    ui->frameScroll->addItem(lwi);
    ui->frameScroll->setItemWidget(lwi,f);
}

void EditScreen::closePanel(){
    ui->propertyPanel->setVisible(false);
}

void EditScreen::preview(){
    PreviewDialog *pd = new PreviewDialog(imgs, projDetails.name, this);
    int res_code = pd->exec();
    if(res_code != -1 && res_code != ui->canvas->currentFrame){
        ui->canvas->currentFrame = res_code;
        ui->frameScroll->setCurrentRow(res_code);
    }
}

void EditScreen::setMode(){
    if(sender()->objectName().compare("moveTool")==0){
        ui->canvas->mode = ui->canvas->MOVE;
        if(ui->canvas->rotaterVisible){ui->canvas->rotaterVisible=false;}
        qDebug() << "MODE: Move";
    }else if(sender()->objectName().compare("selectTool")==0){
        ui->canvas->mode = ui->canvas->SELECT;
        if(ui->canvas->rotaterVisible){ui->canvas->rotaterVisible=false;}
    }else if(sender()->objectName().compare("editTool")==0){
        ui->canvas->mode = ui->canvas->EDIT;
        if(ui->panelFrame->isVisible()){
            ui->panelFrame->setVisible(false);
        }else{
            ui->panelFrame->setVisible(true);
        }
    }
}

void EditScreen::changeFrameNum(){
    ui->canvas->currentFrame = ui->frameScroll->currentRow();
    if(ui->propertyPanel->isVisible()){
        showPropertyPanel();
    }
}

void EditScreen::setShape(){
    if(sender()->objectName().compare("shapeSquare") == 0){
        if(ui->canvas->shape == ui->canvas->SQUARE){
            ui->canvas->shape = ui->canvas->NONE;
            ui->shapeSquare->setStyleSheet("background-color:transparent;border:0px");
        }else{
            ui->canvas->shape = ui->canvas->SQUARE;
            ui->shapeSquare->setStyleSheet("background-color:white;border:0px");
            ui->shapeCircle->setStyleSheet("background-color:transparent;border:0px");
        }
    }else if(sender()->objectName().compare("shapeCircle") == 0){
        if(ui->canvas->shape == ui->canvas->CIRCLE){
            ui->canvas->shape = ui->canvas->NONE;
            ui->shapeCircle->setStyleSheet("background-color:transparent;border:0px");
        }else{
            ui->canvas->shape = ui->canvas->CIRCLE;
            ui->shapeCircle->setStyleSheet("background-color:white;border:0px");
            ui->shapeSquare->setStyleSheet("background-color:transparent;border:0px");
        }
    }
}

void EditScreen::exportImage(){
    QString imgType = "";
    QString fileType = "";
    if(sender()->objectName().compare("makePng")==0){
        imgType = ".png";
        fileType = "PNG (*.png)";
    }else if(sender()->objectName().compare("makeJpg")==0){
        imgType = ".jpg";
        fileType = "JPG (*.jpg)";
    }
    int currRow = ui->frameScroll->currentRow();
    QString *imgUrl = new QString;
    QString defaultName = projDetails.name+"_frame"+QString::number(currRow+1)+imgType;
    ExportFrameDialog *efd = new ExportFrameDialog(imgUrl,defaultName,fileType,imgs->at(currRow));
    int res_code = efd->exec();
    if(res_code == QDialogButtonBox::Ok){
        imgs->at(currRow)->save(*imgUrl);
    }
}

void EditScreen::exportProject(){
    QString *projUrl = new QString;
    QString *exportType = new QString;
    QString *imgFormat = new QString;
    ExportProjectDialog *epd = new ExportProjectDialog(projUrl,exportType,imgFormat);
    int res_code = epd->exec();
    if(res_code == QDialogButtonBox::Ok){
        if(*exportType=="ZIP"){
            QFile *fZip = new QFile(*projUrl+"/"+projDetails.name+".zip");
            fZip->open(QIODevice::ReadWrite);
            fZip->close();
            QDir d(*projUrl);
            QString tempDir = "temp_"+projDetails.name;
            d.mkdir(tempDir);
            d.cd(tempDir);
            for(int i = 0; i < imgs->count(); i++){
                imgs->at(i)->save(d.path()+"/frame"+QString::number(i+1)+imgFormat);
            }
            JlCompress::compressDir(fZip->fileName(),d.path());
            for(int r = 0; r < imgs->count(); r++){
                d.remove("frame"+QString::number(r+1)+imgFormat);
            }
            d.cdUp();
            d.rmdir(tempDir);
        }else{
            QImage *ssheet = new QImage(projInfo.cvsSize.width()*ui->frameScroll->count(),projInfo.cvsSize.height(),QImage::Format_ARGB32);
            PixmapPainter::makeSpriteSheet(projInfo.cvsSize.width(),ssheet,imgs);
            ssheet->save(*projUrl+"/"+projDetails.name+imgFormat);
        }
    }
}

void EditScreen::setupIcons(){
    ui->addFrame->setIcon(QIcon(QPixmap(imgPath+"add.png")));
    ui->moveTool->setIcon(this, QPixmap(imgPath+"moveicon.png"));
    ui->selectTool->setIcon(this, QPixmap(imgPath+"selecticon.png"));
    ui->editTool->setIcon(this, QPixmap(imgPath+"editicon.png"));
    ui->previewTool->setIcon(this, QPixmap(imgPath+"previewicon.png"));
    ui->closePropertyPanel->setIcon(this,QPixmap(imgPath+"closeicon.png"));
    ui->shapeSquare->setIcon(this,QPixmap(imgPath+"shapesquare.png"));
    ui->shapeCircle->setIcon(this,QPixmap(imgPath+"shapecircle.png"));
}

void EditScreen::connectActions(){
    //Actions with signal clicked()
    connect(ui->addFrame,SIGNAL(clicked()),this,SLOT(addFrameToProject()));
    connect(ui->moveTool,SIGNAL(clicked()),this,SLOT(setMode()));
    connect(ui->selectTool,SIGNAL(clicked()),this,SLOT(setMode()));
    connect(ui->editTool,SIGNAL(clicked()),this,SLOT(setMode()));
    connect(ui->previewTool,SIGNAL(clicked()),this,SLOT(preview()));
    connect(ui->closePropertyPanel,SIGNAL(clicked()),this,SLOT(closePanel()));
    connect(ui->shapeSquare,SIGNAL(clicked()),this,SLOT(setShape()));
    connect(ui->shapeCircle,SIGNAL(clicked()),this,SLOT(setShape()));

    //Actions with signal triggered()
    connect(ui->makePng,SIGNAL(triggered()),this,SLOT(exportImage()));
    connect(ui->makeJpg,SIGNAL(triggered()),this,SLOT(exportImage()));
    connect(ui->exportProject,SIGNAL(triggered()),this,SLOT(exportProject()));
    connect(ui->itemClose,SIGNAL(triggered()),this,SLOT(closeProject()));
    connect(ui->itemOpen,SIGNAL(triggered()),this,SLOT(openProject()));
    connect(ui->itemNew,SIGNAL(triggered()),this,SLOT(openProject()));
    connect(ui->itemSave,SIGNAL(triggered()),this,SLOT(saveProject()));
    connect(ui->rmFrame,SIGNAL(triggered()),this,SLOT(removeFrame()));
    connect(ui->menuOpenRecent,SIGNAL(triggered(QAction*)),this,SLOT(openRecentProject(QAction*)));

   //Actions with other built-in signals
    connect(ui->menuFile,SIGNAL(aboutToShow()),this,SLOT(setItemFocus()));
    connect(ui->frameScroll,SIGNAL(itemSelectionChanged()),this,SLOT(changeFrameNum()));
    connect(ui->lumSld,SIGNAL(valueChanged(int)),ui->colourPalette,SLOT(changeLuminosity(int)));
    connect(ui->hueSpb,SIGNAL(valueChanged(int)),ui->colourPalette,SLOT(changeHue(int)));
    connect(ui->satSpb,SIGNAL(valueChanged(int)),ui->colourPalette,SLOT(changeSat(int)));
    connect(ui->lumSpb,SIGNAL(valueChanged(int)),ui->colourPalette,SLOT(changeLum(int)));
    connect(ui->redSpb,SIGNAL(valueChanged(int)),ui->colourPalette,SLOT(changeRed(int)));
    connect(ui->greenSpb,SIGNAL(valueChanged(int)),ui->colourPalette,SLOT(changeGreen(int)));
    connect(ui->blueSpb,SIGNAL(valueChanged(int)),ui->colourPalette,SLOT(changeBlue(int)));
    connect(&cBG,SIGNAL(buttonClicked(int)),ui->colourPalette,SLOT(getDefinedColour(int)));

    //Actions with custom signals
    connect(ui->canvas,SIGNAL(shapeReleased()),this,SLOT(deselectShapes()));
    connect(ui->canvas,SIGNAL(frameUpdated()),this,SLOT(updateFrame()));
    connect(ui->canvas,SIGNAL(openPropPanel()),this,SLOT(showPropertyPanel()));
    connect(ui->canvas,SIGNAL(closePropPanel()),this,SLOT(closePanel()));
}

void EditScreen::connectColourModifiers(){
    ui->colourPalette->dC = ui->canvas->colour;
    ui->colourPalette->hS = ui->hueSpb;
    ui->colourPalette->sS = ui->satSpb;
    ui->colourPalette->lS = ui->lumSpb;
    ui->colourPalette->rS = ui->redSpb;
    ui->colourPalette->bS = ui->blueSpb;
    ui->colourPalette->gS = ui->greenSpb;
    ui->colourPalette->lSld = ui->lumSld;
    ui->colourPalette->oSld = ui->opaSld;
    ui->colourPalette->display = ui->colourDisplay;
    ui->colourPalette->colourGroup = &cBG;
}

void EditScreen::setItemFocus(){
    if(ui->canvas->currComp==-1){
        ui->rmComp->setEnabled(false);
    }else{
        ui->rmComp->setEnabled(true);
    }
    getListOfRecentProjects();
}

void EditScreen::deselectShapes(){
    ui->shapeSquare->setStyleSheet("background-color:transparent;border:0px");
    ui->shapeCircle->setStyleSheet("background-color:transparent;border:0px");
}

void EditScreen::updateFrame(){
    int currFrame = ui->canvas->currentFrame;
    QPixmap *uFrame = new QPixmap(projInfo.cvsSize);
    uFrame->fill(Qt::white);
    ui->canvas->updatePixmap(uFrame);
    imgs->replace(currFrame,uFrame);
    if(uFrame->height() > 128){
        frames->at(currFrame)->gpi->setPixmap(uFrame->scaledToHeight(128));
    }else{
        frames->at(currFrame)->gpi->setPixmap(*uFrame);
    }
    ui->frameScroll->setItemWidget(ui->frameScroll->item(currFrame),frames->at(currFrame));
}

void EditScreen::showPropertyPanel(){
    CompType *compType = ui->canvas->components->at(ui->canvas->currComp);
    CompInfo *compInfo = ui->canvas->frames->at(ui->canvas->currentFrame).components->at(ui->canvas->currComp);
    ui->shapeName->setText(*(compType->name));
    ui->XCoord->setValue(compInfo->coord.x());
    ui->YCoord->setValue(compInfo->coord.y());
    ui->angleDisplay->setValue(compInfo->angle);
    if(!ui->propertyPanel->isVisible()){
        ui->propertyPanel->setVisible(true);
    }
}

void EditScreen::addFrameToProject(){
    imgs->append(new QPixmap(imgs->last()->copy()));
    addFrameToTimeLine(imgs->last(),imgs->count());
    ui->canvas->frames->append(ui->canvas->frames->last().copy());
}

void EditScreen::openProject(){
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
            if(ui->canvas->saved){
                if(sender()->objectName().compare("itemNew") == 0){
                    if(!ui->canvas->newProject()){
                        this->close();
                    }
                }else{
                    if(ui->canvas->openProject(this)){
                        this->close();
                    }
                }
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
            if(sender()->objectName().compare("itemNew") == 0){
                ui->canvas->newProject();
            }else{
                ui->canvas->openProject(this);
            }
        }
    }
}

void EditScreen::removeFrame(){
    int rIndex = ui->canvas->currentFrame;
    imgs->removeAt(rIndex);
    frames->removeAt(rIndex);
    int prevFrame = 0;
    for(AnimFrame *f : *frames){
        if(prevFrame+1!=f->frameNum){
            f->frameNum--;
            f->frameCountLabel->setText(QString::number(f->frameNum));
        }
        prevFrame++;
    }
    ui->frameScroll->takeItem(rIndex);
}

void EditScreen::getListOfRecentProjects(){
    ui->menuOpenRecent->clear();
    projList.clear();
    projList = DBCon::DB()->getRecentProjList();
    if(projList.count() > 0){
        for(rpItem i : projList){
            ui->menuOpenRecent->addAction(i.name);
        }
    }
}

void EditScreen::openRecentProject(QAction *a){
    ProjectIOHandler::openRecentProject(this,ui->canvas->saved,projList.at(ui->menuOpenRecent->actions().indexOf(a)));
}

void EditScreen::setupDefinedColours(){
    cBG.addButton(ui->dcRed,1);
    cBG.addButton(ui->dcBlue,2);
    cBG.addButton(ui->dcGreen,3);
    cBG.addButton(ui->dcOrange,4);
    cBG.addButton(ui->dcYellow,5);
    cBG.addButton(ui->dcBrown,6);
    cBG.addButton(ui->dcPurple,7);
    cBG.addButton(ui->dcPink,8);
    cBG.addButton(ui->dcTurquoise,9);
    cBG.addButton(ui->dcWhite,10);
    cBG.addButton(ui->dcBlack,11);
    cBG.addButton(ui->dcDarkGray,12);
    cBG.addButton(ui->dcGray,13);
    cBG.addButton(ui->dcLightGray,14);
    cBG.addButton(ui->dcDarkRed,15);
    cBG.addButton(ui->dcDarkBlue,16);
    cBG.addButton(ui->dcDarkGreen,17);
    cBG.addButton(ui->dcLightBlue,18);
    cBG.addButton(ui->dcCustom1,19);
    cBG.addButton(ui->dcCustom2,20);
    cBG.addButton(ui->dcCustom3,21);
    cBG.addButton(ui->dcCustom4,22);
    cBG.addButton(ui->dcCustom5,23);
    cBG.addButton(ui->dcCustom6,24);
    cBG.addButton(ui->dcCustom7,25);
    cBG.addButton(ui->dcCustom8,26);
    cBG.addButton(ui->dcCustom9,27);
}
