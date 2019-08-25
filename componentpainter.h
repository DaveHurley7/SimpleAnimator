#ifndef COMPONENTPAINTER_H
#define COMPONENTPAINTER_H

#include <QPainter>

class ComponentPainter
{
public:
    ComponentPainter();
    ~ComponentPainter();
    void initPainter(QImage *img, QColor c);
    void setPixelColour(QPoint *pt);
    void closePainter();

private:
    QPainter p;
    QPen pen;
};

#endif // COMPONENTPAINTER_H
