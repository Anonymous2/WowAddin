#include "stdafx.h"
#include <sstream>
#include <list>
#include <vector>
#include <map>

BOOL CCommand_TestCommand(char const* cmd, char const* args)
{
    Console::Write("Hello from TestCommand: cmd '%s', args '%s'", INPUT_COLOR, cmd, args);

    uint64 guid = ObjectMgr::GetActivePlayerGuid();

	if (!guid)
	{
		Console::Write("TestCommand: Not in world!", ERROR_COLOR);
		return TRUE;
	}

    CGObject_C *pPlayer = ObjectMgr::GetObjectPtr(guid, TYPEMASK_PLAYER);

    C3Vector pos;
    pPlayer->GetPosition(pos);

    const char *model;
    BOOL result = pPlayer->GetModelFileName(&model);

    Console::Write("Local player: position x: %f y: %f z: %f, model '%s'", ECHO_COLOR, pos.X, pos.Y, pos.Z, result ? model : "Unknown");

    CGObject_C *pTarget = ObjectMgr::GetObjectPtr(pPlayer->GetValue<uint64>(UNIT_FIELD_TARGET), TYPEMASK_UNIT);

    if (pTarget)
        Console::Write("Target '%s', guid 0x%016llX", ECHO_COLOR, pTarget->GetObjectName(), pTarget->GetValue<uint64>(OBJECT_FIELD_GUID));
    else
        Console::Write("No target!", ECHO_COLOR);

    return TRUE;
}

BOOL CCommand_Beastmaster(char const* cmd, char const* args)
{
    CDataStore data(CMSG_BEASTMASTER);
    int state = _strnicmp(args, "off", INT_MAX) != 0;
    data.PutInt8(state);
    ClientServices::SendPacket(&data);

    Console::Write("Beastmaster mode is %s", ECHO_COLOR, state ? "on" : "off");

    return TRUE;
}

BOOL CCommand_Invis(char const* cmd, char const* args)
{
    CDataStore data(CMSG_GM_INVIS);
    int state = _strnicmp(args, "off", INT_MAX) != 0;
    data.PutInt32(state);
    ClientServices::SendPacket(&data);

    Console::Write("GM invis mode is %s", ECHO_COLOR, state ? "on" : "off");

    return TRUE;
}

BOOL CCommand_DBLookup(char const* cmd, char const* args)
{
    CDataStore data(CMSG_DBLOOKUP);
    data.PutString(args);
    ClientServices::SendPacket(&data);

    return TRUE;
}

BOOL ShowObjectsEnumProc(uint64 objectGuid, void *param)
{
    CGObject_C *pObject = ObjectMgr::GetObjectPtr(objectGuid, TYPEMASK_OBJECT);

    if (pObject)
		Console::Write("Object '%s', guid 0x%016llX", HIGHLIGHT_COLOR, pObject->GetObjectName(), pObject->GetValue<uint64>(OBJECT_FIELD_GUID));

    return TRUE;
}

BOOL CCommand_ShowObjects(char const* cmd, char const* args)
{
	if (!ObjectMgr::IsInWorld())
	{
		Console::Write("Error: Not in world!", ERROR_COLOR);
		return TRUE;
	}

    ObjectMgr::EnumVisibleObjects(ShowObjectsEnumProc, NULL);

    return TRUE;
}

BOOL CCommand_CreateGuildCommand(char const* cmd, char const* args)
{
    CDataStore data;
    data.PutInt32(CMSG_GUILD_CREATE);
    std::string guildname = std::string(args);
    data.PutString(guildname.c_str());
    ClientServices::SendPacket(&data);

    std::ostringstream ss;
    ss << "Sent CMSG_GUILD_CREATE with guildname " << guildname.c_str() << ". Args: " << args;
    Console::Write(ss.str().c_str(), ECHO_COLOR);
    return true;
}

BOOL CCommand_SendIgnoreCommand(char const* cmd, char const* args)
{
    //! Send '<name> is ignoring you.' to target X times (this message can't be blocked).
    long floodCount = atoi(args);

    for (long i = 0; i < floodCount; ++i)
    {
        CDataStore data;
        data.PutInt32(CMSG_CHAT_IGNORED);

        if (CGObject_C* player = ObjectMgr::GetObjectPtr(ObjectMgr::GetActivePlayerGuid(), TYPEMASK_PLAYER))
        {
            CGObject_C* target = ObjectMgr::GetObjectPtr(player->GetValue<uint64>(UNIT_FIELD_TARGET), TYPEMASK_PLAYER);

            if (!target)
                target = ObjectMgr::GetObjectPtr(player->GetValue<uint64>(UNIT_FIELD_TARGET), TYPEMASK_UNIT);

            if (target)
                data.PutInt64(target->GetValue<uint64>(OBJECT_FIELD_GUID)); // guid
            else
                data.PutInt64(ObjectMgr::GetActivePlayerGuid()); // guid (just send own, we need to send one..)
        }
        else
            data.PutInt64(ObjectMgr::GetActivePlayerGuid()); // guid (just send own, we need to send one..)

        data.PutInt8(1); // unk
        

        ClientServices::SendPacket(&data);
        Console::Write("CMSG_CHAT_IGNORED", ECHO_COLOR);
    }
    return true;
}
