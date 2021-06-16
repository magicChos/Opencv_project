TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/tools/auxiliary.cpp \
    src/tools/geometry.cpp \
    src/extral_fun.cpp \
    src/HoughTransform.cpp \
    main.cpp \
    computeglobalscale.cpp \
    src/main2.cpp \
    src/universal_intrinsics.cpp \
    universal_intrinsics.cpp

HEADERS += include \
    include/tools/auxiliary.h \
    include/tools/geometry.h \
    include/tools/geometry_template.hpp \
    include/extral_fun.h \
    include/HoughTransform.h

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lopencv_world.3.4.1
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lopencv_world.3.4.1
#else:unix: LIBS += -L/usr/local/Cellar/opencv/4.2.0/lib/ -lopencv_imgproc.4.2.0 -lopencv_highgui.4.2.0 -lopencv_core.4.2.0

#INCLUDEPATH += $$PWD/../../../../../usr/local/include
#DEPENDPATH += /usr/local/Cellar/opencv/4.2.0/include/opencv4
LIBS += -L/usr/local/Cellar/opencv/4.2.0/lib/ -lopencv_core.4.2.0
INCLUDEPATH += /usr/local/Cellar/opencv/4.2.0/include/opencv4
