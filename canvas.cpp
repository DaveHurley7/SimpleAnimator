#include <QDebug>
#include <QPainter>
#include <QLine>
#include <QPoint>
#include <cmath>
#include "canvas.h"
#include "canvasHelper.h"
#include "projectiohandler.h"
#include "project.h"
#include "pixmappainter.h"
#include "mathshandler.h"
#include "math.h"
#include "lineeq.h"
#include "sorter.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent){
    this->setStyleSheet("background-color:white;");
    currentFrame = 0;
    cp = new ComponentPainter;
    frames = new QList<Frame>;
    components = new QList<CompType*>;
    mode = MOVE;
}

void Canvas::addHelper(CanvasHelper *helper){
    h = helper;
}

bool Canvas::loadComponents(struct project projDetails, struct projectinfo *projInfo){
    return ProjectIOHandler::load(projDetails.name,projDetails.path+"/"+projDetails.name,frames,components,projInfo);
}

void Canvas::createPixmaps(QList<QPixmap*> *a, QSize s){
    for(Frame f : *frames){
        QPixmap *pmp = new QPixmap(s);
        pmp->fill(Qt::white);
        PixmapPainter::addPixmap(pmp,f.components,*(components));
        a->append(pmp);
    }
}

void Canvas::updatePixmap(QPixmap* p){
    PixmapPainter::addPixmap(p,frames->at(currentFrame).components,*(components));
}

CanvasHelper* Canvas::getHelper(){
    return h;
}

void Canvas::mousePressEvent(QMouseEvent* ev){
    QWidget::mousePressEvent(ev);
    if(mode==EDIT){
        if(shape==SQUARE){
            addShape.r.iX = ev->pos().x();
            addShape.r.iY = ev->pos().y();
            addShape.type = "rect";
        }else if(shape==CIRCLE){
            addShape.c.x = ev->pos().x();
            addShape.c.y = ev->pos().y();
            addShape.type = "circle";
        }else{
            point = ev->pos();
            if(checkInComponent()){
                QPoint compPt = frames->at(currentFrame).components->at(currComp)->coord;
                if(ev->button()==Qt::RightButton){
                    if(!rotaterVisible){
                        moving = true;
                        offset = QPoint(ev->pos().x()-compPt.x(),ev->pos().y()-compPt.y());
                    }
                }else{
                    if(rotaterVisible && ev->button()==Qt::LeftButton){
                        rotating = true;
                        int compAngle = frames->at(currentFrame).components->at(currComp)->angle;
                        if(compPt==point){
                            baseAngle = 0;
                        }else{
                            baseAngle = int(atan2(-(point.y()-compPt.y()),point.x()-compPt.x())*(180/M_PI));
                            if(baseAngle<0){baseAngle+=360;}
                        }
                        currAngle = compAngle;
                    }else{
                        if(prevComp == currComp){
                            drawing = true;
                            cp->initPainter(components->at(currComp)->img, *colour);
                            QPoint *newP = new QPoint();
                            QSize sz = components->at(currComp)->img->size();
                            if(inComponent(point,frames->at(currentFrame).components->at(currComp),sz,newP)){
                                newP->rx()+=sz.width()/2;
                                newP->ry()+=sz.height()/2;
                                if(sz.width()%2==1){newP->rx()++;}
                                if(sz.height()%2==1){newP->ry()++;}
                                cp->setPixelColour(newP);
                            }
                        }
                    }
                }
            }else{
                if(ev->button()==Qt::LeftButton){
                    drawing = true;
                    int cSize = drawnPoints.size();
                    currColour = cSize;
                    for(int cp = 0; cp < cSize; cp++){
                        if(drawnPoints[cp].first==*colour){
                            drawnPoints[cp].second.append(point);
                            currColour = cp;
                            break;
                        }
                    }
                    if(currColour==cSize){
                        drawnPoints.append(QPair<QColor,QList<QPoint>>(*colour,QList<QPoint>({point})));
                    }
                }
            }
        }
    }else if(mode==SELECT){
        selectPoints.append(ev->pos());
        shapeSelect.l = shapeSelect.r = ev->pos().x();
        shapeSelect.t = shapeSelect.b = ev->pos().y();
    }
}

void Canvas::mouseMoveEvent(QMouseEvent* ev){
    QWidget::mouseMoveEvent(ev);
    if(mode==EDIT){
        if(shape==SQUARE){
            int tW = ev->pos().x() - addShape.r.iX;
            int tH = ev->pos().y() - addShape.r.iY;
            if(tW < 0){
                addShape.r.rX = addShape.r.iX + tW;
                addShape.r.w = -tW;
            }else{
                addShape.r.rX = addShape.r.iX;
                addShape.r.w = tW;
            }
            if(tH < 0){
                addShape.r.rY = addShape.r.iY + tH;
                addShape.r.h = -tH;
            }else{
                addShape.r.rY = addShape.r.iY;
                addShape.r.h = tH;
            }
        }else if(shape==CIRCLE){
            addShape.c.rad = int(std::pow(std::pow(addShape.c.x - ev->pos().x(),2)+std::pow(addShape.c.y - ev->pos().y(),2),0.5));
            addShape.c.pX = addShape.c.x - addShape.c.rad;
            addShape.c.pY = addShape.c.y - addShape.c.rad;
            addShape.c.s = addShape.c.rad*2;
        }else{
            if(currComp >= 0){
                CompInfo *comp = frames->at(currentFrame).components->at(currComp);
                if(rotating){
                    angleChange = int(atan2(-(ev->pos().y()-comp->coord.y()),ev->pos().x()-comp->coord.x())*(180/M_PI));
                    if(angleChange<0){angleChange+=360;}
                    comp->angle = currAngle + (angleChange-baseAngle);
                }else if(moving){
                    comp->coord = ev->pos() - offset;
                    emit openPropPanel();
                }else if(drawing){
                    point = ev->pos();
                    QPoint *newP = new QPoint();
                    QSize sz = components->at(currComp)->img->size();
                    if(inComponent(point,frames->at(currentFrame).components->at(currComp),sz,newP)){
                        newP->rx()+=sz.width()/2;
                        newP->ry()+=sz.height()/2;
                        if(sz.width()%2==1){newP->rx()++;}
                        if(sz.height()%2==1){newP->ry()++;}
                        cp->setPixelColour(newP);
                    }
                }
            }else{
                if(drawing){
                    QPoint point = ev->pos();
                    drawnPoints[currColour].second.append(point);
                }
            }
        }
    }else if(mode==SELECT){
        QPoint currPt = ev->pos();
        if(!selectPoints.contains(currPt)){
            selectPoints.append(currPt);
            if(currPt.x() < shapeSelect.l){
                shapeSelect.l = currPt.x();
            }else if(currPt.x() > shapeSelect.r){
                shapeSelect.r = currPt.x();
            }
            if(currPt.y() < shapeSelect.t){
                shapeSelect.t = currPt.y();
            }else if(currPt.y() > shapeSelect.b){
                shapeSelect.b = currPt.y();
            }
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent* ev){
    QWidget::mouseReleaseEvent(ev);
    if(mode==EDIT){
        if(shape != NONE && ((ev->x() != addShape.r.iX || addShape.c.x) && (ev->y() != addShape.r.iY || addShape.c.y))){
            QImage *i = nullptr;
            QPoint p;
            if(shape==SQUARE){
                i = new QImage(addShape.r.w+1,addShape.r.h+1,QImage::Format_ARGB32);
                p = QPoint(addShape.r.rX+(addShape.r.w/2),addShape.r.rY+(addShape.r.h/2));
                if(addShape.r.w%2==1){p.rx()++;}
                if(addShape.r.h%2==1){p.ry()++;}
            }else if(shape==CIRCLE){
                i = new QImage(addShape.c.s,addShape.c.s,QImage::Format_ARGB32);
                p = QPoint(addShape.c.x,addShape.c.y);
            }
            if(i != nullptr){
                createComponent(i,p);
            }
        }else{
            if(point == ev->pos()){
                if(checkInComponent()){
                    if(ev->button() == Qt::RightButton){
                        if((currComp == prevComp) || (prevComp == -1 && !rotaterVisible)){
                            rotaterVisible = !rotaterVisible;
                        }else if((currComp != prevComp) && rotaterVisible == false){
                            rotaterVisible = true;
                        }
                        if(moving){
                            moving=false;
                        }
                    }else{
                        if(drawing){
                            drawing = false;
                            cp->closePainter();
                            emit frameUpdated();
                            saved = false;
                        }
                    }
                }else{
                    currComp = -1;
                    prevComp = -1;
                    emit closePropPanel();
                    rotaterVisible = false;
                }
            }else{
                if(currComp >= 0){
                    components->at(currComp)->modified = true;
                    if(moving){moving = false;generateBone();}
                    if(drawing){drawing = false;cp->closePainter();}
                    if(rotating){rotating = false;generateBone();}
                    saved = false;
                    emit frameUpdated();
                }else{
                    if(drawing){drawing = false;}
                }
            }
        }
    }else if(mode==SELECT){
        QList<QPoint> genPoints;
        QPoint initP = selectPoints.first();
        QPoint finalP = selectPoints.last();
        if(initP!=ev->pos()){
            double xDist = initP.x() - finalP.x();
            double yDist = initP.y() - finalP.y();
            LineEq closingLine = {yDist/xDist,initP};
            QPoint p;
            if(abs(xDist) >= abs(yDist)){
                int lastX = initP.x();
                if(xDist < 0){
                    for(int x = finalP.x()-1; x > lastX; x--){
                        int y = closingLine.calcY(x);
                        if(!selectPoints.contains(p = QPoint(x,y))){genPoints.append(p);}
                    }
                }else{
                    for(int x = finalP.x()+1; x < lastX; x++){
                        int y = closingLine.calcY(x);
                        if(!selectPoints.contains(p = QPoint(x,y))){genPoints.append(p);}
                    }
                }
            }else{
                int lastY = initP.y();
                if(yDist < 0){
                    for(int y = finalP.y()-1; y > lastY; y--){
                        int x = closingLine.calcX(y);
                        if(!selectPoints.contains(p = QPoint(x,y))){genPoints.append(p);}
                    }
                }else{
                    for(int y = finalP.y()+1; y < lastY; y++){
                        int x = closingLine.calcX(y);
                        if(!selectPoints.contains(p = QPoint(x,y))){genPoints.append(p);}
                    }
                }
            }
        }
        if(genPoints.count()){
            selectPoints.append(genPoints);
            genPoints.clear();
            qDebug() << selectPoints;
            Sorter::sortPoints(&selectPoints);
            qDebug() << selectPoints;
        }
    }
}

bool Canvas::checkInComponent(){
    for(CompInfo *ci : *(frames->at(currentFrame).components)){
        if(inComponent(point,ci,components->at(ci->cIndex)->img->size())){
            if(currComp >= 0){
                prevComp = currComp;
            }
            currComp = ci->cIndex;
            emit openPropPanel();
            return true;
        }
    }
    return false;
}

void Canvas::animate(){
    update();
}

Canvas::~Canvas(){}

void Canvas::paintEvent(QPaintEvent *e){
    (void)e;
    h->startPainter(this);
    h->fillBG(this->width(),this->height());
    if(h->img!=nullptr){h->addImage();}
    if(currentFrame >= 0){
        for(CompInfo *ci : *(frames->at(currentFrame).components)){
            h->drawComponent(ci,*(components->at(ci->cIndex)->img));
        }
        if(currComp >= 0){
            h->highlightComponent(*(frames->at(currentFrame).components->at(currComp)),components->at(currComp)->img->size(),components->at(currComp)->shape);
            if(rotaterVisible){
                h->showRotater(frames->at(currentFrame).components->at(currComp)->coord);
            }
        }
    }
    switch(mode) {
        case EDIT:
            h->drawShape(addShape);
            [[clang::fallthrough]];
        case MOVE:
            if(frames->at(currentFrame).bones->count()){
                for(Bone *b : *(frames->at(currentFrame).bones)){
                    h->showBone(b->coords);
                }
            }
            break;
        case SELECT:
            h->showPoints(selectPoints);
            break;
    }
    if(drawnPoints.size()){
        for(QPair<QColor,QList<QPoint>> cList : drawnPoints){
            h->setupPen(cList.first);
            for(QPoint p : cList.second){
                h->drawPoint(p);
            }
        }
    }
    h->stopPainter();
}

void Canvas::createComponent(QImage *i, QPoint p){
    i->fill(qRgba(0,0,0,0));
    int sCount = 1;
    QString *sComp = new QString("Component");
    QString *s = new QString(sComp+QString::number(sCount));
    QString *sCheck = new QString;
    bool invalid = true;
    while(invalid){
        for(CompType *ct : *components){
            if(ct->name->compare(*s)==0){
                sCount++;
                *s = QString(sComp+QString::number(sCount));
            }
        }
        if(s->compare(sCheck)){
            invalid=false;
        }else{
            sCheck = s;
        }
    }
    QString *t = new QString;
    *t = addShape.type;
    components->append(new CompType(s,i,t));
    int lIndex = components->size()-1;
    if(!lIndex){components->last()->isRoot=true;}
    for(Frame f : *frames){
        LineEq l = (components->last()->isRoot && i->size().height() > i->size().width()) ? LineEq(tan(90),p) : LineEq(tan(0),p);
        f.components->append(new CompInfo(lIndex,p,0,l));
    }
    addShape = AddShape();
    currComp = lIndex;
    emit shapeReleased();
    shape = NONE;
    saved = false;
}

void Canvas::generateBone(){
    CompInfo* c = frames->at(currentFrame).components->at(currComp);
    QSize sz = components->at(currComp)->img->size();
    for(CompInfo* ci : *(frames->at(currentFrame).components)){
        if(currComp != ci->cIndex){
            QPoint inP  = c->leq.getIntersection(ci->leq);
            if(inComponent(inP,ci,components->at(ci->cIndex)->img->size())){
                if(inComponent(inP,c,sz)){
                    if(!boneConnectionExists(ci->cIndex,currComp)){
                        frames->at(currentFrame).bones->append(new Bone(inP,ci->cIndex,{currComp}));
                    }
                }else{
                    if(inP.x() > c->coord.x()){
                        if(c->angle>90 && c->angle<270){
                            inP = c->coord;
                            inP.rx()-=sz.width()/2;
                            inP = MathsHandler::rotatePoint(c->coord,inP,c->angle);
                        }else{
                            inP = c->coord;
                            inP.rx()+=sz.width()/2;
                            if(sz.width()%2==1){inP.rx()++;}
                            inP = MathsHandler::rotatePoint(c->coord,inP,c->angle);
                        }
                    }else{
                        if(c->angle>90 && c->angle<270){
                            inP = c->coord;
                            inP.rx()+=sz.width()/2;
                            if(sz.width()%2==1){inP.rx()++;}
                            inP = MathsHandler::rotatePoint(c->coord,inP,c->angle);
                        }else{
                            inP = c->coord;
                            inP.rx()-=sz.width()/2;
                            inP = MathsHandler::rotatePoint(c->coord,inP,c->angle);
                        }
                    }
                    if(inComponent(inP,ci,components->at(ci->cIndex)->img->size())){
                        if(!boneConnectionExists(ci->cIndex,currComp)){
                            frames->at(currentFrame).bones->append(new Bone(inP,ci->cIndex,{currComp}));
                        }
                    }
                }
            }
        }
    }
}

bool Canvas::boneConnectionExists(int from, int to){
    for(Bone *b : *(frames->at(currentFrame).bones)){
        if(from==b->from){
            for(int t : b->to){
                if(to==t){
                    int rI = frames->at(currentFrame).bones->indexOf(b);
                    frames->at(currentFrame).bones->removeAt(rI);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Canvas::inComponent(QPoint p, CompInfo *c, QSize s, QPoint *rP){
    QPoint newP = QPoint(p.x() - c->coord.x(),p.y() - c->coord.y());
    double angleRad = c->angle*(M_PI/180);
    newP = QPoint(int(newP.x()*cos(angleRad) - newP.y()*sin(angleRad)),int((newP.y()*cos(angleRad) + newP.x()*sin(angleRad))));
    bool validX = false;
    bool validY = false;
    if(newP.x() < 0){
        if(newP.x() >= -(s.width()/2)){
            validX = true;
        }
    }else{
        if(s.width()%2==1){
            if(newP.x() <= s.width()/2){
                validX = true;
            }
        }else{
            if(newP.x() <= s.width()/2-1){
                validX = true;
            }
        }
    }
    if(newP.y() < 0){
        if(newP.y() >= -(s.height()/2)){
            validY = true;
        }
    }else{
        if(s.height()%2==1){
            if(newP.y() <= s.height()/2){
                validY = true;
            }
        }else{
            if(newP.y() <= s.height()/2-1){
                validY = true;
            }
        }
    }
    if(rP!=nullptr){
        *rP = newP;
    }
    return validX && validY;
}

bool Canvas::newProject(){
    return ProjectIOHandler::newProject();
}

bool Canvas::openProject(QWidget *window){
    return ProjectIOHandler::openProject(window);
}

void Canvas::saveProject(QString path){
    ProjectIOHandler::save(path, frames);
}
