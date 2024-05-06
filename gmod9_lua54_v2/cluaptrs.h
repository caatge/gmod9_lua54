extern  void* CLua_ScriptExists = (void*)0x22412360;
extern  void* CLua_RunString = (void*)0x22412400;

extern  void* CLua_RunFunction = (void*)0x22412430;

extern  void* CLua_StartFunction = (void*)0x22412470;
extern  void* CLua_CallFunctionWithArgs = (void*)0x224124A0;

extern  void* CLua_AddArgument_cstring = (void*)0x224124F0;
extern  void* CLua_AddArgument_int = (void*)0x22412550;
extern  void* CLua_AddArgument_float = (void*)0x22412510;
extern  void* CLua_AddArgument_bool = (void*)0x22412530;
extern  void* CLua_AddArgument_cbaseentity = (void*)0x22412570;
extern  void* CLua_AddArgument_vector = (void*)0x224125A0;


extern  void* CLua_SetGlobal_float = (void*)0x224125D0;
extern  void* CLua_SetGlobal_int = (void*)0x22412660;
//CLua::SetGlobal cstring
//CLua::SetGlobal bool

extern  void* CLua_NumArgs = (void*)0x224126F0;

extern  void* CLua_GetString = (void*)0x22412700;
extern  void* CLua_GetInt = (void*)0x22412780;
extern  void* CLua_GetEnt = (void*)0x224127D0;
extern  void* CLua_GetPlayer = (void*)0x22412830;
extern  void* CLua_GetCombatCharacter = (void*)0x224128C0;
extern  void* CLua_GetFloat = (void*)0x224129A0;
extern  void* CLua_GetBool = (void*)0x224129F0;
extern  void* CLua_GetVector = (void*)0x22412A40;
//getanimating (not used)

extern  void* CLua_RunScript = (void*)0x224136F0;

extern  void* CLua_AddReturn_stringvector = (void*)0x22414160;
extern  void* CLua_AddReturn_floatvector = (void*)0x224141F0;
extern  void* CLua_AddReturn_cbaseent = (void*)0x22412B80;
extern  void* CLua_AddReturn_vector = (void*)0x22412B50;
extern  void* CLua_AddReturn_bool = (void*)0x22412AF0;
extern  void* CLua_AddReturn_float = (void*)0x22412B10;
extern  void* CLua_AddReturn_cstring = (void*)0x22412B30;
extern  void* CLua_AddReturn_int = (void*)0x22412AD0;


extern  void* CLua_Shutdown = (void*)0x22415340;
extern  void* CLua_Startup = (void*)0x22415A30;

extern void* CC_LUA_RunCommand = (void*)0x22412BB0;
void (__cdecl* GLUA_BindFunctions)(void*, void*) = (void (__cdecl*)(void*,void*))0x22414320;