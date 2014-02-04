#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T21:12:42
#
#-------------------------------------------------

QT       -= gui

TARGET = RowloPersistence

LIB_DIRNAME = libRowloPersistence

BUILD_DIR = ../..
SOURCE_DIR = $${BUILD_DIR}/../$${LIB_DIRNAME}
TOPLEVEL_DIR = $${BUILD_DIR}/../..
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

DEFINES += LIBROWLOPERSISTENCE_LIBRARY

INCLUDEPATH += \
    include

SOURCES += \
    src/LibRowloPersistenceMain.cpp \
    src/persistence/PersistableModelElement.cpp \
    src/persistence/Error.cpp \
    src/persistence/PersistableModelElementFactory.cpp

HEADERS += \
    LibRowloPersistenceMain.h\
    libRowloPersistence_global.h \
    include/persistence/Error.h \
    include/persistence/PersistableModelElement.h \
    include/persistence/IDataBaseFacade.h \
    include/persistence/IModelElementFactory.h \
    include/persistence/PersistableModelElementFactory.h

copyheaders.commands += mkdir -p $${INCLUDE_TARGET_DIR}/persistence &&
copyheaders.commands += cp -f $${SOURCE_DIR}/include/*.h $${INCLUDE_TARGET_DIR} &&
copyheaders.commands += cp -f $${SOURCE_DIR}/include/persistence/*.h $${INCLUDE_TARGET_DIR}/persistence

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
