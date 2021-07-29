QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS +=-L$$PWD/lib/log4cxx -llog4cxx -L/usr/local/lib -lmodbus -L/lib/x86_64-linux-gnu -lDLPrinter



SOURCES += \
    GetConfig.cpp \
    ModbusProcess.cpp \
    PackerMain.cpp \
    PrinterProcess.cpp \
    SingletonTimerScheduler.cpp \
    class/ErpInterface.cpp \
    class/HttpConnect.cpp \
    class/LabelInfoData.cpp \
    class/LabelTemplateData.cpp \
    class/PackRuleData.cpp \
    class/PackerModbusTCP.cpp \
    class/PackerTcpSocket.cpp \
    class/QrCodeData.cpp \
    class/SingletonModbusInfo.cpp \
    class/SingletonPackerData.cpp \
    class/SingletonPrinter.cpp \
    class/SingletonPropertiesTable.cpp \
    class/SingletonUserLogger.cpp \
    class/UserTimer.cpp \
    class/UserTimerManager.cpp \
    lib/xml/pugixml.cpp \
    test/LoggerTest.cpp \
    test/MainUnitTest.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ModbusProcess.h \
    PackerBase.h \
    PackerMain.h \
    PrinterProcess.h \
    SingletonTimerScheduler.h \
    class/ErpInterface.h \
    class/HttpConnect.h \
    class/LabelInfoData.h \
    class/LabelTemplateData.h \
    class/ModbusInfo.h \
    class/PackRuleData.h \
    class/PackerModbusTCP.h \
    class/PackerTcpSocket.h \
    class/QrCodeData.h \
    class/SingletonModbusInfo.h \
    class/SingletonPackerData.h \
    class/SingletonPrinter.h \
    class/SingletonPropertiesTable.h \
    class/UserTimer.h \
    class/UserTimerManager.h \
    lib/xml/pugixml.hpp \
    test/LoggerTest.h \
    class/SingletonUserLogger.h \
    test/MainUnitTest.h \
    tool/UniversalTool.h \
    lib/nlohmann/json.hpp

INCLUDEPATH += ./lib/xml \
               ./lib \
               ./class \
               ./lib/nlohmann

DISTFILES += \
    run/debug/config/log4cxx.properties \
    run/debug/config/config.cfg
