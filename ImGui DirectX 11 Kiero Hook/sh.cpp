#include "includes.h"

namespace sh {
    void menu() {
        uint64_t game_assembly = reinterpret_cast<uint64_t>(GetModuleHandleA("GameAssembly.dll"));
        uint64_t unity_player = reinterpret_cast<uint64_t>(GetModuleHandleA("unityplayer.dll"));

        if (!game_assembly || !unity_player) {
            printf("Failed to find GameAssembly.dll or unityplayer.dll\n");
            Sleep(1000);
            return;
        }

        uintptr_t address = utils::read<uintptr_t>(utils::read<uintptr_t>(game_assembly + 0x8CAA6A0) + 0xC0) + 0x1DC;
        float value = utils::read<float>(address);

        ImGui::Begin("Speed Hack");

        ImGui::Text("Speed Batlle %llx is %f", address, value);

        static float new_value = value;

        if (ImGui::InputFloat("New Value", &new_value, 0.1f, 1.0f)) {
            
        }

        if (ImGui::Button("Set kecepatan")) {
            utils::write<float>(address, new_value); 
        }

        uintptr_t new_address = utils::read<uintptr_t>(unity_player + 0x1D21D78) + 0xFC;
        float new_speed = utils::read<float>(new_address);

        ImGui::Text("Speed Normal World %llx is %f", new_address, new_speed);

        static float new_speed_value = new_speed;

        if (ImGui::InputFloat("Gass", &new_speed_value, 0.1f, 1.0f)) {
           
        }

        if (ImGui::Button("Set Speed")) {
            utils::write<float>(new_address, new_speed_value);
        }

        ImGui::End();

        Sleep(1);
    }
}
