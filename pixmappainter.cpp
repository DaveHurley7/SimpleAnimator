#include "pixmappainter.h"

PixmapPainter::PixmapPainter(){}    

void PixmapPainter::addPixmap(QPixmap *p, QList<CompInfo*> *compInfos, QList<CompType*> cList){
    pt.begin(p);
    pt.setRenderHint(QPainter::SmoothPixmapTransform);
    for(CompInfo *c : *compInfos){
        int hW = cList.at(c->cIndex)->img->width()/2;
        int hH = cList.at(c->cIndex)->img->height()/2;
        pt.translate(c->coord);
        pt.rotate(-(c->angle));
        pt.translate(-(hW),-(hH));
        pt.drawImage(0,0,*(cList.at(c->cIndex)->img));
        pt.translate(hW,hH);
        pt.rotate(c->angle);
        pt.translate(-(c->coord));
    }
    pt.end();
}

void PixmapPainter::makeSpriteSheet(int frameW, QImage *i, QList<QPixmap*> *p){
    pt.begin(i);
    pt.setRenderHint(QPainter::SmoothPixmapTransform);
    int x = 0;
    for(int f = 0; f < p->count(); f++){
        pt.drawPixmap(x,0,*(p->at(f)));
        x += frameW;
    }
    pt.end();
}


QPainter PixmapPainter::pt;
