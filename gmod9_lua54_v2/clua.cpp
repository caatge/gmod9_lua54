#include "clua.hpp"
#include "stdtools.h"
#include "cluaptrs.h"
#include "vector3.h"
#include "main.hpp"
#include "stdint.h"

int(__cdecl* ENTINDEX)(void* ent) = (int(__cdecl*)(void*))0x2226D970;

void* (__cdecl* UTIL_EntityByIndex)(int id) = (void* (__cdecl*)(int))0x2226D930;
void* (__cdecl* UTIL_PlayerByIndex)(int id) = (void* (__cdecl*)(int))0x2226D7C0;

void* (__cdecl* _dynamic_cast)(void*, int, void*, void*, int) = (void* (__cdecl*)(void*, int, void*, void*, int))0x2242C136;

void* m_LuaImplementations = (void*)0x22665D74;
void** (__thiscall* emplace_map_real)(void* t, void* b) = (void** (__thiscall*)(void*, void*))0x22415900;

void(__thiscall* map_find_orsomeshit)(void* this_, void** objref, void* noidea) = (void(__thiscall*)(void* this_, void** objref, void* noidea))0x22414520;
void(__thiscall* map_find_orsomeshit2)(void* this_, void** objref, void* noidea) = (void(__thiscall*)(void* this_, void** objref, void* noidea))0x22414D80;


char* g_GMODFolder = (char*)0x2265BDD0; // max 264

char path[1024]{};
char cpath[1024]{};

int panic(lua_State* L) {
    CPrint(red, "Lua: panic %s\n", lua_tostring(L, -1));
    return 0;
}

int rgbprint(lua_State* L) {
    lua_getfield(L, 1, "r");
    int r = lua_tointeger(L, -1);
    lua_getfield(L, 1, "g");
    int g = lua_tointeger(L, -1);
    lua_getfield(L, 1, "b");
    int b = lua_tointeger(L, -1);
    CPrint(Color(r, g, b), lua_tostring(L, 2));
    return 0;
}
struct luaL_Reg reg[] = {
    {"rgbprint", rgbprint},
    {NULL,NULL},
};
#define L this->L

void CLua::Startup() {
    CPrint(Color(255, 128, 255), "CLua::Startup\n");
    L = lua_newstate((lua_Alloc)0x224208C0, 0);
    *emplace_map_real(m_LuaImplementations, this) = this;
    luaL_openlibs(L);
    reg_v3(L);
    GLUA_BindFunctions(this, L);
    lua_atpanic(L, panic);
    luaL_newlib(L, reg);
    lua_setglobal(L, "misc");
    lua_getglobal(L, "package");
    lua_pushstring(L, path);
    lua_setfield(L, -2, "path");
    lua_pushstring(L, cpath);
    lua_setfield(L, -2, "cpath");
    lua_settop(L, 0);
}

void CLua::Shutdown() {
    int v3;
    map_find_orsomeshit(m_LuaImplementations, (void**)&v3, (void*)this);
    map_find_orsomeshit2(m_LuaImplementations, (void**)&v3, (void*)v3);
    lua_close(L);
    L = NULL;
    //return 0;
}

bool CLua::RunScript(const char* script_path) {
    char p[MAX_PATH];
    strcpy(p, script_path);
    FileHandle_t f = g_pFullFileSystem->Open(p, "rb", 0);
    if (!f) {
        Q_snprintf(p, 260, "lua/%s", script_path);
        f = g_pFullFileSystem->Open(p, "rb", 0);
        if (!f) {
            CPrint(red, "Couldn't open script %s\n", p);
            return 0;
        }
    }
    unsigned long size = g_pFullFileSystem->Size(f);
    char* mem = (char*)MemAllocScratch(size + 1);
    g_pFullFileSystem->Read(mem, size, f);
    *((char*)mem + size) = 0;
    g_pFullFileSystem->Close(f);
    if (luaL_dostring(L, mem)) {
        CPrint(red, "Lua: Errors when running script '%s'\n", script_path);
        MemFreeScratch();
        return 0;
    }
    MemFreeScratch();
    return 1;
}


bool CLua::RunFunction(char* name, int results) {
    lua_getglobal(L, name);
    if (lua_pcall(L, 0, results, 0)) {
        CPrint(pink, "CLua::RunFunction (%s) failed!\n", name);
        CPrint(red, lua_tostring(L, -1));
        Msg("\n");
        return false;
    }
    return true;
}

void CLua::StartFunction(char* name) {
    lua_getglobal(L, name);
}

bool CLua::RunString(const char* str) {
    if (str) {
        return luaL_dostring(L, str) == 0;
    }
    return 0;
}

void CLua::CallFunctionWithArgs(const char* name, int a3, int a4)
{
    int result = lua_pcall(L, a3, a4, 0);
    if (result)
    {
        CPrint(red, "Lua: Error calling '%s' : '%s'\n", name, lua_tostring(L, -1));
        lua_settop(L, -2);
    }
}

// AddArgument
void CLua::AddArgument(const char* str) {
    lua_pushstring(L, str);
}

void CLua::AddArgument(float n) {
    lua_pushnumber(L, n);
}

void CLua::AddArgument(bool b) {
    lua_pushboolean(L, b);
}

void CLua::AddArgument(int n) {
    lua_pushinteger(L, n);
}

void CLua::AddArgument(CBaseEntity* ent) {
    lua_pushinteger(L, ENTINDEX(ent));
}

void CLua::AddArgument(float v1, float v2, float v3) {
    double* v = Pnew(L);
    *v = v1;
    v[1] = v2;
    v[2] = v3;
}

//Get
bool CLua::GetBool(int pos) {
    if (!lua_type(L, pos))
        return 0;
    bool v4 = lua_toboolean(L, pos);
    if (pos < 0)
        lua_settop(L, -2);
    return v4;
}

CBaseEntity* CLua::GetEnt(int pos) {

    if (!lua_type(L, pos))
        return 0;
    int v3 = luaL_checkinteger(L, pos);
    if (pos < 0)
        lua_settop(L, -2);
    if (v3)
        return (CBaseEntity*)UTIL_EntityByIndex(v3);
    else
        return 0;
}

double CLua::GetFloat(int pos) {
    double v5;
    if (!lua_type(L, pos)) {
        return 0.0;
    }
    v5 = lua_tonumber(L, pos);
    if (pos < 0)
        lua_settop(L, -2);
    return v5;
}

int CLua::GetInt(int pos)
{
    int result;
    int v4;

    result = lua_type(L, pos);
    if (result)
    {
        v4 = (unsigned __int64)lua_tonumber(L, pos);
        if (pos < 0)
            lua_settop(L, -2);
        return v4;
    }
    return result;
}

CBasePlayer* CLua::GetPlayer(int pos)
{
    int v3;
    void* v4;

    if (!lua_type(L, pos))
        return 0;
    v3 = lua_tointeger(L, pos);
    if (pos < 0)
        lua_settop(L, -2);
    if (v3
        && v3 <= 32
        && (v4 = UTIL_PlayerByIndex(v3)) != 0
        && ((CallVirtualFunc<70, bool>(v4))))
    {
        return (CBasePlayer*)_dynamic_cast(v4, 0, (void*)0x22585EB0, (void*)0x2258BD20, 0);
    }
    else
    {
        return 0;
    }
}

const char* CLua::GetString(int pos)
{
    const char* v4; // eax

    if (!lua_isstring(L, pos))
        return "";
    if (pos >= 0)
        return (const char*)lua_tostring(L, pos);
    memset(&tempstorage, 0, 0x2000u);
    v4 = lua_tostring(L, pos);
    Q_strncpy(tempstorage, v4, 0x2000);
    lua_settop(L, -2);
    return tempstorage;
}

float* CLua::GetVector(float* v, int pos)
{
    float* result; // eax
    double* v5; // eax
    float v6; // [esp+8h] [ebp-Ch]
    float v7; // [esp+Ch] [ebp-8h]
    float v8; // [esp+10h] [ebp-4h]

    if (lua_type(L, pos))
    {
        v5 = (double*)l_vecget(L, pos);
        v6 = *v5;
        v7 = v5[1];
        v8 = v5[2];
        if (pos < 0)
            lua_settop(L, -2);
        result = v;
        *v = v6;
        v[1] = v7;
        v[2] = v8;
    }
    else
    {
        result = v;
        *v = 0.0;
        v[1] = 0.0;
        v[2] = 0.0;
    }
    return result;
}

CBaseCombatCharacter* CLua::GetCombatCharacter(int pos)
{
    void* v4; // eax

    if (!lua_type(L, pos))
        return 0;
    int v3 = lua_tointeger(L, pos);
    if (pos < 0)
        lua_settop(L, -2);
    if (v3 && (v4 = UTIL_EntityByIndex(v3)) != 0)
        return CallVirtualFunc<62, CBaseCombatCharacter*>(v4);
    else
        return 0;
}

//

int CLua::NumArgs() {
    return lua_gettop(L);
}


// AddReturn
void CLua::AddReturn(int a)
{
    lua_pushinteger(L, a);
}

void CLua::AddReturn(float a)
{
    lua_pushnumber(L, a);
}

void CLua::AddReturn(bool a)
{
    lua_pushboolean(L, a);
}

void CLua::AddReturn(CBaseEntity* b)
{
    lua_pushinteger(L, ENTINDEX(b));
}

void CLua::AddReturn(const char* b)
{
    lua_pushstring(L, b);
}

void CLua::AddReturn(OldVectorReader<float> v)
{
    lua_newtable(L);
    for (int i = 0; i < v.Length(); i++) {
        lua_pushinteger(L, i + 1);
        lua_pushinteger(L, *v.Get(i));
        lua_settable(L, -3);
    }
}

void CLua::AddReturn(OldVectorReader<OldString> v) {
    lua_newtable(L);
    for (int i = 0; i < v.Length(); i++) {
        lua_pushinteger(L, i + 1);
        lua_pushstring(L, v.Get(i)->GetStringData());
        lua_settable(L, -3);
    }
}

void CLua::AddReturn(float v1, float v2, float v3) {
    double* v = Pnew(L);
    v[0] = v1;
    v[1] = v2;
    v[2] = v3;
}
//

// SetGlobal
void CLua::SetGlobal(const char* n, float v) {
    lua_pushnumber(L, v);
    lua_setglobal(L, n);
}

void CLua::SetGlobal(const char* n, int v) {
    lua_pushinteger(L, v);
    lua_setglobal(L, n);
}

void CraftRequirePaths() {
    Q_snprintf(path, sizeof(path), "%s\\lua\\?.lua;%s\\lua\\?\\init.lua;.\\?.lua;.\\?\\init.lua", g_GMODFolder, g_GMODFolder);
    Q_snprintf(cpath, sizeof(cpath), "%s\\lua\\?.dll;.\\?.dll", g_GMODFolder);
}

void InitCLuaHooks() {

    CraftRequirePaths();

    HookMethod(CLua_Startup, &CLua::Startup);
    HookMethod(CLua_Shutdown, &CLua::Shutdown);
    HookMethod(CLua_RunScript, &CLua::RunScript);
    HookMethod(CLua_RunFunction, &CLua::RunFunction);
    HookMethod(CLua_StartFunction, &CLua::StartFunction);
    HookMethod(CLua_RunString, &CLua::RunString);
    HookMethod(CLua_CallFunctionWithArgs, &CLua::CallFunctionWithArgs);
    HookMethod(CLua_NumArgs, &CLua::NumArgs);
    // AddArgument
    HookMethod(CLua_AddArgument_cstring, static_cast<void(CLua::*)(const char*)>(&CLua::AddArgument));
    HookMethod(CLua_AddArgument_int, static_cast<void(CLua::*)(int)>(&CLua::AddArgument));
    HookMethod(CLua_AddArgument_float, static_cast<void(CLua::*)(float)>(&CLua::AddArgument));
    HookMethod(CLua_AddArgument_bool, static_cast<void(CLua::*)(bool)>(&CLua::AddArgument));
    HookMethod(CLua_AddArgument_cbaseentity, static_cast<void(CLua::*)(CBaseEntity*)>(&CLua::AddArgument));
    HookMethod(CLua_AddArgument_vector, static_cast<void(CLua::*)(float,float,float)>(&CLua::AddArgument));
    HookMethod(CLua_AddArgument_cstring, static_cast<void(CLua::*)(const char*)>(&CLua::AddArgument));
    // SetGlobal
    HookMethod(CLua_SetGlobal_float, static_cast<void(CLua::*)(const char*,float)>(&CLua::SetGlobal));
    HookMethod(CLua_SetGlobal_int, static_cast<void(CLua::*)(const char*,int)>(&CLua::SetGlobal));
    // Get
    HookMethod(CLua_GetBool, &CLua::GetBool);
    HookMethod(CLua_GetEnt, &CLua::GetEnt);
    HookMethod(CLua_GetFloat, &CLua::GetFloat);
    HookMethod(CLua_GetInt, &CLua::GetInt);
    HookMethod(CLua_GetPlayer, &CLua::GetPlayer);
    HookMethod(CLua_GetString, &CLua::GetString);
    HookMethod(CLua_GetVector, &CLua::GetVector);
    HookMethod(CLua_GetCombatCharacter, &CLua::GetCombatCharacter);
    // AddReturn
    HookMethod(CLua_AddReturn_int, static_cast<void(CLua::*)(int)>(&CLua::AddReturn));
    HookMethod(CLua_AddReturn_float, static_cast<void(CLua::*)(float)>(&CLua::AddReturn));
    HookMethod(CLua_AddReturn_bool, static_cast<void(CLua::*)(bool)>(&CLua::AddReturn));
    HookMethod(CLua_AddReturn_cbaseent, static_cast<void(CLua::*)(CBaseEntity*)>(&CLua::AddReturn));
    HookMethod(CLua_AddReturn_cstring, static_cast<void(CLua::*)(const char*)>(&CLua::AddReturn));
    HookMethod(CLua_AddReturn_floatvector, static_cast<void(CLua::*)(OldVectorReader<float>)>(&CLua::AddReturn));
    HookMethod(CLua_AddReturn_stringvector, static_cast<void(CLua::*)(OldVectorReader<OldString>)>(&CLua::AddReturn));
    HookMethod(CLua_AddReturn_vector, static_cast<void(CLua::*)(float,float,float)>(&CLua::AddReturn));
}