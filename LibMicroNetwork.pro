TEMPLATE = app
CONFIG += console c++17

include(Dependencies/Dependencies.pri)

SOURCES += \
    main.cpp

HEADERS += \
    LFrameworkConfig.h

LIBS += -lUser32 -lAdvapi32 -lOle32 -lWinusb -lSetupAPI

