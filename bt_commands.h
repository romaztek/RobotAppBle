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

    Q_INVOKABLE QString connectedCommand() { return  QStringLiteral(u"0"); }
    Q_INVOKABLE QString disconnectedCommand() { return  QStringLiteral(u"$"); }

    Q_INVOKABLE QString driveLeftCommandLow() { return  QStringLiteral(u"1"); }
    Q_INVOKABLE QString driveLeftCommandNormal() { return  QStringLiteral(u"2"); }
    Q_INVOKABLE QString driveLeftCommandHigh() { return  QStringLiteral(u"3"); }
    Q_INVOKABLE QString driveLeftCommandStrong() { return  QStringLiteral(u"4"); }

    Q_INVOKABLE QString driveRightCommandLow() { return  QStringLiteral(u"5"); }
    Q_INVOKABLE QString driveRightCommandNormal() { return  QStringLiteral(u"6"); }
    Q_INVOKABLE QString driveRightCommandHigh() { return  QStringLiteral(u"7"); }
    Q_INVOKABLE QString driveRightCommandStrong() { return  QStringLiteral(u"8"); }

    Q_INVOKABLE QString centerCommand() { return  QStringLiteral(u"9"); }
    Q_INVOKABLE QString forwardCommand() { return  QStringLiteral(u"q"); }
    Q_INVOKABLE QString backwardCommand() { return  QStringLiteral(u"w"); }
    Q_INVOKABLE QString driveStopCommand() { return  QStringLiteral(u"e"); }

    Q_INVOKABLE QString servoStopCommand() { return  QStringLiteral(u"r"); }
    Q_INVOKABLE QString servoReturnToDefaultCommand() { return  QStringLiteral(u"t"); }

    Q_INVOKABLE QString servoHelloCommand() { return  QStringLiteral(u"y"); }
    Q_INVOKABLE QString servoDanceCommand() { return  QStringLiteral(u"u"); }
    Q_INVOKABLE QString servoByeCommand() { return  QStringLiteral(u"i"); }

    Q_INVOKABLE QString headUpCommand() { return  QStringLiteral(u"("); }
    Q_INVOKABLE QString headDownCommand() { return  QStringLiteral(u")"); }
    Q_INVOKABLE QString headLeftCommand() { return  QStringLiteral(u"-"); }
    Q_INVOKABLE QString headRightCommand() { return  QStringLiteral(u"+"); }
    Q_INVOKABLE QString headDefaultCommand() { return  QStringLiteral(u"_"); }

    Q_INVOKABLE QString faceImage1Command() { return  QStringLiteral(u"o"); }
    Q_INVOKABLE QString faceImage2Command() { return  QStringLiteral(u"p"); }
    Q_INVOKABLE QString faceImage3Command() { return  QStringLiteral(u"a"); }


    Q_INVOKABLE QString audio1Command() { return  QStringLiteral(u"!"); }
    Q_INVOKABLE QString audio2Command() { return  QStringLiteral(u"@"); }
    Q_INVOKABLE QString audio3Command() { return  QStringLiteral(u"#"); }


    static void init();

};

#endif // BT_COMMANDS_H
