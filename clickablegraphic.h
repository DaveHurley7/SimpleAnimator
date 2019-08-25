#ifndef CLICKABLEGRAPHIC_H
#define CLICKABLEGRAPHIC_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <Qt>

namespace Ui {
    class ClickableGraphic;
}

class ClickableGraphic : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ClickableGraphic(QWidget *parent = nullptr);
    ~ClickableGraphic();
    void setIcon(QObject *p, QPixmap pmp);

signals:
    void clicked();

protected:
    QGraphicsScene* gs;
    QGraphicsPixmapItem gpi;
    QObject *p;
    void mousePressEvent(QMouseEvent *ev);
};

#endif // CLICKABLEGRAPHIC_H
