QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fft.cpp \
    filter.cpp \
    freqdomainplot.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    timedomainplot.cpp \
    wavegenerator.cpp \
    wavreader.cpp

HEADERS += \
    fft.h \
    filter.h \
    freqdomainplot.h \
    mainwindow.h \
    qcustomplot.h \
    timedomainplot.h \
    wavegenerator.h \
    wavreader.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/../../../libs/fftw-3.3.5-dll64/ -llibfftw3-3

INCLUDEPATH += $$PWD/../../../libs/fftw-3.3.5-dll64
DEPENDPATH += $$PWD/../../../libs/fftw-3.3.5-dll64
