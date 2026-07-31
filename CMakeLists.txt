cmake_minimum_required(VERSION 3.16)

project(Signal-Processing-Toolbox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport Multimedia)

# FFTW.
# Preferred route is pkg-config. Otherwise set FFTW_DIR (-DFFTW_DIR=... or an
# environment variable) to a folder holding fftw3.h and the import library,
# which is how a Windows build usually finds it.
if(NOT FFTW_DIR AND DEFINED ENV{FFTW_DIR})
    set(FFTW_DIR "$ENV{FFTW_DIR}")
endif()

if(FFTW_DIR)
    find_path(FFTW_INCLUDE_DIR fftw3.h HINTS "${FFTW_DIR}" NO_DEFAULT_PATH)
    find_library(FFTW_LIBRARY NAMES fftw3 libfftw3-3 HINTS "${FFTW_DIR}" NO_DEFAULT_PATH)
else()
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(PC_FFTW QUIET fftw3)
    endif()
    find_path(FFTW_INCLUDE_DIR fftw3.h HINTS ${PC_FFTW_INCLUDE_DIRS})
    find_library(FFTW_LIBRARY NAMES fftw3 libfftw3-3 HINTS ${PC_FFTW_LIBRARY_DIRS})
endif()

if(NOT FFTW_INCLUDE_DIR OR NOT FFTW_LIBRARY)
    message(FATAL_ERROR
        "FFTW not found. Install libfftw3-dev, or configure with -DFFTW_DIR=<path to fftw>.")
endif()

add_executable(Signal-Processing-Toolbox
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    fft.cpp
    fft.h
    filter.cpp
    filter.h
    freqdomainplot.cpp
    freqdomainplot.h
    timedomainplot.cpp
    timedomainplot.h
    wavegenerator.cpp
    wavegenerator.h
    wavreader.cpp
    wavreader.h
    third_party/qcustomplot/qcustomplot.cpp
    third_party/qcustomplot/qcustomplot.h
)

# Vendored QCustomPlot. PUBLIC so the uic-generated header, which includes
# <qcustomplot.h> by bare name, resolves it too.
target_include_directories(Signal-Processing-Toolbox PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/qcustomplot"
    "${FFTW_INCLUDE_DIR}"
)

target_link_libraries(Signal-Processing-Toolbox PRIVATE
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::Multimedia
    "${FFTW_LIBRARY}"
)

# QCustomPlot is third party code: keep its warnings out of the build log so the
# project's own warnings stay visible.
if(NOT MSVC)
    set_source_files_properties(third_party/qcustomplot/qcustomplot.cpp
        PROPERTIES COMPILE_OPTIONS "-w")
endif()

include(GNUInstallDirs)
install(TARGETS Signal-Processing-Toolbox
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
