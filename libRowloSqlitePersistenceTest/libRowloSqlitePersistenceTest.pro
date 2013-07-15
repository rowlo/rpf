#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T21:29:29
#
#-------------------------------------------------

QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_librowlosqlitepersistencetest
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

LIBS += \
    -lgcov

TEMPLATE = app

INCLUDEPATH += \
    include \
    ../libRowloPersistence/include \
    ../libRowloSqlitePersistence/include

SOURCES += src/tst_librowlosqlitepersistencetest.cpp \
    src/persistence/sqlite/SQLiteDataBaseFacadeTest.cpp \
    ../libRowloPersistence/src/persistence/Error.cpp \
    ../libRowloPersistence/src/persistence/PersistableModelElement.cpp \
    ../libRowloPersistence/src/persistence/PersistableModelElementFactory.cpp \
    ../libRowloSqlitePersistence/src/persistence/sqlite/SQLiteDataBaseFacade.cpp \
    ../libRowloSqlitePersistence/src/LibRowloSqlitePersistenceMain.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    include/persistence/sqlite/SQLiteDataBaseFacadeTest.h
