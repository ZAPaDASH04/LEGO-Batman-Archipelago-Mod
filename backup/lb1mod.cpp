#include <Windows.h>
#include <iostream>
#include <fstream>

//extern int UImain();

// void WriteMemory(DWORD address, char* bytes, int bytenum){
//     unsigned long oldProtection;
//     VirtualProtect((LPVOID)address, bytenum, PAGE_EXECUTE_READWRITE, &oldProtection);
// }

// char* ReadMemory(LPVOID pAddress, char* bytes, int bytenum) {
//     unsigned long oldProtection;
//     VirtualProtect((LPVOID)pAddress, bytenum, PAGE_EXECUTE_READ, &oldProtection);
//     memcpy(bytes, (LPVOID)pAddress, bytenum);
//     VirtualProtect((LPVOID)pAddress, bytenum, oldProtection, NULL);
//     return bytes;
// }

// void WriteMemory(LPVOID pAddress, int depth, char* bytes, int bytenum){
//     unsigned long oldProtection;
//     LPVOID tmp;
//     if (depth>0) {
//         VirtualProtect((LPVOID)pAddress, 4, PAGE_EXECUTE_READ, &oldProtection);
//         memcpy(&tmp, (LPVOID)pAddress, sizeof(DWORD));
//         VirtualProtect((LPVOID)pAddress, 4, oldProtection, NULL);
//         return WriteMemory(tmp, depth-1, bytes, bytenum);
//     }
//     // correct depth.
//     VirtualProtect((LPVOID)pAddress, bytenum, PAGE_EXECUTE_READWRITE, &oldProtection);
//     memcpy((LPVOID)pAddress, bytes, bytenum);
//     VirtualProtect((LPVOID)pAddress, bytenum, oldProtection, NULL);

// }

// char* ReadMemory(LPVOID pAddress, int depth, char* bytes, int bytenum){
//     unsigned long oldProtection;
//     LPVOID tmp;
//     if (depth>0) {
//         VirtualProtect((LPVOID)pAddress, 4, PAGE_EXECUTE_READ, &oldProtection);
//         memcpy(&tmp, (LPVOID)pAddress, sizeof(DWORD));
//         VirtualProtect((LPVOID)pAddress, 4, oldProtection, NULL);
//         return ReadMemory(tmp, depth-1, bytes, bytenum);
//     }
//     // correct depth.
//     VirtualProtect((LPVOID)pAddress, bytenum, PAGE_EXECUTE_READ, &oldProtection);
//     memcpy(bytes, (LPVOID)pAddress, bytenum);
//     VirtualProtect((LPVOID)pAddress, bytenum, oldProtection, NULL);

//     return bytes;
// }


// DWORD WINAPI MainThread(LPVOID lpParam){
//     std::ofstream out("C:\\Users\\Joseph Howard\\Desktop\\REPOSITORIES\\Lego Archepelago\\LB1hooking\\test\\output.txt", std::ios::app);
    
//     char output[200] = {};
    
//     char batman[1];
//     while (true)
//     {
//         Sleep(10000);
//         out << (int) (ReadMemory((void*)0x006CA830,batman,1)[1]) << std::endl;
        
//         //sprintf(output, "%x", batman[0]);
//         //MessageBoxA(NULL, output, "Injection", MB_OK);
//     }
//     //UImain();
//     return S_OK;
// }

DWORD WINAPI MainThread(LPVOID lpParam)
{
    std::ofstream fout("C:\\Users\\Public\\stupid\\output.txt");
    if (fout.is_open()) {
        fout << "injection SUCCESS!" << std::endl;
        fout.close();
    } else {
        OutputDebugStringA("Failed to open output.txt");
    }
    while (true) {
        Sleep(10000);
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, 
                      DWORD ul_reason_for_call, 
                      LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, MainThread, nullptr, 0, nullptr);
    }
    return TRUE;
}