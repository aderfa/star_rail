#include <Windows.h>
#include <vector>
#include <TlHelp32.h>
#include "includes.h"
#include "FpsUnlock.h"

namespace MemoryChanger {
    HANDLE hProcess;

    template <typename T>
    T Read(LPVOID Address)
    {
        T Data;
        ReadProcessMemory(hProcess, Address, &Data, sizeof(T), nullptr);
        return Data;
    }

    template <typename T>
    void Write(LPVOID Address, T Data)
    {
        WriteProcessMemory(hProcess, Address, &Data, sizeof(T), nullptr);
    }

    uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
    {
        MODULEENTRY32W modEntry{};
        modEntry.dwSize = sizeof(modEntry);
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
        if (hSnap != INVALID_HANDLE_VALUE && Module32FirstW(hSnap, &modEntry)) {
            do {
                if (!_wcsicmp(modEntry.szModule, modName)) {
                    return (uintptr_t)modEntry.modBaseAddr;
                }
            } while (Module32NextW(hSnap, &modEntry));
        }
        CloseHandle(hSnap);
        return 0;
    }

    void SearchAndReplace(const std::vector<BYTE>& searchBytes, const std::vector<BYTE>& replaceBytes)
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        size_t searchSize = searchBytes.size();
        for (BYTE* pAddress = (BYTE*)sysInfo.lpMinimumApplicationAddress; pAddress < sysInfo.lpMaximumApplicationAddress; pAddress += sysInfo.dwPageSize) {
            std::vector<BYTE> buffer(searchSize);
            SIZE_T bytesRead;
            if (ReadProcessMemory(hProcess, pAddress, buffer.data(), searchSize, &bytesRead) && std::equal(buffer.begin(), buffer.end(), searchBytes.begin())) {
                WriteProcessMemory(hProcess, pAddress, replaceBytes.data(), replaceBytes.size(), nullptr);
            }
        }
    }

    void MemoryChangerImGui()
    {
        DWORD procId = GetCurrentProcessId();
        if (procId) {
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
            ImGui::Text("Memory Limit Fps Ditemukan.");
            uintptr_t address = GetModuleBaseAddress(procId, L"unityplayer.dll") + 0x1C4E000;
            int value = Read<int>((LPVOID)address);
            ImGui::Text("Batas Fps Saat ini : %d", value);
            ImGui::Separator();
            static int newValue = 0;
            ImGui::InputInt("Fps Baru : ", &newValue);
            if (ImGui::Button("Set Fps")) {
                Write<int>((LPVOID)address, newValue);
                ImGui::Text("Berhasil.");
            }
        }
        else {
            ImGui::Text("Process not found!");
        }
    }
}
