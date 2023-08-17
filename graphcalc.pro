QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17 
RESOURCES = icons/icons.qrc

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    graphics/widget.cpp \
		graphics/numbutton.cpp \
		graphics/lineedit.cpp \
		graphics/action.cpp \
		QCustomPlot/qcustomplot.cpp \
		graphics/plot.cpp

HEADERS += \
		graphics/widget.h \
		graphics/numbutton.h \
		graphics/lineedit.h \
		graphics/action.h \
		QCustomPlot/qcustomplot.h \
		graphics/plot.h

LIBS += \
		-L. backend.a \
		-lm

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
