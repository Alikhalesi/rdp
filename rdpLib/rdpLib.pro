QT -= gui
QT += network quick multimedia
TEMPLATE = lib
DEFINES += RDPLIB_LIBRARY
INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x86"
LIBS     += -lgdi32 -luser32 -lgdi32 -lgdiplus
CONFIG += c++11
QMAKE_CXXFLAGS += /Zi
QMAKE_LFLAGS += /DEBUG
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    channel/LenValInterceptor.cpp \
    channel/inputCommandInterceptor.cpp \
    clienttcpchannel.cpp \
    components/FrameImageProvider.cpp \
    components/FrameWriter.cpp \
    managers/DefaultCommandManager.cpp \
    managers/frameManager.cpp \
    managers/hookManager.cpp \
    rdplib.cpp \
    servertcpchannel.cpp \
    util/imageUtil.cpp

HEADERS += \
    channel/IChannelInterceptor.hpp \
    channel/LenValInterceptor.hpp \
    channel/inputCommandInterceptor.hpp \
    clienttcpchannel.h \
    components/FrameImageProvider.hpp \
    components/FrameWriter.hpp \
    managers/DefaultCommandManager.hpp \
    managers/ICommandManager.hpp \
    managers/frameManager.hpp \
    managers/hookManager.hpp \
    model/command.hpp \
    rdpLib_global.h \
    rdplib.h \
    servertcpchannel.h \
    util/imageUtil.hpp \
    util/spinLock.hpp

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target


