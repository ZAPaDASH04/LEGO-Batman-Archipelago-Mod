// loadtest.cpp
#include <windows.h>
#include <iostream>

int main() {
    HMODULE mod = LoadLibraryA("C:\\stupid\\mod.dll");
    if (mod) {
        std::cout << "DLL loaded successfully.\n";
        FreeLibrary(mod);
    } else {
        std::cerr << "DLL load failed. Error: " << GetLastError() << "\n";
    }
}