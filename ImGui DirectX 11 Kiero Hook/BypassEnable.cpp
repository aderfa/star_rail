#define _CRT_SECURE_NO_WARNINGS
#include "kiero/minhook/include/MinHook.h"
#include <iostream>
#include <windows.h>
#include "BypassEnable.h"
#include "hook.h"
#include "bypass.h"

namespace enablebypass_new {
    void init() {
        AllocConsole();

        freopen("CONOUT$", "w", stdout);

        bypass::init();

        hook::init();

        
    }

    void enablebypass() {
        
    }
}
