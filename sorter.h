#ifndef SORTER_H
#define SORTER_H

#include <QList>
#include <QPoint>

class Sorter
{
public:
    Sorter();
    static void sortPoints(QList<QPoint> *pList);
private:
    static void quickSort(QList<QPoint> *pList, int start, int end);
    static void insertionSort(QList<QPoint> *pList, int start, int end);
    static QPoint temp;
};

#endif // SORTER_H
