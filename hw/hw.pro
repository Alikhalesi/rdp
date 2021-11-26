QT += quick
INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x86"
LIBS     += -lgdi32 -luser32
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        channel/p2pChannel.cpp \
        components/FrameImageProvider.cpp \
        controller/commandController.cpp \
        controller/serverController.cpp \
        main.cpp \
        streamer/DesktopFrameGenerator.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    hw_fa_IR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    channel/p2pChannel.hpp \
    components/FrameImageProvider.hpp \
    controller/commandController.hpp \
    controller/serverController.hpp \
    streamer/DesktopFrameGenerator.hpp \
    streamer/Streamer.hpp

QMAKE_LIBDIR +=D:/qtProjetcts/build-rdpLib-Desktop_Qt_6_2_1_MSVC2019_64bit-Release/release
INCLUDEPATH += D:/qtProjetcts/rdpLib
DEPENDPATH += $$PWD/../rdpLib

LIBS +=-lrdpLib
