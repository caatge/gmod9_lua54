#ifndef STDTOOLS_H
#define STDTOOLS_H
#include "windows.h"
#include "stdint.h"
#include "Color.h"

extern Color pink;
extern Color red;
extern void CPrint(Color, const char* fmt, ...);

template <class T>
class OldVectorReader {
public:
    OldVectorReader(void* old_vector) {
        oldvector = old_vector;
    }
    T* Get(size_t index) {
        return reinterpret_cast<T*>(((char*)GetStartOfData()) + (sizeof(T) * index));
    }

    size_t Length() {
        return ((size_t)GetEndOfData() - (size_t)GetStartOfData()) / sizeof(T);
    }

    size_t DataBytes() {
        return (size_t)GetEndOfData() - (size_t)GetStartOfData();
    }

    void* GetStartOfData() {
        return *(void**)((unsigned long)oldvector + 4);
    }

    void* GetEndOfData() {
        return *(void**)((unsigned long)oldvector + 8);
    }
private:
    void* oldvector;
};

class OldString {
public:
    const char* GetStringData() {
        if (*(unsigned long*)((char*)this + 24) < 0x10u) {
            return (const char*)((char*)this + 4);
        }
        else {
            return *(const char**)((char*)this + 4);
        }
    }
private:
    char _[28];
};
template <size_t Index, typename ReturnType, typename... Args>
__forceinline ReturnType CallVirtualFunc(void* instance, Args... args) //i dont remember where i got this from but its a nice tool
{
    using Fn = ReturnType(__thiscall*)(void*, Args...);

    auto function = (*reinterpret_cast<Fn**>(instance))[Index];
    return function(instance, args...);
}

void jmphook(void*, void*);
void HookMethod(void* original, ...);

#endif