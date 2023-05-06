#include "includes.h"
#include "utils.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include "typedef.h"
#include "globals.h"

namespace hook {
	bool create_hook(HookData& data, uintptr_t GameAssembly);

	namespace anti_cheat {
		HANDLE WINAPI h_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
		{
			if (memcmp(lpFileName, L"\\\\.\\ACE-BASE", 24) == 0) {
				wprintf(L"Thread (%i) attempting to communicate with anti-cheat driver -> %s\n", GetCurrentThreadId(), lpFileName);

				SuspendThread(GetCurrentThread()); // 200iq bypass for memory protection
			}

			return p_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		}
	}

	namespace game {
		int current_phase = 0;

		int get_currect_phase() {
			return current_phase;
		}

		__int64 __fastcall h_setcurrentphase(__int64 a1, int a2, __int64 a3, char a4) {
			current_phase = a2;

			return o_setcurrentphase(a1, a2, a3, a4);
		}

		char __fastcall h_setautobattleflag(__int64 a1, unsigned __int8 a2) {
			return o_setautobattleflag(a1, a2);
		}

		__int64 __fastcall h_isautobattle(__int64 a1) {
			auto ret = o_isautobattle(a1);

			if (!ret && globals::auto_battle) {
				h_setautobattleflag(a1, 1);
			}

			return ret;
		}
	}

	std::vector<HookData> v_hooks{
		{"setcurrentphase", 0x5B9E130, &game::h_setcurrentphase, &o_setcurrentphase},
		{"isautobattle", 0x512F580, &game::h_isautobattle, &o_isautobattle},
		{"setautobattleflag", 0x5130040,&game::h_setautobattleflag,&o_setautobattleflag},
	};

	void init() {


		if (MH_Initialize() != MH_OK)
		{
			puts("----------- START CONSOLE -----------");

			return;
		}

		if (MH_CreateHookApiEx(L"kernelbase", "CreateFileW", &anti_cheat::h_CreateFileW, reinterpret_cast<void**>(&p_CreateFileW), reinterpret_cast<void**>(&t_CreateFileW)) != MH_OK)
		{
			puts("Error creating hook for CreateFileW function");

			return;
		}

		if (MH_EnableHook(t_CreateFileW) != MH_OK)
		{
			puts("Error enabling hook for CreateFileW function");

			return;
		}

		puts("waiting game module..");

		uintptr_t base_address = 0;

		while (!base_address) base_address = reinterpret_cast<uintptr_t>(GetModuleHandleA("gameassembly.dll"));

		// mega deadcode

		if (utils::read<uint8_t>(base_address + v_hooks[0].rva) != 0x48) {
			v_hooks[0].rva = 0x5B9DFD0;
		}

		puts("hooks setup..");

		for (auto& hook : v_hooks) {
			if (!create_hook(hook, base_address)) {
				return;
			}
		}

		puts("hooks successfully created!");


		return;
	}

	bool create_hook(HookData& data, uintptr_t GameAssembly) {

		auto address = reinterpret_cast<void**>(GameAssembly + data.rva);


		if (MH_CreateHook(address, data.hook, reinterpret_cast<void**>(data.original)) != MH_OK) {
			printf("Error creating hook for %s\n", data.name.c_str());
			return false;
		}

		if (MH_EnableHook(address) != MH_OK) {
			printf("Error enabling hook for %s function\n", data.name.c_str());
			return false;
		}

		return true;
	}
}