#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T21:27:39
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = RowloSqlitePersistence

LIB_DIRNAME = libRowloSqlitePersistence
BASELIB_DIRNAME = libRowloPersistence

BUILD_DIR = ../..
SOURCE_DIR = $${BUILD_DIR}/../$${LIB_DIRNAME}
SOURCE_INCLUDE_DIR = ../$${LIB_DIRNAME}
TOPLEVEL_DIR = $${BUILD_DIR}/../..
BASESOURCE_DIR = $${BUILD_DIR}/../../$${BASELIB_DIRNAME}/$${BASELIB_DIRNAME}
BASESOURCE_INCLUDE_DIR = ../../$${BASELIB_DIRNAME}/$${BASELIB_DIRNAME}
LIB_TARGET_DIR = $${TOPLEVEL_DIR}/_/lib
INCLUDE_TARGET_DIR = $${TOPLEVEL_DIR}/_/include
TESTCOVERAGE_DIR = $${BUILD_DIR}/testcoverage/$${LIB_DIRNAME}

DESTDIR = $${LIB_TARGET_DIR}
TEMPLATE = lib
VER_MAJ = 1
VER_MIN = 0
VER_PAT = 0rc1

QMAKE_EXTRA_TARGETS += copyheaders
POST_TARGETDEPS += copyheaders

DEFINES += LIBROWLOSQLITEPERSISTENCE_LIBRARY

SOURCES += \
    src/persistence/sqlite/SQLiteDataBaseFacade.cpp \
    src/LibRowloSqlitePersistenceMain.cpp

INCLUDEPATH += \
    include \
# INCLUDEPATH is used by build and by qtcreator, however, work dir for build is one level deeper.
# Hence, specifying only build-relative-path breaks qtcreator's source lookup. Working around by
# spezifying paths for both.
# Those are the relative paths for building (build view):
    $${BASESOURCE_DIR}/include \
# Those are the relative paths for qtcreator (source view):
    $${BASESOURCE_INCLUDE_DIR}/include

HEADERS += LibRowloSqlitePersistenceMain.h\
        libRowloSqlitePersistence_global.h \
    include/persistence/sqlite/SQLiteDataBaseFacade.h \
    include/persistence/sqlite/ErrorCodes.h

copyheaders.commands += mkdir -p $${INCLUDE_TARGET_DIR}/persistence/sqlite &&
copyheaders.commands += cp -f $${SOURCE_DIR}/include/*.h $${INCLUDE_TARGET_DIR} &&
copyheaders.commands += cp -f $${SOURCE_DIR}/include/persistence/sqlite/*.h $${INCLUDE_TARGET_DIR}/persistence/sqlite

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

LIBS += \
    -L$${LIB_TARGET_DIR} -lRowloPersistence
