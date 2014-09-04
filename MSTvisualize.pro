QT              +=  core gui widgets

TARGET          =   MSTvisualize
TEMPLATE        =   app

SOURCES         +=  src/main.cpp \
                    src/mainwindow.cpp \
                    src/spantree.cpp \
                    src/view2d.cpp \
                    src/canvas.cpp

HEADERS         +=  src/mainwindow.h \
                    src/spantree.h \
    src/view2d.h \
    src/canvas.h


LIBS            +=  -stdlib=libc++ -L/usr/lib -L/usr/local/lib -lCGAL -lmpfr -lgmp
QMAKE_CXXFLAGS  +=  -stdlib=libc++
QMAKE_CXXFLAGS  +=  -std=c++11 -O2
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

OBJECTS_DIR     =   build
MOC_DIR         =   build


INCLUDEPATH     +=  /usr/local/include /usr/include

