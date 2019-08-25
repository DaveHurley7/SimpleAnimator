#ifndef CANVAS_HELPER_H
#define CANVAS_HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include <vector>
#include <utility>
#include <QPoint>
#include <QImage>
#include <QPainter>
#include "canvas.h"

class CanvasHelper
{
public:
    CanvasHelper();
    void startPainter(Canvas *cvs);
    void stopPainter();
    void setImage(QImage* img);
    void fillBG(int w, int h);
    void addImage();
    void drawComponent(CompInfo *c, QImage img);
    void drawShape(struct Canvas::AddShape s);
    QImage* img;
    void highlightComponent(CompInfo i, QSize s, QString *t);
    void showRotater(QPoint r);
    void showPoints(QList<QPoint> ps);
    void showBone(QPoint pt);
    void drawPoint(QPoint pt);
    void setupPen(QColor c);
    QPen drawerPen;

private:
    QPainter p;
    QPen stdPen, shapePen, highlightPen, bonePen; //, drawerPen;
    QColor highlight = QColor(57,227,7);
    QImage rotImg = QImage(":/resources/images/rotater.png");
    QBrush boneBrush;
};

#endif // CANVAS_HELPER_H
