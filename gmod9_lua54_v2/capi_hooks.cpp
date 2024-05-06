#include "lua.hpp"
#include "windows.h"
#include "stdint.h"
#include "stdtools.h"

void* unmodified = (void*)0x22424817;
__declspec (naked) void lua_close_real(lua_State* L) {
	__asm {
        mov     eax, [esp + 4]
        mov     ecx, [eax + 10h]
        push dword ptr[unmodified]
        retn
	}
}

void lua_close_(lua_State* L) {
    static bool called = false;
    if (!called) {
        lua_close_real(L);
    }
    else {
        lua_close(L);
    }
    called = true;
}

void __cdecl lua_settop_(lua_State* L, int top) {
    lua_settop(L, top);
}

void __cdecl lua_pushstring_(lua_State* L, const char* s) {
    lua_pushstring(L, s);
}

void __cdecl lua_gettable_(lua_State* L, int i) {
    if (i == -10001) {
        const char* name = lua_tostring(L, -1);
        lua_pop(L, 1);
        lua_getglobal(L, name);
    }
}

void __cdecl lua_settable_(lua_State* L, int idx) {
    if (idx == -10001) {
        // stack
        // -1 top value
        // -2 key
        const char* key = lua_tostring(L, -2);
        lua_setglobal(L, key);
        lua_pop(L, 1);
    }
}

int __cdecl lua_type_(lua_State* L, int idx) {
    return lua_type(L, idx);
}

void __cdecl lua_rawset_(lua_State* L, int idx) {
    lua_rawset(L, idx);
}

void __cdecl lua_pushclosure_(lua_State* L, void* fn, int a) {
    lua_pushcclosure(L, (lua_CFunction)fn, a);
}

void __cdecl lua_insert_(lua_State* L, int idx) {
    lua_insert(L, idx);
}

void __cdecl lua_newtable_(lua_State* L) {  
    lua_newtable(L);
}

int __cdecl lua_gettop_(lua_State* L) {
    return lua_gettop(L);
}

const char* __cdecl lua_tostring_(lua_State* L, int p) {
    return lua_tostring(L, p);
}

int __cdecl lua_isstring_(lua_State* L, int p) {
    return lua_isstring(L, p);
}


int __cdecl lua_dostring_(lua_State* L, const char* s) {
    bool r = luaL_dostring(L, s);
    if (r) {
        CPrint(pink, "%s\n", lua_tostring(L, -1));
        CPrint(red, "lua_dostring error\n");
    }
    return r;
}

void* lua_settop_og =       (void*)0x224160F0;
void* lua_settable_og =     (void*)0x22416C10;
void* lua_rawset_og =       (void*)0x22416C50;
void* lua_pushstring_og =   (void*)0x22416870;
void* lua_pushclosure_og =  (void*)0x22416940;
void* lua_gettable_og =     (void*)0x22416A20;
void* lua_type_og =         (void*)0x224162A0;
void* lua_insert_og =       (void*)0x224161B0;
void* lua_newtable_og =     (void*)0x22416B10;
void* lua_gettop_og =       (void*)0x224160E0;
void* lua_tostring_og =     (void*)0x224165F0;
void* lua_isstring_og =     (void*)0x22416380;
void* lua_dostring_og =     (void*)0x22418180;
void* lua_close_og =        (void*)0x22424810;

void InitCAPIHooks() {
    jmphook(lua_settop_og, &lua_settop_);
    jmphook(lua_pushstring_og, &lua_pushstring_);
    jmphook(lua_gettable_og, &lua_gettable_);
    jmphook(lua_settable_og, &lua_settable_);
    jmphook(lua_type_og, &lua_type_);
    jmphook(lua_rawset_og, &lua_rawset_);
    jmphook(lua_pushclosure_og, &lua_pushclosure_);
    jmphook(lua_insert_og, &lua_insert_);
    jmphook(lua_newtable_og, &lua_newtable_);
    jmphook(lua_gettop_og, &lua_gettop_);
    jmphook(lua_tostring_og, &lua_tostring_);
    jmphook(lua_isstring_og, &lua_isstring_);
    jmphook(lua_dostring_og, &lua_dostring_);
    jmphook(lua_close_og, &lua_close_);
}