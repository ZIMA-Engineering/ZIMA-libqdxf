TEMPLATE = lib
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt
#LIBS += -lGLU

LIBS += -L../  \
    -ldxfrw \

PRE_TARGETDEPS += ../libdxfrw.a

TARGET = qdxf
DESTDIR = ../


SOURCES += dxfinterface.cpp \
    spline.cpp \
    mtexttohtml.cpp \
    dxfsceneview.cpp

HEADERS += \
    dxfinterface.h \
    spline.h \
    mtexttohtml.h \
    scene_items.h \
    dxfsceneview.h

OTHER_FILES +=

