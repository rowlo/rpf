#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T21:25:01
#
#-------------------------------------------------

QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_librowlopersistencetest
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

LIBS += \
    -lgcov

TEMPLATE = app

INCLUDEPATH += \
    include \
    ../libRowloPersistence/include

SOURCES += \
    src/tst_librowlopersistencetest.cpp \
    src/persistence/PersistableModelElementTest.cpp \
    src/persistence/ErrorTest.cpp \
    ../libRowloPersistence/src/persistence/Error.cpp \
    ../libRowloPersistence/src/persistence/PersistableModelElement.cpp \
    ../libRowloPersistence/src/persistence/PersistableModelElementFactory.cpp \
    ../libRowloPersistence/src/LibRowloPersistenceMain.cpp \
    src/persistence/PersistableModelElementFactoryTest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    include/persistence/PersistableModelElementTest.h \
    include/persistence/ErrorTest.h \
    include/persistence/PersistableModelElementFactoryTest.h
