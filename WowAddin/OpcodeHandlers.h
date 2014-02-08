#pragma once

void SetMessageHandlers();
BOOL LookupResultsHandler(void *param, NETMESSAGE msgId, uint32 time, CDataStore *msg);
BOOL TestCommandHandler(void *param, NETMESSAGE msgId, uint32 time, CDataStore *msg);
