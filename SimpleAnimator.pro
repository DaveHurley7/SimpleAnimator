#-------------------------------------------------
#
# Project created by QtCreator 2019-01-01T18:53:12
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleAnimator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
INCLUDEPATH += $$[QT_INSTALL_PREFIX]/Src/3rdparty/QtZlib
DEFINES += QT_DEPRECATED_WARNINGS \
            QUAZIP_STATIC
LIBS += -lz

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        clickablelabel.cpp \
    colourpalette.cpp \
    componentpainter.cpp \
    exportframedialog.cpp \
    exportprojectdialog.cpp \
    frame.cpp \
        main.cpp \
    createprojwindow.cpp \
    editscreen.cpp \
    mathshandler.cpp \
    pixmappainter.cpp \
    previewdialog.cpp \
    projectiohandler.cpp \
    sorter.cpp \
    startscreen.cpp \
    dbconnection.cpp \
    canvas.cpp \
    canvasHelper.cpp \
    clickablegraphic.cpp \
    animframe.cpp   \
    JlCompress.cpp  \
    qioapi.cpp  \
    quaadler32.cpp  \
    quacrc32.cpp    \
    quagzipfile.cpp \
    quaziodevice.cpp    \
    quazip.cpp  \
    quazipdir.cpp   \
    quazipfile.cpp  \
    quazipfileinfo.cpp  \
    quazipnewinfo.cpp   \
    unzip.c \
    zip.c

HEADERS += \
    bone.h \
    clickablelabel.h \
    colourpalette.h \
    component.h \
    componentpainter.h \
    comptype.h \
    createprojwindow.h \
    editscreen.h \
    exportframedialog.h \
    exportprojectdialog.h \
    frame.h \
    lineeq.h \
    mathshandler.h \
    pixmappainter.h \
    previewdialog.h \
    projectinfo.h \
    projectiohandler.h \
    recentprojitem.h \
    sorter.h \
    startscreen.h \
    project.h \
    dbconnection.h \
    canvas.h \
    canvasHelper.h \
    clickablegraphic.h \
    animframe.h \
    crypt.h \
    ioapi.h \
    JlCompress.h   \
    quaadler32.h    \
    quachecksum32.h \
    quacrc32.h  \
    quagzipfile.h   \
    quaziodevice.h  \
    quazip.h    \
    quazip_global.h \
    quazipdir.h \
    quazipfile.h    \
    quazipfileinfo.h    \
    quazipnewinfo.h \
    unzip.h \
    zip.h

FORMS += \
    createprojwindow.ui \
    editscreen.ui \
    exportframedialog.ui \
    exportprojectdialog.ui \
    previewdialog.ui \
    startscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
