QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bitRestMapEditor
TEMPLATE = app

RC_FILE = BitRestMapEditor.rc

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    rmapobject.cpp \
    rmapobjectmodel.cpp \
    rtable.cpp \
    rtablemodel.cpp \
    rsection.cpp \
    rsectionmodel.cpp \
    rgridwidget.cpp \
    newmapdialog.cpp \
    rmapobjectmovepolicy.cpp \
    rmapobjectbehaviourpolicy.cpp \
    rmapobjectresizepolicy.cpp \
    rmapobjectfactory.cpp \
    rcompositemapobject.cpp \
    rmapobjecttreewidgetitem.cpp \
    rmapobjecttableviewdelegate.cpp \
    rmapobjectfocusholder.cpp \
    connectiondialog.cpp \
    rfontselectordialog.cpp \
    rpictureselectordialog.cpp \
    rfontdescriptor.cpp \
    rimagedescriptor.cpp \
    rcompositemapobjectmodel.cpp \
    rmapregistry.cpp \
    rrefsdescriptor.cpp \
    aboutdialog.cpp

HEADERS += \
    mainwindow.h \
    rmapobject.h \
    rmapobjectmodel.h \
    rtable.h \
    rtablemodel.h \
    rsection.h \
    rsectionmodel.h \
    rgridwidget.h \
    newmapdialog.h \
    rmapobjectbehaviourpolicy.h \
    rmapobjectmovepolicy.h \
    rmapobjectresizepolicy.h \
    rmapobjectfactory.h \
    rcompositemapobject.h \
    rmapobjecttreewidgetitem.h \
    rmapobjecttableviewdelegate.h \
    rmapobjectfocusholder.h \
    connectiondialog.h \
    rfontselectordialog.h \
    rpictureselectordialog.h \
    rfontdescriptor.h \
    rimagedescriptor.h \
    rcompositemapobjectmodel.h \
    rmapregistry.h \
    rrefsdescriptor.h \
    aboutdialog.h

OTHER_FILES += \
    BitRestMapEditor.rc
