#ifndef PIXMAPPAINTER_H
#define PIXMAPPAINTER_H

#include <QPixmap>
#include <QImage>
#include <QPainter>
#include "component.h"
#include "comptype.h"

class PixmapPainter
{
public:
    PixmapPainter();
    static void addPixmap(QPixmap *p, QList<CompInfo*> *compInfos, QList<CompType*> cList);
    static void makeSpriteSheet(int frameW, QImage *i, QList<QPixmap*> *p);

private:
    static QPainter pt;
};

#endif // PIXMAPPAINTER_H
