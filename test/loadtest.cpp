// loadtest.cpp
#include <windows.h>
#include <iostream>

int main() {
    std::cout << "test.\n";
    HMODULE mod = LoadLibraryA("C:\\Program Files (x86)\\Steam\\steamapps\\common\\Lego Batman\\ap.dll");
    if (mod) {
        std::cout << "DLL loaded successfully.\n";
        FreeLibrary(mod);
    } else {
        std::cerr << "DLL load failed. Error: " << GetLastError() << "\n";
    }
}

// Refused to load library
// detected as viruse when global address added.

// 0xC000007B
// i686-w64-mingw32-g++ -m32 -o .\test\loadtest.exe .\test\loadtest.cpp -static-libgcc -static-libstdc++