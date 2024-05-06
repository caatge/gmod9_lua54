#include "iserverplugin.h"
#include "eiface.h"
#include "interface.h"

#include "filesystem.h"
extern IFileSystem* g_pFullFileSystem;

void InitCLuaHooks();
void InitCAPIHooks();

class CVSPlugin : public IServerPluginCallbacks
{
public:
	CVSPlugin() : m_iClientCommandIndex(0) {};
	~CVSPlugin() {};

	virtual bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
	virtual void			Unload(void);
	virtual void			Pause(void) {};
	virtual void			UnPause(void) {};
	virtual const char* GetPluginDescription(void) { return "Did I ever tell you what the definition of insanity is? Insanity is doing the exact... same fucking thing... over and over again expecting... shit to change... That. Is. Crazy. The first time somebody told me that, I dunno, I thought they were bullshitting me, so, I shot him. The thing is... He was right. And then I started seeing, everywhere I looked, everywhere I looked all these fucking pricks, everywhere I looked, doing the exact same fucking thing... over and over and over and over again thinking 'this time is gonna be different' no, no, no please... This time is gonna be different"; };
	virtual void			LevelInit(char const* pMapName) {};
	virtual void			ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) {};
	virtual void			GameFrame(bool simulating) {};
	virtual void			LevelShutdown(void) {};
	virtual void			ClientActive(edict_t* pEntity) {};
	virtual void			ClientDisconnect(edict_t* pEntity) {};
	virtual void			ClientPutInServer(edict_t* pEntity, char const* playername) {};
	virtual void			SetCommandClient(int index) { m_iClientCommandIndex = index; };
	virtual void			ClientSettingsChanged(edict_t* pEdict) {};
	virtual PLUGIN_RESULT	ClientConnect(bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) { return PLUGIN_CONTINUE; };
	virtual PLUGIN_RESULT	ClientCommand(edict_t* pEntity) { return PLUGIN_CONTINUE; };
	virtual PLUGIN_RESULT	NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) { return PLUGIN_CONTINUE; };

	virtual int             GetCommandIndex() { return m_iClientCommandIndex; }
private:
	int m_iClientCommandIndex;
};