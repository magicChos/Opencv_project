TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    sgbm.cpp \
    bm.cpp \
    auxiliary.cpp \
    bm_main.cpp \
    tools/auxiliary.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/local/lib/release/ -lopencv_world.3.4.1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/local/lib/debug/ -lopencv_world.3.4.1
else:unix: LIBS += -L$$PWD/../../../usr/local/lib/ -lopencv_world.3.4.1

INCLUDEPATH += $$PWD/../../../usr/local/include

HEADERS += \
    sgbm.h \
    bm.h \
    auxiliary.h \
    tools/auxiliary.h


