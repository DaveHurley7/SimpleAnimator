#include "canvasHelper.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPen>
#include <QWidget>
#include <QDebug>

CanvasHelper::CanvasHelper(){
    shapePen.setColor(Qt::black);
    shapePen.setWidth(1);
    highlightPen.setColor(highlight);
    highlightPen.setWidth(3);
    boneBrush.setColor(Qt::red);
    bonePen.setColor(Qt::red);
}

void CanvasHelper::startPainter(Canvas *cvs){
    p.begin(cvs);
    p.setPen(stdPen);
    p.setRenderHints(QPainter::SmoothPixmapTransform);
}

void CanvasHelper::stopPainter(){
    p.end();
}

void CanvasHelper::setImage(QImage* image){
    img = image;
}

void CanvasHelper::addImage(){
    p.drawImage(0,0,*img);
}

void CanvasHelper::fillBG(int w, int h){
    p.fillRect(0,0,w,h,QBrush(Qt::white));
}

void CanvasHelper::drawComponent(CompInfo *c, QImage img){
    p.translate(c->coord);
    p.rotate(-(c->angle));
    p.translate(-(img.width()/2),-(img.height()/2));
    p.drawImage(0,0,img);
    p.translate(img.width()/2,img.height()/2);
    p.rotate(c->angle);
    p.translate(-(c->coord));
}

void CanvasHelper::drawShape(Canvas::AddShape s){
    p.setPen(shapePen);
    if(s.type == "rect"){
        if(s.r.w!=0&&s.r.h!=0){
            p.drawRect(QRect(s.r.rX,s.r.rY,s.r.w,s.r.h));
        }
    }else if(s.type == "circle"){
        if(s.c.rad!=0){
            QRect r(s.c.pX,s.c.pY,s.c.s,s.c.s);
            p.drawArc(r,0,5760);
        }
    }
}

void CanvasHelper::highlightComponent(CompInfo i, QSize s, QString *t){
    p.setPen(highlightPen);
    int x = -(s.width()/2 + 2);
    int y = -(s.height()/2 + 2);
    p.translate(i.coord);
    p.rotate(-i.angle);
    p.translate(x,y);
    if(*t=="rect"){
        p.drawRect(0,0,s.width()+2,s.height()+2);
    }else if(*t=="circle"){
        p.drawArc(0,0,s.width()+2,s.height()+2,0,5760);
    }
    p.translate(-x,-y);
    p.rotate(i.angle);
    p.translate(-i.coord);
}

void CanvasHelper::showRotater(QPoint r){
    int x = r.x() - (rotImg.width()/2);
    int y = r.y() - (rotImg.height()/2);
    p.drawImage(x,y,rotImg);

}

void CanvasHelper::showPoints(QList<QPoint> ps){
    for(QPoint pt : ps){
        p.drawPoint(pt);
    }
}

void CanvasHelper::showBone(QPoint pt){
    p.setBrush(boneBrush);
    p.setPen(bonePen);
    p.setPen(QPen(Qt::red));
    p.setBrush(QBrush(Qt::red));
    p.drawEllipse(pt.x()-3,pt.y()-3,6,6);
}

void CanvasHelper::drawPoint(QPoint pt){
    p.drawPoint(pt);
}

void CanvasHelper::setupPen(QColor c){
    drawerPen.setColor(c);
    p.setPen(drawerPen);
}
