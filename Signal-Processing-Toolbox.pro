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

# FFTW.
# On unix the library is resolved through pkg-config. On Windows (or when
# pkg-config is unavailable) point FFTW_DIR at the FFTW folder, either here or
# via qmake FFTW_DIR=/path/to/fftw.
isEmpty(FFTW_DIR): FFTW_DIR = $$(FFTW_DIR)

# Fall back to the historical checkout layout so existing setups keep building.
win32:isEmpty(FFTW_DIR) {
    LEGACY_FFTW_DIR = $$PWD/../../../libs/fftw-3.3.5-dll64
    exists($$LEGACY_FFTW_DIR): FFTW_DIR = $$LEGACY_FFTW_DIR
}

!isEmpty(FFTW_DIR) {
    win32: LIBS += -L$$FFTW_DIR/ -llibfftw3-3
    else:  LIBS += -L$$FFTW_DIR/ -lfftw3
    INCLUDEPATH += $$FFTW_DIR
    DEPENDPATH += $$FFTW_DIR
} else {
    unix {
        CONFIG += link_pkgconfig
        PKGCONFIG += fftw3
    }
    win32: error("FFTW not found. Pass FFTW_DIR=<path to fftw> to qmake.")
}
