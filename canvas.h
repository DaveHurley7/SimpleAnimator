#ifndef CANVAS_H
#define CANVAS_H

#include <QFrame>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <vector>
#include <QPoint>
#include "frame.h"
#include "animframe.h"
#include "componentpainter.h"
#include "comptype.h"
#include "projectinfo.h"
#include "lineeq.h"

class CanvasHelper;

class Canvas : public QWidget{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas() override;
    void addHelper(CanvasHelper *h);
    CanvasHelper* getHelper();
    bool loadComponents(struct project projDetails, struct projectinfo *projInfo);
    QList<Frame> *frames;
    QList<CompType*> *components;
    int currentFrame;
    void createPixmaps(QList<QPixmap*> *f, QSize s);
    void updatePixmap(QPixmap* p);
    enum Mode{
        MOVE,
        SELECT,
        EDIT
    };
    enum Shape{
        NONE,
        SQUARE,
        CIRCLE
    };
    Mode mode;
    Shape shape = NONE;
    struct rect{
        int iX,iY,rX,rY,w,h;
        rect(){}
    };
    struct circle{
        int x,y,pX,pY,rad,s;
        circle(){}
    };
    struct AddShape{
        QString type = "";
        union{
            struct rect r;
            struct circle c;
        };
        AddShape(){r=rect();c=circle();}
    } addShape;
    struct SelectShape{
        int t, l, b, r;
    } shapeSelect;
    int currComp = -1;
    int prevComp = -1;
    int currAngle = 0;
    int baseAngle = 0;
    int angleChange = 0;
    QPoint point;
    QPoint offset;
    bool saved = true;
    bool newProject();
    bool openProject(QWidget *window);
    void saveProject(QString path);
    bool checkInComponent();
    void createComponent(QImage *i, QPoint p);
    bool rotaterVisible = false;
    QColor *colour = new QColor(8,236,248);

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void shapeReleased();
    void frameUpdated();
    void openPropPanel();
    void closePropPanel();

public slots:
    void animate();

private:
    CanvasHelper *h;
    ComponentPainter *cp;
    bool inComponent(QPoint p, CompInfo *c, QSize s, QPoint *rP = nullptr);
    QList<QPoint> selectPoints;
    QList<QPair<QColor,QList<QPoint>>> drawnPoints;
    bool drawing = false;
    bool moving = false;
    bool rotating = false;
    int currColour = 0;
    void generateBone();
    bool boneConnectionExists(int from, int to);
};
#endif // CANVAS_H
