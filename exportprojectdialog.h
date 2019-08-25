#ifndef EXPORTPROJECTDIALOG_H
#define EXPORTPROJECTDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QDir>
#include "project.h"

namespace Ui {
class ExportProjectDialog;
}

class ExportProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportProjectDialog(QString *p, QString *t, QString *f, QWidget *parent = nullptr);
    ~ExportProjectDialog();

public slots:
    void accept();
    void reject();
    void setPath();

private:
    Ui::ExportProjectDialog *ui;
    QButtonGroup bgT, bgF;
    QString defaultUrl = QDir::homePath()+"/Pictures/";
    QString recentUrl = "";
    QString *exportUrl, *exportType, *format;
};

#endif // EXPORTPROJECTDIALOG_H
