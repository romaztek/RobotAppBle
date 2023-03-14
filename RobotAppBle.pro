
TARGET = RobotAppBle

QT += bluetooth quick

SOURCES += main.cpp \
           btcontroller.cpp \
           logic.cpp

HEADERS += btcontroller.h \
           logic.h

RESOURCES += qml.qrc

DISTFILES += main.qml \
             ConnectWindow.qml \
             ControlWindow.qml \
             CommandButton.qml
