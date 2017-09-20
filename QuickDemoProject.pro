TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += src/main.cpp \
    src/InformationFetcher.cpp \
    src/InformationBinder.cpp

RESOURCES += resources/qml.qrc

INCLUDEPATH += include

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    include/InformationFetcher.h \
    include/InformationBinder.h
