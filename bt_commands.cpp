#include "bt_commands.h"

Bt_Commands::Bt_Commands()
{

}

void Bt_Commands::init()
{
    qmlRegisterType<Bt_Commands>("ru.romankartashev.bt_commands", 1, 0, "Bt_Commands");
}
