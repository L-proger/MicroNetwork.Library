TEMPLATE = app
CONFIG += console c++17

include(Dependencies/Dependencies.pri)

SOURCES += \
    Library.cpp \
    main.cpp

HEADERS += \
    LFrameworkConfig.h \
    Library.h


HEADERS += $$files($$PWD/Api/*.h, true)

INCLUDEPATH += $$PWD/Interfaces

LIBS += -lUser32 -lAdvapi32 -lOle32 -lWinusb -lSetupAPI
