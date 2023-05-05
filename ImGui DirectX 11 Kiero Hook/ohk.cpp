#include "includes.h"
#include <stdint.h>

namespace onehit {
    void ohk() {
        uint64_t game_assembly = reinterpret_cast<uint64_t>(GetModuleHandleA("GameAssembly.dll"));

        if (!game_assembly) {
            printf("Failed to find GameAssembly.dll\n");
            Sleep(1000);
            return;
        }

        uintptr_t address = utils::read<uintptr_t>(utils::read<uintptr_t>(utils::read<uintptr_t>(utils::read<uintptr_t>(utils::read<uintptr_t>(utils::read<uintptr_t>(utils::read<uintptr_t>(game_assembly + 0x086D4C40) + 0xE70) + 0x60) + 0x370) + 0x50) + 0x110) + 0x28) + 0x54;

        uint32_t value = utils::read<uint32_t>(address);





        uint32_t value_as_int = 1;
        utils::write<uint32_t>(address, value_as_int);




        Sleep(1);
    }
}
