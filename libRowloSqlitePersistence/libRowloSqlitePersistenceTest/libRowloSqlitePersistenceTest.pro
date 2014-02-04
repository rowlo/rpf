#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T21:29:29
#
#-------------------------------------------------

QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_librowlosqlitepersistencetest

LIB_DIRNAME = libRowloSqlitePersistence
BASELIB_DIRNAME = libRowloPersistence

BUILD_DIR = ../..
SOURCE_INCLUDE_DIR = ../$${LIB_DIRNAME}
SOURCE_DIR = $${BUILD_DIR}/../$${LIB_DIRNAME}
TEST_SOURCE_DIR = $${BUILD_DIR}/../$${LIB_DIRNAME}Test
TOPLEVEL_DIR = $${BUILD_DIR}/../..
BASESOURCE_DIR = $${BUILD_DIR}/../../$${BASELIB_DIRNAME}/$${BASELIB_DIRNAME}
BASESOURCE_INCLUDE_DIR = ../../$${BASELIB_DIRNAME}/$${BASELIB_DIRNAME}
LCOV_DATA_DIR = $${BUILD_DIR}/work/$${LIB_DIRNAME}Test
TESTCOVERAGE_DIR = $${BUILD_DIR}/testcoverage/$${LIB_DIRNAME}
TESTS_TARGET_DIR = $${BUILD_DIR}/tests/$${LIB_DIRNAME}

DESTDIR = $${TESTS_TARGET_DIR}
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

QMAKE_CLEAN += $${TESTS_TARGET_DIR}/*.backup
QMAKE_CLEAN += $${LCOV_DATA_DIR}/coverage*
QMAKE_CLEAN += $${LCOV_DATA_DIR}/*.gcno
QMAKE_CLEAN += $${LCOV_DATA_DIR}/*.gcda
QMAKE_CLEAN += $${TESTS_TARGET_DIR}/tlog

LIBS += \
    -lgcov

TEMPLATE = app

INCLUDEPATH += \
    include \
# INCLUDEPATH is used by build and by qtcreator, however, work dir for build is one level deeper.
# Hence, specifying only build-relative-path breaks qtcreator's source lookup. Working around by
# spezifying paths for both.
# Those are the relative paths for building (build view):
    $${BASESOURCE_DIR}/include \
    $${SOURCE_DIR}/include \
# Those are the relative paths for qtcreator (source view):
    $${BASESOURCE_INCLUDE_DIR}/include \
    $${SOURCE_INCLUDE_DIR}/include

SOURCES += src/tst_librowlosqlitepersistencetest.cpp \
    src/persistence/sqlite/SQLiteDataBaseFacadeTest.cpp \
    $${BASESOURCE_DIR}/src/persistence/Error.cpp \
    $${BASESOURCE_DIR}/src/persistence/PersistableModelElement.cpp \
    $${BASESOURCE_DIR}/src/persistence/PersistableModelElementFactory.cpp \
    $${SOURCE_DIR}/src/persistence/sqlite/SQLiteDataBaseFacade.cpp \
    $${SOURCE_DIR}/src/LibRowloSqlitePersistenceMain.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    include/persistence/sqlite/SQLiteDataBaseFacadeTest.h
