#include "vector.h"
#include "lua.hpp"
#include "stdtools.h"
class CBaseEntity;
class CBasePlayer;
class CBaseCombatCharacter;

class CLua {
public:
	void	Startup();
	void	Shutdown();
	bool	RunScript(const char* script_path);
	bool	RunFunction(char* name, int results);
	void	StartFunction(char* name);
	bool	RunString(const char* str);
	void	CallFunctionWithArgs(const char* funcname, int args, int res);
	int		NumArgs();
	// AddArgument
	void	AddArgument(const char*);
	void	AddArgument(float);
	void	AddArgument(bool);
	void	AddArgument(int);
	void	AddArgument(CBaseEntity*);
	void	AddArgument(float v1, float v2, float v3);
	// Get
	bool					GetBool(int stackpos);
	CBaseEntity*			GetEnt(int stackpos);
	double					GetFloat(int stackpos); // nice float
	int						GetInt(int stackpos);
	CBasePlayer*			GetPlayer(int stackpos);
	const char*				GetString(int stackpos);
	float*					GetVector(float* vec, int stackpos);
	CBaseCombatCharacter*	GetCombatCharacter(int stackpos);
	// AddReturn
	void	AddReturn(int);
	void	AddReturn(float);
	void	AddReturn(bool);
	void	AddReturn(CBaseEntity*);
	void	AddReturn(const char*);
	void	AddReturn(OldVectorReader<float> floatv);
	void	AddReturn(OldVectorReader<OldString>);
	void	AddReturn(float v1, float v2, float v3);
	// SetGlobal
	void	SetGlobal(const char* n, float v);
	void	SetGlobal(const char* n, int v);
	lua_State* L;
	char tempstorage[0x2000];
};