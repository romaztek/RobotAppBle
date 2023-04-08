#ifndef BT_COMMANDS_H
#define BT_COMMANDS_H

#include <QObject>
#include <QString>
#include <QQmlEngine>

class Bt_Commands : public QObject
{
    Q_OBJECT
public:
    Bt_Commands();

    Q_INVOKABLE QString connectedCommand() { return  QStringLiteral(u"cconn"); }

    Q_INVOKABLE QString driveLeftCommandLow() { return  QStringLiteral(u"lelow"); }
    Q_INVOKABLE QString driveLeftCommandNormal() { return  QStringLiteral(u"lenor"); }
    Q_INVOKABLE QString driveLeftCommandHigh() { return  QStringLiteral(u"lehgh"); }
    Q_INVOKABLE QString driveLeftCommandStrong() { return  QStringLiteral(u"lestg"); }

    Q_INVOKABLE QString driveRightCommandLow() { return  QStringLiteral(u"rilow"); }
    Q_INVOKABLE QString driveRightCommandNormal() { return  QStringLiteral(u"rinor"); }
    Q_INVOKABLE QString driveRightCommandHigh() { return  QStringLiteral(u"rihgh"); }
    Q_INVOKABLE QString driveRightCommandStrong() { return  QStringLiteral(u"ristg"); }

    Q_INVOKABLE QString centerCommand() { return  QStringLiteral(u"mcntr"); }
    Q_INVOKABLE QString forwardCommand() { return  QStringLiteral(u"mfrwd"); }
    Q_INVOKABLE QString backwardCommand() { return  QStringLiteral(u"mbkwd"); }
    Q_INVOKABLE QString driveStopCommand() { return  QStringLiteral(u"mstop"); }

    Q_INVOKABLE QString servoStopCommand() { return  QStringLiteral(u"sstop"); }
    Q_INVOKABLE QString servoReturnToDefaultCommand() { return  QStringLiteral(u"shome"); }
    Q_INVOKABLE QString servoHelloCommand() { return  QStringLiteral(u"shllo"); }
    Q_INVOKABLE QString servoDanceCommand() { return  QStringLiteral(u"sdnce"); }
    Q_INVOKABLE QString servoByeCommand() { return  QStringLiteral(u"sbbye"); }

    Q_INVOKABLE QString faceImage1Command() { return  QStringLiteral(u"fimg1"); }
    Q_INVOKABLE QString faceImage2Command() { return  QStringLiteral(u"fimg2"); }
    Q_INVOKABLE QString faceImage3Command() { return  QStringLiteral(u"fimg3"); }
    Q_INVOKABLE QString faceImage4Command() { return  QStringLiteral(u"fimg4"); }


    static void init();

};

#endif // BT_COMMANDS_H
