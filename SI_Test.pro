TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += \
        -static-libstdc++ \
        -static-libgcc

HEADERS += \
        ListRand.h

SOURCES += \
        ListRand.cpp \
        Main.cpp
