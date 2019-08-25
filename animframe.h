#ifndef ANIMFRAME_H
#define ANIMFRAME_H

#include <QWidget>
#include <QListWidgetItem>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QVBoxLayout>

class AnimFrame : public QWidget
{
    Q_OBJECT
public:
    AnimFrame(QPixmap p, int frameNum);
    ~AnimFrame();
    QWidget *pane;
    QGraphicsView* view;
    QGraphicsScene* scene;
    QGraphicsPixmapItem* gpi;
    int frameNum;
    QLabel* frameCountLabel;
    QVBoxLayout* layout;
    void mouseEventPress();

signals:
    void clicked();
};

#endif // ANIMFRAME_H
