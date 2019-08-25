#ifndef CREATEPROJWINDOW_H
#define CREATEPROJWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <QRegularExpression>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>

namespace Ui {
class CreateProjDialog;
}

class CreateProjDialog : public QDialog{
    Q_OBJECT
public:
    explicit CreateProjDialog(QWidget *parent = nullptr);
    ~CreateProjDialog() override;
    virtual void closeEvent(QCloseEvent *ev) override;
    QUrl filename;

public slots:
    void browseProjPath();
    void createProject();
    void toggleImageFind();
    void chooseImageInDialog();

private:
    Ui::CreateProjDialog *ui;
    QString homeUrl = QDir::homePath()+"/Documents/SimpleAnimatorProjects";
    QString recentPath;
    void displayError(QString error);
    QRegularExpression fileRegex = QRegularExpression("^[a-zA-Z0-9_\\-]+$");
    QGraphicsScene* gs;
    QGraphicsPixmapItem gpi;
    int previewH;
    QImage* cvsImg;
};

#endif // ANIMWINDOW_H
