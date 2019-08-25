#include "animframe.h"
#include <QDebug>

AnimFrame::AnimFrame(QPixmap p, int frameNum){
    layout = new QVBoxLayout();
    view = new QGraphicsView();
    scene = new QGraphicsScene(layout);
    gpi = new QGraphicsPixmapItem(p);

    view->setScene(scene);
    scene->addItem(gpi);

    this->frameNum = frameNum;
    frameCountLabel = new QLabel(QString::number(frameNum));

    layout->addWidget(view);
    layout->addWidget(frameCountLabel);
    frameCountLabel->setAlignment(Qt::AlignCenter);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    this->setLayout(layout);
}

AnimFrame::~AnimFrame(){
}

void AnimFrame::mouseEventPress(){
    emit clicked();
}
