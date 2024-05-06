#include "main.hpp"
#include "filesystem.h"
#include "tier2/tier2.h"

CVSPlugin g_VSPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CVSPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_VSPlugin);

#define fivefour
bool CVSPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
    ConnectTier2Libraries(&interfaceFactory, 1);
#ifdef fivefour
    InitCLuaHooks();
    InitCAPIHooks();
#endif
	return true;
}

void CVSPlugin::Unload(void)
{
}