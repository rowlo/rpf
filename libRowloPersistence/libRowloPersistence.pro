#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T21:12:42
#
#-------------------------------------------------

QT       -= gui

TARGET = RowloPersistence
TEMPLATE = lib
VER_MAJ = 1
VER_MIN = 0
VER_PAT = 0rc1

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

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
