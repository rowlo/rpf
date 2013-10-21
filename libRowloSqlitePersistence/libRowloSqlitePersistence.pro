#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T21:27:39
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = RowloSqlitePersistence
TEMPLATE = lib
VER_MAJ = 1
VER_MIN = 0
VER_PAT = 0rc1

DEFINES += LIBROWLOSQLITEPERSISTENCE_LIBRARY

SOURCES += \
    src/persistence/sqlite/SQLiteDataBaseFacade.cpp \
    src/LibRowloSqlitePersistenceMain.cpp

INCLUDEPATH += \
    include \
    ../libRowloPersistence/include

HEADERS += LibRowloSqlitePersistenceMain.h\
        libRowloSqlitePersistence_global.h \
    include/persistence/sqlite/SQLiteDataBaseFacade.h \
    include/persistence/sqlite/ErrorCodes.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

LIBS += \
    -L../libRowloPersistence-build-Desktop_Qt_5_0_0_GCC_64bit_SDK-Debug -lRowloPersistence
