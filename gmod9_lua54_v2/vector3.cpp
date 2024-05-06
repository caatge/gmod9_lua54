#include "cmath"
#include "vector3.h"
// shoutouts to ida pro

typedef int(__cdecl* luaopen_v3_t)(lua_State* L);

luaopen_v3_t ogvec3 = NULL;

double* Pnew(lua_State* L)
{
	double* vec; // [esp+14h] [ebp-4h]

	vec = (double*)lua_newuserdata(L, 24);
	luaL_getmetatable(L, "vector3");
	lua_setmetatable(L, -2);
	return vec;
}

double* l_vecget(lua_State* L, int nArg)
{
	if (!luaL_checkudata(L, nArg, "vector3"))
		luaL_typeerror(L, nArg, "vector3");
	return (double*)lua_touserdata(L, nArg);
}

// called on vector3(x,y,z)
int vecnew(lua_State* L)
{
	lua_settop(L, 3);
	double* vec = Pnew(L);
	*vec = luaL_optnumber(L, 1, 0.0);
	vec[1] = luaL_optnumber(L, 2, 0.0);
	vec[2] = luaL_optnumber(L, 3, 0.0);
	return 1;
}

// garry thought its a good idea to keep colors as vector3...
// called on __index vector3[index], vector3.index etc
int vecgetindex(lua_State* L)
{
	double* vec;
	const char* field = luaL_checkstring(L, 2);
	vec = l_vecget(L, 1);
	switch (field[0])
	{
	case '1':
	case 'r':
	case 'x':
		lua_pushnumber(L, *vec);
		break;
	case '2':
	case 'g':
	case 'y':
		lua_pushnumber(L, vec[1]);
		break;
	case '3':
	case 'b':
	case 'z':
		lua_pushnumber(L, vec[2]);
		break;
	default:
		luaL_getmetatable(L, "vector3"); // we lookup the table here for stuff like vector3:Length() etc
		lua_pushvalue(L, 2);
		lua_gettable(L, -2);
		break;
	}
	return 1;
}

// called on newindex (when new value is assigned to array index), example: vector3[index] = 123
int vecsetindex(lua_State* L)
{
	double* vec = l_vecget(L, 1);
	const char* field = luaL_checklstring(L, 2, 0);
	double newval = luaL_checknumber(L, 3);
	switch (field[0])
	{
	case '1':
	case 'r':
	case 'x':
		*vec = newval;
		break;
	case '2':
	case 'g':
	case 'y':
		vec[1] = newval;
		break;
	case '3':
	case 'b':
	case 'z':
		vec[2] = newval;
		break;
	default:
		return 0;
	}
	return 0;
}

// called when vector3 is converted to string
int vectostring(lua_State* L)
{
	char s[64];
	double* vec = l_vecget(L, 1);
	sprintf_s(s, "%.2f %.2f %.2f", *vec, vec[1], vec[2]);
	lua_pushstring(L, s);
	return 1;
}

int vecadd(lua_State* L)
{
	double* vec1 = (double*)l_vecget(L, 1);
	double* vec2 = (double*)l_vecget(L, 2);
	double* vec = (double*)Pnew(L);
	vec[0] = vec1[0] + vec2[0];
	vec[1] = vec1[1] + vec2[1];
	vec[2] = vec1[2] + vec2[2];
	return 1;
}

int vecsub(lua_State* L)
{
	double* vec1 = (double*)l_vecget(L, 1);
	double* vec2 = (double*)l_vecget(L, 2);
	double* vec = (double*)Pnew(L);
	vec[0] = vec1[0] - vec2[0];
	vec[1] = vec1[1] - vec2[1];
	vec[2] = vec1[2] - vec2[2];
	return 1;
}

int vecdiv(lua_State* L)
{
	double* vec1 = l_vecget(L, 1);
	double div = luaL_checknumber(L, 2);
	double* vec = Pnew(L);
	vec[0] = vec1[0] / div;
	vec[1] = vec1[1] / div;
	vec[2] = vec1[2] / div;
	return 1;
}

int vecmul(lua_State* L)
{
	double* vec1 = l_vecget(L, 1);
	double mul = luaL_checknumber(L, 2);
	double* vec = Pnew(L);
	vec[0] = vec1[0] * mul;
	vec[1] = vec1[1] * mul;
	vec[2] = vec1[2] * mul;
	return 1;
}

int veclength(lua_State* L)
{
	double* vec = l_vecget(L, 1);
	lua_pushnumber(L, sqrt((*vec * *vec + vec[1] * vec[1] + vec[2] * vec[2])));
	return 1;
}

int _vecadd(lua_State* L)
{
	double* vec1 = l_vecget(L, 1);
	double* vec2 = l_vecget(L, 2);
	vec1[0] = vec1[0] + vec2[0];
	vec1[1] = vec1[1] + vec2[1];
	vec1[2] = vec1[2] + vec2[2];
	return 0;
}

int _vecmul(lua_State* L)
{
	double* vec1 = l_vecget(L, 1);
	double mul = luaL_checknumber(L, 2);
	vec1[0] = vec1[0] * mul;
	vec1[1] = vec1[1] * mul;
	vec1[2] = vec1[2] * mul;
	return 0;
}

int _vecsub(lua_State* L)
{
	double* vec1 = l_vecget(L, 1);
	double* vec2 = l_vecget(L, 2);
	vec1[0] = vec1[0] - vec2[0];
	vec1[1] = vec1[1] - vec2[1];
	vec1[2] = vec1[2] - vec2[2];
	return 0;
}

int _vecdiv(lua_State* L)
{
	double* vec1 = l_vecget(L, 1);
	double div = luaL_checknumber(L, 2);
	vec1[0] = vec1[0] / div;
	vec1[1] = vec1[1] / div;
	vec1[2] = vec1[2] / div;
	return 0;
}

int vecdot(lua_State* L)
{
	double* vec1 = l_vecget(L, 1);
	double* vec2 = l_vecget(L, 2);
	lua_pushnumber(L, (vec1[0] * vec1[0] + vec1[1] * vec1[1] + vec1[2] * vec1[2]));
	return 1;
}

int veccross(lua_State* L) {
	double* v1 = l_vecget(L, 1);
	double* v2 = l_vecget(L, 2);
	double* vec = Pnew(L);
	vec[0] = v1[1] * v2[2] - v1[2] * v2[1];
	vec[1] = v1[2] * v2[0] - v1[0] * v2[2];
	vec[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return 1;
}

int vecnormalize(lua_State* L) {
	double* vec = l_vecget(L, 1);
	double ilength = 1.0 / sqrt((*vec * *vec + vec[1] * vec[1] + vec[2] * vec[2]));
	double* vecnormed = Pnew(L);
	vecnormed[0] = vec[0] * ilength;
	vecnormed[1] = vec[1] * ilength;
	vecnormed[2] = vec[2] * ilength;
	return 1;
}

int veceq(lua_State* L) {
	//__eq
	double* v1 = l_vecget(L, 1);
	double* v2 = l_vecget(L, 2);
	if (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2]) {
		lua_pushboolean(L, 1);
		return 1;
	}
	lua_pushboolean(L, 0);
	return 1;
}

struct luaL_Reg l_vectorreg[] = { // stuff prefixed with _ doesn't create a new structure!
	{"__newindex", vecsetindex},
	{"__tostring", vectostring},
	{"__index", vecgetindex},
	{"__add", vecadd},
	{"__sub", vecsub},
	{"__div", vecdiv},
	{"__mul", vecmul},
	{"__eq", veceq},
	{"Length", veclength},
	{"Add", _vecadd},
	{"Multiply", _vecmul},
	{"Substract", _vecsub},
	{"Divide", _vecdiv},
	{"Dot", vecdot},
	{"Cross", veccross},
	{"Normalize", vecnormalize},
	{NULL,NULL}
};

int __cdecl reg_v3(lua_State* L) {
	luaL_newmetatable(L, "vector3");
	luaL_setfuncs(L, l_vectorreg, 0);
	lua_pushcclosure(L, vecnew, 0);
	lua_setglobal(L, "vector3");
	return 1;
}