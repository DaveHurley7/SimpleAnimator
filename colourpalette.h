#ifndef COLOURPALETTE_H
#define COLOURPALETTE_H

#include <QPainter>
#include <QWidget>
#include <QMouseEvent>
#include <QSpinBox>
#include <QSlider>
#include <QButtonGroup>

class ColourPalette : public QWidget
{
    Q_OBJECT
public:
    explicit ColourPalette(QWidget *parent = nullptr);
    ~ColourPalette() override;
    QColor pC = QColor(8,236,248), *dC = new QColor(8,236,248);
    QSpinBox *hS, *sS, *lS, *rS, *gS, *bS;
    QSlider *lSld, *oSld;
    QWidget *display;
    QButtonGroup *colourGroup;
    void showColourChange();
    void updateColours();

public slots:
    void changeLuminosity(int l);
    void updateColourInputs();
    void changeHue(int h);
    void changeSat(int s);
    void changeLum(int l);
    void changeRed(int r);
    void changeGreen(int g);
    void changeBlue(int b);
    void updateHslValues();
    void updateRgbValues();
    void getDefinedColour(int bId);

protected:
    void paintEvent(QPaintEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

signals:
    void colourChanged();

private:
    QLinearGradient hLG, sLG;
    QPainter p;
    QString imgPath = ":/resources/images/";
    QPoint mPos;
    QPen ring;
    QImage cpI = QImage(imgPath+"colourpalette.png");
    bool showColour = false;
};

#endif // COLOURPALETTE_H
