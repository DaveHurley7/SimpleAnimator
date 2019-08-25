#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QDir>
#include "dbconnection.h"

namespace Ui {
class StartDialog;
}

class StartDialog : public QDialog{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = nullptr);
    ~StartDialog();

public slots:
    void createProj();
    void openProj();

private:
    Ui::StartDialog *ui;
    DBCon* db;
};

#endif // MAINWINDOW_H
