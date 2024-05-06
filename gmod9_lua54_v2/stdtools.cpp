#include "stdtools.h"
#include "stdint.h"
#include "Color.h"
#include "tier0/dbg.h"

Color red(255, 0, 0);
Color pink(255, 128, 255);

#pragma pack(push, 1)
struct jmp_t {
    uint8_t _ = 0xE9;
    uintptr_t addr;
};
#pragma pack(pop)

void jmphook(void* original, void* myfunc)
{
    jmp_t jmp;
    jmp.addr = (uintptr_t)myfunc - (uintptr_t)original - 5;
    DWORD oldp;
    VirtualProtect((void*)original, 5, PAGE_EXECUTE_READWRITE, &oldp);
    memcpy(original, &jmp, sizeof(jmp));
    VirtualProtect(original, 5, oldp, &oldp);
}


void HookMethod(void* original, ...) {
    va_list args;
    va_start(args, original);
    void* myfunc = va_arg(args, void*);
    va_end(args);
    jmphook(original, myfunc);
}

void CPrint(Color c, const char* fmt, ...) {
    DWORD dwMode = 0;
    DWORD oldmode = 0;
    CONSOLE_SCREEN_BUFFER_INFO f;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hOut, &f);
    GetConsoleMode(hOut, &dwMode);
    oldmode = dwMode;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    char buffer[4096];
    va_list args;
    va_start(args, fmt);
    vsprintf_s(buffer, fmt, args);
    va_end(args);
    Msg("\033[38;2;%i;%i;%i\m", c.r(), c.g(), c.b());
    Msg(buffer);
    SetConsoleMode(hOut, oldmode);
    SetConsoleTextAttribute(hOut, f.wAttributes);
}
