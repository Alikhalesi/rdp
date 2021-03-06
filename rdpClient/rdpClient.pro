QT += quick
INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x86"
LIBS     += -lgdi32 -luser32
CONFIG += c++11
QMAKE_CXXFLAGS += /Zi
QMAKE_LFLAGS += /DEBUG
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Controller/clientController.cpp \
        Controller/commandController.cpp \
        main.cpp \
        managers/localHookManager.cpp \
        managers/uiupdater.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Controller/clientController.hpp \
    Controller/commandController.hpp \
    managers/localHookManager.hpp \
    managers/uiupdater.h



QMAKE_LIBDIR +=D:/qtProjetcts/build-rdpLib-Desktop_Qt_6_2_1_MSVC2019_64bit-Release/release
INCLUDEPATH += D:/qtProjetcts/rdpLib \
D:\qtProjetcts\rxqt\include \
D:\qtProjetcts\rxqt\RxCpp\Rx\v2\src
DEPENDPATH += $$PWD/../rdpLib

LIBS +=-lrdpLib
