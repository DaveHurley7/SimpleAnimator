#include "componentpainter.h"
#include <QDebug>

ComponentPainter::ComponentPainter(){}
ComponentPainter::~ComponentPainter(){}

void ComponentPainter::initPainter(QImage *img, QColor c){
    p.begin(img);
    p.setRenderHints(QPainter::Antialiasing);
    pen.setColor(c);
    p.setPen(pen);
}

void ComponentPainter::setPixelColour(QPoint *pt){
    p.drawPoint(*pt);
}

void ComponentPainter::closePainter(){
    p.end();
}


