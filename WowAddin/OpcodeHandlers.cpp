#include "stdafx.h"

void SetMessageHandlers()
{
    //ClientServices::etMessageHandler(SMSG_DBLOOKUP, LookupResultsHandler, (void*)0xDEADBABE);
    ClientServices::SetMessageHandler(SMSG_PONG, TestCommandHandler, (void*)0xDEADBABE);
}

BOOL LookupResultsHandler(void *param, NETMESSAGE msgId, uint32 time, CDataStore *msg)
{
    char received[256];

    msg->GetString(received, sizeof(received));

    Console::Write("Hello from LookupResultsHandler! param %08X time %u ticks %u received %s", ECHO_COLOR, param, time, GetTickCount(), received);

    return TRUE;
}

BOOL TestCommandHandler(void *param, NETMESSAGE msgId, uint32 time, CDataStore *msg)
{
    for (int i = 0; i < 50; ++i)
        Console::Write("received SMSG_PONG", ECHO_COLOR);

    int32 ping;
    msg->GetInt32(ping);
    Console::Write("SMSG_PONG received: param %08X time %u ticks %u received %u", ECHO_COLOR, param, time, GetTickCount(), ping);
    return TRUE;
}
