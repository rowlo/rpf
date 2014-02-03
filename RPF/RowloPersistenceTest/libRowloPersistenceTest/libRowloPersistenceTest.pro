#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T21:25:01
#
#-------------------------------------------------

QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_librowlopersistencetest
DESTDIR = ../tests
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

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
# Those are the relative paths for qtcreator (source view):
    ../../RowloPersistence/libRowloPersistence/include

SOURCES += \
    src/tst_librowlopersistencetest.cpp \
    src/persistence/PersistableModelElementTest.cpp \
    src/persistence/ErrorTest.cpp \
    ../../../RowloPersistence/libRowloPersistence/src/persistence/Error.cpp \
    ../../../RowloPersistence/libRowloPersistence/src/persistence/PersistableModelElement.cpp \
    ../../../RowloPersistence/libRowloPersistence/src/persistence/PersistableModelElementFactory.cpp \
    ../../../RowloPersistence/libRowloPersistence/src/LibRowloPersistenceMain.cpp \
    src/persistence/PersistableModelElementFactoryTest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    include/persistence/PersistableModelElementTest.h \
    include/persistence/ErrorTest.h \
    include/persistence/PersistableModelElementFactoryTest.h
