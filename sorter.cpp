#include "sorter.h"
#include "mathshandler.h"
#include <QDebug>
Sorter::Sorter(){}

void Sorter::sortPoints(QList<QPoint> *pList){
    int pLen = pList->count();
    quickSort(pList,0,pLen-1);
    int start = 0;
    int currY = pList->first().y();
    for(int p = 1; p < pLen; p++){
        if(pList->at(p).y()!=currY){
            if(start!=p-1){
                if(start+1==p-1){
                    if(pList->at(start).x()>pList->at(p-1).x()){
                        temp = pList->at(p-1);
                        pList->replace(p-1,pList->at(start));
                        pList->replace(start,temp);
                    }
                }else{
                    insertionSort(pList,start,p-1);
                }
            }
            start = p;
            currY = pList->at(p).y();
        }
    }
}

void Sorter::quickSort(QList<QPoint> *pList, int start, int end){
    int i = start, j = end-1;
    int p = (start+end)/2;
    QPoint pivot = pList->at(p);
    if(i==j){
        if(pivot.y() > pList->at(end).y()){
            temp=pList->at(end);
            pList->replace(end,pivot);
            pList->replace(i,temp);
        }
    }else{
        pList->append(pivot);
        pList->removeAt(p);
        while(i<j){
            while(i < end && pList->at(i).y()<pivot.y()){
                i++;
            }
            while(j >= start && pList->at(j).y()>pivot.y()){
                j--;
            }
            if(i<j){
                temp=pList->at(i);
                pList->replace(i,pList->at(j));
                pList->replace(j,temp);
                i++;
            }
        }
        if(i!=pList->indexOf(pivot)){
            pList->insert(i,pivot);
            pList->pop_back();
            i++;
        }
        if(start < j){
            quickSort(pList,start,j);
        }
        if(i <  end){
            quickSort(pList,i,end);
        }
    }
}

void Sorter::insertionSort(QList<QPoint> *pList, int start, int end){
    for(int p = start+1; p <= end; p++){
        int i = p-1;
        int x = pList->at(p).x();
        while(i >= start && pList->at(i).x() > x){
            i--;
        }
        i++;
        if(i<p){
            pList->insert(i,pList->at(p));
            pList->removeAt(p+1);
        }
    }
}
QPoint Sorter::temp;
