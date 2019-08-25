#ifndef EXPORTFRAMEDIALOG_H
#define EXPORTFRAMEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDir>

namespace Ui {
class ExportFrameDialog;
}

class ExportFrameDialog : public QDialog
{
    Q_OBJECT

public:
    ExportFrameDialog(QString *u, QString n, QString fT, QPixmap *p, QWidget *parent = nullptr);
    ~ExportFrameDialog();

public slots:
    void setPath();
    void accept();
    void reject();

private:
    Ui::ExportFrameDialog *ui;
    QGraphicsScene *gs;
    QGraphicsPixmapItem *gpi;
    QString defaultUrl = QDir::homePath()+"/Pictures/";
    QString recentUrl = "";
    QString *exportUrl;
    QString acceptedType;
};

#endif // EXPORTFRAMEDIALOG_H
