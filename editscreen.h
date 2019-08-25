#ifndef EDITSCREEN_H
#define EDITSCREEN_H

#include <QMainWindow>
#include <QImage>
#include <QDir>
#include <QMenu>
#include <QButtonGroup>
#include <animframe.h>
#include "canvasHelper.h"
#include "project.h"
#include "projectinfo.h"
#include "recentprojitem.h"

namespace Ui {
class EditScreen;
}

class EditScreen : public QMainWindow{
    Q_OBJECT

public:
    EditScreen(struct project proj, QImage *img = nullptr, QWidget *parent = nullptr);
    ~EditScreen();

public slots:
    void closeProject();
    void addFrameToTimeLine(QPixmap *pmp = nullptr, int index = 1);
    void closePanel();
    void preview();
    void setMode();
    void changeFrameNum();
    void setShape();
    void exportImage();
    void setItemFocus();
    void exportProject();
    void deselectShapes();
    void updateFrame();
    void showPropertyPanel();
    void openProject();
    void saveProject();
    void addFrameToProject();
    void removeFrame();
    void getListOfRecentProjects();
    void openRecentProject(QAction *a);

private:
    Ui::EditScreen *ui;
    CanvasHelper h;
    void setupIcons();
    void connectActions();
    struct project projDetails;
    struct projectinfo projInfo;
    QString imgPath = ":/resources/images/";
    virtual void closeEvent(QCloseEvent *ev);
    QList<QPixmap*> *imgs;
    QList<AnimFrame*> *frames;
    QString homeUrl = QDir::homePath()+"/Documents/SimpleAnimatorProjects";
    QList<rpItem> projList;
    void setupDefinedColours();
    QButtonGroup cBG;
    void connectColourModifiers();
};

#endif // EDITSCREEN_H
