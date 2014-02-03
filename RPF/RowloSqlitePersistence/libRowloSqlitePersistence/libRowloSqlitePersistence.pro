#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T21:27:39
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = RowloSqlitePersistence
DESTDIR = ../../../_/lib
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
    ../../../RowloPersistence/libRowloPersistence/include \
# Those are the relative paths for qtcreator (source view):
    ../../RowloPersistence/libRowloPersistence/include

HEADERS += LibRowloSqlitePersistenceMain.h\
        libRowloSqlitePersistence_global.h \
    include/persistence/sqlite/SQLiteDataBaseFacade.h \
    include/persistence/sqlite/ErrorCodes.h

copyheaders.commands += mkdir -p ../../../_/include/persistence/sqlite &&
copyheaders.commands += cp -f ../../libRowloSqlitePersistence/include/*.h ../../../_/include &&
copyheaders.commands += cp -f ../../libRowloSqlitePersistence/include/persistence/sqlite/*.h ../../../_/include/persistence/sqlite

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

LIBS += \
    -L../../../_/lib -lRowloPersistence
