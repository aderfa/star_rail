#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "bypass.h"
#include "hook.h"
#include "utils.h"
#include "BypassEnable.h"
#include "kntl.h"
#include "Logger.h"
#include "kiero/minhook/include/MinHook.h"
#include "FpsUnlock.h"
#include "sh.h"
#include "ult.h"
#include "e.h"
#include "gm.h"
#include "mesum.h"
#include "autobattle.h"
#include "globals.h"
#include "ohk.h"
#include "player.h"
#include "short.h"


#define NAMA "Botline"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;