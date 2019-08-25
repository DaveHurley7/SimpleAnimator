#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QDialog>
#include <QAction>
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

namespace Ui {
class PreviewDialog;
}

class PreviewDialog : public QDialog
{
    Q_OBJECT

public:
    PreviewDialog(QList<QPixmap*> *imgs, QString projName, QWidget *parent = nullptr);
    ~PreviewDialog();

private:
    Ui::PreviewDialog *ui;
    QAction *initialFrame;
    QAction *currentFrame;
    QMenu *buttonMenu;
    int frameNum;
    bool isPaused;
    QList<QPixmap*> *images;
    QGraphicsScene *gs;
    QGraphicsPixmapItem *gpi;
    QString imgPath = ":/resources/images/";

public slots:
    void stopPreview();
    void display();
    void playMedia();
    void stopMedia();
    void skipBackFrame();
    void skipForwardFrame();
};

#endif // PREVIEWDIALOG_H
