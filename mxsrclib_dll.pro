QT       -= core gui qt

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
    $$PWD\LibMPSSE\Include \
    ../mxsrclib \
    ../mxsrclib/arch/qt

SOURCES += \
    ../mxsrclib/imp_filt.cpp \
    ftdi_dll.cpp \
    gpio_ftdi_control.cpp \
    LibMPSSE/ftdi_common.c \
    LibMPSSE/ftdi_i2c.c \
    LibMPSSE/ftdi_infra.c \
    LibMPSSE/ftdi_mid.c \
    LibMPSSE/ftdi_spi.c \
    ../mxsrclib/arch/qt/counter.cpp \
    ../mxsrclib/arch/qt/mxifa.cpp \
    ../mxsrclib/hardflowg.cpp \
    ../mxsrclib/irsalg.cpp \
    ../mxsrclib/irscpu.cpp \
    ../mxsrclib/irsdevices.cpp \
    ../mxsrclib/irserror.cpp \
    ../mxsrclib/irsexcept.cpp \
    ../mxsrclib/irsfilter.cpp \
    ../mxsrclib/irsint.cpp \
    ../mxsrclib/irslocale.cpp \
    ../mxsrclib/irsmbus.cpp \
    ../mxsrclib/irsstring.cpp \
    ../mxsrclib/irssysutils.cpp \
    ../mxsrclib/irstcpip.cpp \
    ../mxsrclib/irstime.cpp \
    ../mxsrclib/measmul.cpp \
    ../mxsrclib/mxdata.cpp \
    ../mxsrclib/mxnet.cpp \
    ../mxsrclib/mxnetc.cpp \
    ../mxsrclib/mxnetr.cpp \
    ../mxsrclib/niusbgpib.cpp \
    ../mxsrclib/niusbgpib_hardflow.cpp \
    ../mxsrclib/timer.cpp \
    impulse_filter_dll.cpp

HEADERS += \
    ../mxsrclib/imp_filt.h \
    mxsrclib_dll_global.h \
    gpio_ftdi_control.h \
    CDMv2.12.28/ftd2xx.h \
    LibMPSSE/Include/libMPSSE_i2c.h \
    LibMPSSE/Include/libMPSSE_spi.h \
    LibMPSSE/ftdi_common.h \
    LibMPSSE/ftdi_i2c.h \
    LibMPSSE/ftdi_infra.h \
    LibMPSSE/ftdi_mid.h \
    LibMPSSE/ftdi_spi.h \
    ../mxsrclib/arch/qt/counter.h \
    ../mxsrclib/hardflowg.h \
    ../mxsrclib/irsalg.h \
    ../mxsrclib/irscpu.h \
    ../mxsrclib/irsdevices.h \
    ../mxsrclib/irserror.h \
    ../mxsrclib/irsexcept.h \
    ../mxsrclib/irsfilter.h \
    ../mxsrclib/irsfinal.h \
    ../mxsrclib/irsint.h \
    ../mxsrclib/irslocale.h \
    ../mxsrclib/irsmbus.h \
    ../mxsrclib/irsstring.h \
    ../mxsrclib/irsstrmstd.h \
    ../mxsrclib/irssysutils.h \
    ../mxsrclib/irstcpip.h \
    ../mxsrclib/irstime.h \
    ../mxsrclib/measmul.h \
    ../mxsrclib/mxdata.h \
    ../mxsrclib/mxnet.h \
    ../mxsrclib/mxnetc.h \
    ../mxsrclib/mxnetr.h \
    ../mxsrclib/niusbgpib.h \
    ../mxsrclib/niusbgpib_hardflow.h \
    ../mxsrclib/timer.h \
    revisions.h

win32-msvc*: DEFINES += "_HAS_AUTO_PTR_ETC=1"

win32 {
  LIBS += \
          -lws2_32 \
          -lSetupapi \
}

win32-g++ {
  QMAKE_CXXFLAGS += -finput-charset=CP1251
  QMAKE_CXXFLAGS += -fno-strict-aliasing
  QMAKE_CXXFLAGS += -Wno-deprecated
  QMAKE_CXXFLAGS += -Wextra
  QMAKE_CXXFLAGS += -Woverloaded-virtual
  QMAKE_CXXFLAGS += -Wno-unused-parameter
  QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
  QMAKE_CXXFLAGS += -Wno-misleading-indentation
  QMAKE_CXXFLAGS += -Wno-unused-variable
}


win32-msvc* {
  QMAKE_CXXFLAGS -= -Zc:wchar_t-
  QMAKE_CXXFLAGS -= -w34100
  QMAKE_CXXFLAGS += -wd4800
  QMAKE_CXXFLAGS += -w44355
  QMAKE_CXXFLAGS += -wd4996
  QMAKE_CXXFLAGS += -wd4250
  QMAKE_CFLAGS_RELEASE = -Zi -MDd
  QMAKE_CXXFLAGS += -MP8
}

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
