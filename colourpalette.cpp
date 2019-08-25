#include "colourpalette.h"

#include <QDebug>
#include <QAbstractButton>

ColourPalette::ColourPalette(QWidget *parent) : QWidget(parent){
    ring.setColor(Qt::black);
}

ColourPalette::~ColourPalette(){}

void ColourPalette::paintEvent(QPaintEvent *ev){
    (void) *ev;
    p.begin(this);
    p.setPen(ring);
    p.drawImage(0,0,cpI);
    if(showColour){
        p.drawArc(mPos.x()-6,mPos.y()-6,12,12,0,5760);
    }
    p.end();
}

void ColourPalette::mousePressEvent(QMouseEvent *ev){
    showColour = true;
    mPos = ev->pos();
    update();
    pC = cpI.pixel(ev->pos());
    showColourChange();
}

void ColourPalette::mouseMoveEvent(QMouseEvent *ev){
    if(showColour){
        mPos = ev->pos();
        update();
        pC = cpI.pixel(ev->pos());
        showColourChange();
    }
}

void ColourPalette::mouseReleaseEvent(QMouseEvent *ev){
    (void) ev;
    showColour = false;
}

void ColourPalette::showColourChange(){
    lSld->setStyleSheet(lSld->styleSheet().replace(95,7,pC.name()));
    dC->setHsl(pC.hue(),pC.saturation(),lSld->value());
    updateColourInputs();
    display->setStyleSheet("background-color:"+dC->name());
}

void ColourPalette::changeLuminosity(int l){
    dC->setHsl(pC.hue(),pC.saturation(),l);
    updateColourInputs();
    display->setStyleSheet("background-color:"+dC->name());
}

void ColourPalette::updateColourInputs(){
    updateHslValues();
    updateRgbValues();
}

void ColourPalette::changeHue(int h){
    dC->setHsl(h,dC->saturation(),dC->lightness());
    updateRgbValues();
    updateColours();
}

void ColourPalette::changeSat(int s){
    dC->setHsl(dC->hue(),s,dC->lightness());
    updateRgbValues();
    updateColours();
}

void ColourPalette::changeLum(int l){
    dC->setHsl(dC->hue(),dC->saturation(),l);
    updateRgbValues();
    updateColours();
    lSld->setValue(l);
}

void ColourPalette::changeRed(int r){
    dC->setRed(r);
    updateHslValues();
    updateColours();
}

void ColourPalette::changeGreen(int g){
    dC->setGreen(g);
    updateHslValues();
    updateColours();
}

void ColourPalette::changeBlue(int b){
    dC->setBlue(b);
    updateHslValues();
    updateColours();
}

void ColourPalette::updateColours(){
    pC.setHsl(pC.hue(),pC.saturation(),128);
    lSld->setStyleSheet(lSld->styleSheet().replace(95,7,pC.name()));
    display->setStyleSheet("background-color:"+dC->name()+";");
}

void ColourPalette::updateHslValues(){
    hS->blockSignals(true);
    sS->blockSignals(true);
    lS->blockSignals(true);
    hS->setValue(pC.hue());
    sS->setValue(pC.saturation());
    lS->setValue(lSld->value());
    hS->blockSignals(false);
    sS->blockSignals(false);
    lS->blockSignals(false);
}

void ColourPalette::updateRgbValues(){
    rS->blockSignals(true);
    bS->blockSignals(true);
    gS->blockSignals(true);
    rS->setValue(dC->red());
    bS->setValue(dC->blue());
    gS->setValue(dC->green());
    rS->blockSignals(false);
    bS->blockSignals(false);
    gS->blockSignals(false);
}

void ColourPalette::getDefinedColour(int bId){
    switch(bId){
        case 1:
            dC->setRgb(255,0,0);
            break;
        case 2:
            dC->setRgb(0,0,255);
            break;
        case 3:
            dC->setRgb(0,255,0);
            break;
        case 4:
            dC->setRgb(255,136,0);
            break;
        case 5:
            dC->setRgb(255,255,0);
            break;
        case 6:
            dC->setRgb(139,69,19);
            break;
        case 7:
            dC->setRgb(255,0,255);
            break;
        case 8:
            dC->setRgb(255,192,203);
            break;
        case 9:
            dC->setRgb(64,224,208);
            break;
        case 10:
            dC->setRgb(255,255,255);
            break;
        case 11:
            dC->setRgb(0,0,0);
            break;
        case 12:
            dC->setRgb(64,64,64);
            break;
        case 13:
            dC->setRgb(128,128,128);
            break;
        case 14:
            dC->setRgb(192,192,192);
            break;
        case 15:
            dC->setRgb(139,0,0);
            break;
        case 16:
            dC->setRgb(0,51,102);
            break;
        case 17:
            dC->setRgb(0,100,0);
            break;
        case 18:
            dC->setRgb(173,216,230);
            break;
    }
    display->setStyleSheet("background-color:"+dC->name()+";");
}



