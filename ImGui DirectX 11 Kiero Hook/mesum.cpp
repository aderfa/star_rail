#include "includes.h"
#include <stdint.h>

namespace intip {
void intip() {
    uint64_t game_assembly = reinterpret_cast<uint64_t>(GetModuleHandleA("GameAssembly.dll"));

    if (!game_assembly) {
        printf("Failed to find GameAssembly.dll\n");
        Sleep(1000);
        return;
    }


    utils::write<uint8_t>(game_assembly + 0x51292C0, 0xC3); // mengubah nilai di alamat tersebut dengan nilai dari input yang baru




    Sleep(1);
 }
}