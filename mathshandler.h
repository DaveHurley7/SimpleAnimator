#ifndef MATHSHANDLER_H
#define MATHSHANDLER_H

#include <QPoint>
#include <QList>


class MathsHandler
{
public:
    MathsHandler();
    static QPoint rotatePoint(QPoint pivot, QPoint p, int a);
};

#endif // MATHSHANDLER_H
