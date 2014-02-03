#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T21:29:29
#
#-------------------------------------------------

QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_librowlosqlitepersistencetest
DESTDIR = ../tests
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

QMAKE_CLEAN += ../tests/*.backup coverage* *.gcno ../tests/tlog

LIBS += \
    -lgcov

TEMPLATE = app

INCLUDEPATH += \
    include \
# INCLUDEPATH is used by build and by qtcreator, however, work dir for build is one level deeper.
# Hence, specifying only build-relative-path breaks qtcreator's source lookup. Working around by
# spezifying paths for both.
# Those are the relative paths for building (build view):
    ../../../RowloPersistence/libRowloPersistence/include \
    ../../../RowloSqlitePersistence/libRowloSqlitePersistence/include \
# Those are the relative paths for qtcreator (source view):
    ../../RowloPersistence/libRowloPersistence/include \
    ../../RowloSqlitePersistence/libRowloSqlitePersistence/include

SOURCES += src/tst_librowlosqlitepersistencetest.cpp \
    src/persistence/sqlite/SQLiteDataBaseFacadeTest.cpp \
    ../../../RowloPersistence/libRowloPersistence/src/persistence/Error.cpp \
    ../../../RowloPersistence/libRowloPersistence/src/persistence/PersistableModelElement.cpp \
    ../../../RowloPersistence/libRowloPersistence/src/persistence/PersistableModelElementFactory.cpp \
    ../../../RowloSqlitePersistence/libRowloSqlitePersistence/src/persistence/sqlite/SQLiteDataBaseFacade.cpp \
    ../../../RowloSqlitePersistence/libRowloSqlitePersistence/src/LibRowloSqlitePersistenceMain.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    include/persistence/sqlite/SQLiteDataBaseFacadeTest.h
