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

    Q_INVOKABLE QString stage1CommandOn() { return  QStringLiteral(u"d"); }
    Q_INVOKABLE QString stage2CommandOn() { return  QStringLiteral(u"f"); }
    Q_INVOKABLE QString stage3CommandOn() { return  QStringLiteral(u"g"); }
    Q_INVOKABLE QString stage4CommandOn() { return  QStringLiteral(u"h"); }
    Q_INVOKABLE QString stage5CommandOn() { return  QStringLiteral(u"j"); }
    Q_INVOKABLE QString stage6CommandOn() { return  QStringLiteral(u"k"); }

    Q_INVOKABLE QString stage1CommandOff() { return  QStringLiteral(u"l"); }
    Q_INVOKABLE QString stage2CommandOff() { return  QStringLiteral(u"z"); }
    Q_INVOKABLE QString stage3CommandOff() { return  QStringLiteral(u"x"); }
    Q_INVOKABLE QString stage4CommandOff() { return  QStringLiteral(u"c"); }
    Q_INVOKABLE QString stage5CommandOff() { return  QStringLiteral(u"v"); }
    Q_INVOKABLE QString stage6CommandOff() { return  QStringLiteral(u"b"); }

    Q_INVOKABLE QString stageAllCommandOff() { return  QStringLiteral(u"n"); }


    static void init();

};

#endif // BT_COMMANDS_H
