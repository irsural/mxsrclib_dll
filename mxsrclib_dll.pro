CONFIG -= qt

TEMPLATE = lib
DEFINES += MXSRCLIB_DLL_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    $$PWD\LibMPSSE \
    $$PWD\LibMPSSE\Include

SOURCES += \
    ftdi_dll.cpp \
    gpio_ftdi_control.cpp \
    LibMPSSE/ftdi_common.c \
    LibMPSSE/ftdi_i2c.c \
    LibMPSSE/ftdi_infra.c \
    LibMPSSE/ftdi_mid.c \
    LibMPSSE/ftdi_spi.c

HEADERS += \
    mxsrclib_dll_global.h \
    gpio_ftdi_control.h \
    CDMv2.12.28/ftd2xx.h \
    LibMPSSE/Include/libMPSSE_i2c.h \
    LibMPSSE/Include/libMPSSE_spi.h \
    LibMPSSE/ftdi_common.h \
    LibMPSSE/ftdi_i2c.h \
    LibMPSSE/ftdi_infra.h \
    LibMPSSE/ftdi_mid.h \
    LibMPSSE/ftdi_spi.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/CDMv2.12.28/Static/i386/ -lftd2xx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/CDMv2.12.28/Static/i386/ -lftd2xx

INCLUDEPATH += $$PWD/CDMv2.12.28
DEPENDPATH += $$PWD/CDMv2.12.28
