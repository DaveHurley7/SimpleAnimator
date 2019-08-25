#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QWidget>
#include <Qt>

namespace Ui {
    class clickableLabel;
}

class clickableLabel: public QLabel{
    Q_OBJECT

public:
    explicit clickableLabel(QWidget *parent = nullptr);
    ~clickableLabel();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* ev);
};

#endif // CLICKABLELABEL_H
