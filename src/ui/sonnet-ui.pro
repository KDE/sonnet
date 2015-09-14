TARGET = sonnet-ui
TEMPLATE = lib
CONFIG += staticlib
QT += widgets

SOURCES += highlighter.cpp
HEADERS += highlighter.h

DEFINES += SONNETUI_EXPORT=""
DEFINES += SONNETCORE_EXPORT=""
DEFINES += INSTALLATION_PLUGIN_PATH=""
DEFINES += SONNET_STATIC

INCLUDEPATH += ../core

system("touch sonnetui_export.h")

