// mod.cpp
#include <windows.h>
#include <fstream>
#include <shlobj.h> // For SHGetFolderPath

bool IsMemoryReadable(void* addr, size_t size) {
    MEMORY_BASIC_INFORMATION mbi;
    if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
        return false;
    if (mbi.State != MEM_COMMIT)
        return false;

    // Check if protection allows reading
    if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))
        return false;

    // Sometimes PAGE_EXECUTE_READ, PAGE_READONLY, PAGE_READWRITE, PAGE_EXECUTE_READWRITE etc are valid
    if (mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))
        return true;

    return false;
}

bool IsMemoryWritable(void* addr, size_t size) {
    MEMORY_BASIC_INFORMATION mbi;
    if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
        return false;

    if (mbi.State != MEM_COMMIT)
        return false;

    if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))
        return false;

    // Check if protection allows writing
    if (mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_WRITECOPY))
        return true;

    return false;
}

bool IsMemoryExecutable(void* addr, size_t size) {
    MEMORY_BASIC_INFORMATION mbi;
    if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
        return false;

    if (mbi.State != MEM_COMMIT)
        return false;

    if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))
        return false;

    // Check if protection allows execution
    if (mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))
        return true;

    return false;
}


bool WriteCode(LPVOID pAddress, int depth, void* bytesOld, void* bytes, int byteCount){
    int maxWaitMs = 10000;
       // Resolve multilevel pointer, if depth > 0
    std::ofstream file("C:\\stupid\\a.txt");
    file << "Writing code." << std::endl;
    for (int i = 0; i < depth; ++i) {
        if (IsBadReadPtr(pAddress, sizeof(LPVOID))) {
            file << "IsBadReadPtr." << std::endl;
            file.close();
            return false;
        }
        pAddress = *((LPVOID*)pAddress);
    }

    // // Ensure the address is valid before writing
    // if (IsBadWritePtr(pAddress, byteCount)) {
    //     file << "IsBadWritePtr." << std::endl;
    //     file.close();
    //     return false;
    // }
    

    //legobatman.exe+1C356D - 80 87 C7150000 FF     - add byte ptr [edi+000015C7],-01
    int waited = 0;
    while (waited < maxWaitMs) {
        if (IsMemoryReadable(pAddress, byteCount)) {
            // TODO: figure out if you can avoid this vvv
            if (memcmp(pAddress, bytesOld, byteCount) == 0) {
                break; // Pattern matched, safe to patch
            }
        }
        Sleep(200);
        waited += 200;
    }
    


    file << "Memory is valid. Attempting patch..." << std::endl;


    // Change memory protection to allow writing
    DWORD oldProtect;
    if (!VirtualProtect(pAddress, byteCount, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        file << "Failed to change protections." << std::endl;
        file.close();
        return false;
    }
    // Write the bytes
    memcpy(pAddress, bytes, byteCount);

    // Restore the original protection
    VirtualProtect(pAddress, byteCount, oldProtect, &oldProtect);

    return true;

}

void HookFunc() {

}


DWORD WINAPI ThreadProc(LPVOID lpParam) {
    HMODULE hSelf = (HMODULE)lpParam;

    // Prevent the DLL from being unloaded
    HMODULE dummy;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)hSelf, &dummy);

    std::ofstream file("C:\\stupid\\a.txt");
    file << "ThreadProc started" << std::endl;
    file.close();

    // DWORD BASE_ADDR;// = (DWORD)GetModuleHandle(nullptr);
    
    // while ((BASE_ADDR = (DWORD)GetModuleHandle(nullptr)) == 0) {
    //     Sleep(50);
    // }

    // // Wait for LEGOBatman.exe to load.
    // HMODULE hModule = nullptr;
    // while (hModule == nullptr) {
    //     hModule = GetModuleHandleA("LEGOBatman.exe"); // use your exe's real name here
    //     Sleep(50);
    // }
    
    HMODULE hModule = nullptr;
    while ((hModule = GetModuleHandleA("LEGOBatman.exe")) == nullptr) {
        Sleep(50);
    }
    DWORD BASE_ADDR = (DWORD)hModule;
    
    BYTE NOP[16] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90};

    BYTE* batman = *((BYTE**)(BASE_ADDR + 0x006CA830));
    // 7 byte add function


    BYTE* dmgFuncAddr = (BYTE*)(BASE_ADDR + (0x1C356D));


    ///////////////TODO: This sucks but I just can't find a good way to do this :(
    // to prevent reading/writing too early. this includes both writing code and writing data.
    Sleep(30000);


    //BYTE* myFuncAddr = HookFunc;
    //memcpy(dmgFuncAddr,NOP,7);
    WriteCode(dmgFuncAddr, 0, (BYTE[]){0x80,0x87,0xC7,0x15,0x00,0x00,0xFF}, NOP, 7);

    while (true) {
        std::ofstream file("C:\\stupid\\a.txt", std::ios::app);
        file << std::hex << (int) batman << " -> " << (int) (*batman) << std::endl; // write whether enabled.
        file.close();
        if (*batman == 0x00) *batman = 0x03;
        else *batman = 0x00;
        Sleep(1000);
    }

    // Never reached, but good practice:
    FreeLibraryAndExitThread(hSelf, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule); // Prevent thread attach/detach notifications
        HANDLE hThread = CreateThread(NULL, 0, ThreadProc, (LPVOID)hModule, 0, NULL);
        if (hThread) CloseHandle(hThread);
    }
    return TRUE;
}