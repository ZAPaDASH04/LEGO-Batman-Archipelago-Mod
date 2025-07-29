/**
 * @file mod.cpp
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com)
 * @brief 
 * @version 0.2
 * @date 2025-07-07
 * 
 */

#include "LB1AP.h"

#include <windows.h>
#include <fstream>
#include <shlobj.h> // For SHGetFolderPath

std::ofstream file;

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

// bool IsMemoryExecutable(void* addr, size_t size) {
//     MEMORY_BASIC_INFORMATION mbi;
//     if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
//         return false;

//     if (mbi.State != MEM_COMMIT)
//         return false;

//     if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))
//         return false;

//     // Check if protection allows execution
//     if (mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))
//         return true;

//     return false;
// }

bool WaitForExecutableMemory(void* addr, DWORD timeoutMs = 10000) {
    DWORD elapsed = 0;
    MEMORY_BASIC_INFORMATION mbi;
    //std::ofstream file("a.txt", std::ios::app);
    file << "WaitForExecutableMemory." << std::endl;

    while (elapsed < timeoutMs) {
        if (VirtualQuery(addr, &mbi, sizeof(mbi))) {
            file << "Checking state." << std::endl;
            if (mbi.State == MEM_COMMIT &&
                (mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
                file << "Succeeded." << std::endl;
                return true;
            }
        }
        file << "VirtualQuery Failed." << std::endl;
        Sleep(50);
        elapsed += 50;
    }
    return false;
}

bool WriteCode(LPVOID pAddress, int depth, void* bytesOld, void* bytes, int byteCount){
    int maxWaitMs = 20000;
       // Resolve multilevel pointer, if depth > 0
    //std::ofstream file("a.txt", std::ios::app);
    file << "Writing code." << std::endl;
    file << "Waiting for executable memory..." << std::endl;
    if (!WaitForExecutableMemory((void*)pAddress)) {
        // Now it's safe to patch
        file << "Failed. Memory never became executable." << std::endl;
        return false;
    }
    // for (int i = 0; i < depth; ++i) {
    //     if (IsBadReadPtr(pAddress, sizeof(LPVOID))) {
    //         file << "IsBadReadPtr." << std::endl;
    //         return false;
    //     }
    //     pAddress = *((LPVOID*)pAddress);
    // }

    // // Ensure the address is valid before writing
    // if (IsBadWritePtr(pAddress, byteCount)) {
    //     file << "IsBadWritePtr." << std::endl;
    //     return false;
    // }
    

    //legobatman.exe+1C356D - 80 87 C7150000 FF     - add byte ptr [edi+000015C7],-01
    int waited = 0;
    while (waited < maxWaitMs) {
        if (IsMemoryReadable(pAddress, byteCount)) {
            // TODO: figure out if you can avoid this vvv
            if (memcmp(pAddress, bytesOld, byteCount) == 0) {
                file << "Matches." << std::endl;
                break; // Pattern matched, safe to patch
            }
            file << "Doesn't Match." << std::endl;
        }
        Sleep(200);
        waited += 200;
    }

    if (waited >= maxWaitMs) {
        file << "Failed to write. " << std::endl;
        return false;
    }
    


    file << "Memory is valid. Attempting patch..." << std::endl;


    // Change memory protection to allow writing
    DWORD oldProtect;
    if (!VirtualProtect(pAddress, byteCount, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        file << "Failed to change protections." << std::endl;
        return false;
    }
    file << "VirtPro." << std::endl;
    // Write the bytes
    memcpy(pAddress, bytes, byteCount);
    file << "Written." << std::endl;

    // Restore the original protection
    VirtualProtect(pAddress, byteCount, oldProtect, &oldProtect);
    file << "ReVirtPro." << std::endl;

    return true;

}

void HookFunc() {

}

DWORD WINAPI ThreadProc(LPVOID lpParam) {
    HMODULE hSelf = (HMODULE)lpParam;

    // Prevent the DLL from being unloaded
    HMODULE dummy;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)hSelf, &dummy);
    

    file.open("a.txt");
    std::cout.rdbuf(file.rdbuf());
    std::cerr.rdbuf(file.rdbuf());
    freopen("a.txt", "a", stdout);
    freopen("a.txt", "a", stderr);    
    // FILE* fp;
    // if (freopen_s(&fp, "a.txt", "a", stdout) != 0) {
    //     std::cerr << "Failed to redirect stdout\n";
    //     return 1;
    // }
    setvbuf(stdout, NULL, _IONBF, 0);
    std::cout << "hello world" << std::endl;
    std::cerr << "error world" << std::endl;
    printf("does this work\n");
    file << "ThreadProc started" << std::endl;

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
        file << "Waiting for Module." << std::endl;
        Sleep(50);
    }
    file << "Module found." << std::endl;
    DWORD BASE_ADDR = (DWORD)hModule; 
    DWORD UP0 = 0x0; // initial version
    DWORD UP1 = 0x1000; // 7/9/2025 update | WARN: LEGO Batman suddenly updated and I assume this +0x1000 is the general fix for it at least when it comes to data addresses. code addresses vary.
    DWORD UP = UP0 + UP1;
    
    BYTE NOP[16] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90};


    // Wait for data and code to be readable and writable

    ///////////////TODO: This sucks but I just can't find a good way to do this :(
    // WARN: does not work yet. just inject after in level.
    //Sleep(30000);
    BYTE* saveFile = (BYTE*)(BASE_ADDR + UP + (0x56801C));
    BYTE* level = (BYTE*)(BASE_ADDR + UP + (0x6C98C4)); // This may not be working somehow

    while (*level == 0x00) {
        Sleep(500); 
        // this is flawed as it often crashes on batman robin loading
    }
    file << "Level is " << std::hex << (int)*level << std::endl;
    file << "Save file is " << (int)*saveFile << std::endl;
    // TODO: wait for player to gain control?
    if (*saveFile == 0xFF) {
        // NEW GAME started
        // TODO: Find some way to make the player save.
    } else {
        // preexisting save file.
    }


    // Temporary pointers for testing. will be moved later.
    BYTE* levelBeatenH1_1 = *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + -0x4C5; // you can bank on batman beaten
    BYTE* levelBeatenH1_2 = *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + -0x4B9;
    BYTE* levelUnlockedH1_1 = *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + -0x4C6;
    BYTE* levelUnlockedH1_2 = *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + -0x4BA; // An icy reception unlocked.

    file << "levelBeatenH1_1: " << std::hex << (int)*levelBeatenH1_1 << std::endl;
    file << "levelBeatenH1_2: " << std::hex << (int)*levelBeatenH1_2 << std::endl;
    file << "levelUnlockedH1_1: " << std::hex << (int)*levelUnlockedH1_1 << std::endl;
    file << "levelUnlockedH1_2: " << std::hex << (int)*levelUnlockedH1_2 << std::endl;

    BYTE* batman =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x00;
    BYTE* robin  =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x01;
    BYTE* brucew =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x18;
    BYTE* char04 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x13;
    BYTE* char05 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x02;
    BYTE* char06 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x11;
    BYTE* char07 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x1B;
    BYTE* char08 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x40;
    BYTE* char09 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0xCB;
    BYTE* char10 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x32;
    BYTE* char11 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x30;
    BYTE* char12 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x42;
    BYTE* char13 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x2B;
    BYTE* char14 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x2D;
    BYTE* char15 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x43;
    BYTE* char16 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0F;
    BYTE* char17 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x16;
    BYTE* char18 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x15;
    BYTE* char19 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x07;
    BYTE* char20 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0D;
    BYTE* char21 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x19;
    BYTE* char22 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x04;
    BYTE* char23 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x05;
    BYTE* char24 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0E;
    BYTE* char25 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x10;
    BYTE* char26 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0C;
    BYTE* char27 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0B;
    BYTE* char28 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x14;
    BYTE* char29 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x1A;
    BYTE* char30 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x21;
    BYTE* char31 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x09;
    BYTE* char32 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x0A;
    BYTE* char33 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x44;
    BYTE* char34 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x46;
    BYTE* char35 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x3B;
    BYTE* char36 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x3D;
    BYTE* char37 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x39;
    BYTE* char38 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x3A;
    BYTE* char39 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x33;
    BYTE* char40 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x34;
    BYTE* char41 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x29;
    BYTE* char42 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x36;
    BYTE* char43 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x37;
    BYTE* char44 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x38;
    BYTE* char45 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x08;
    BYTE* char46 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x12;
    BYTE* char47 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x49;
    BYTE* char48 =      *((BYTE**)(BASE_ADDR + UP + 0x006CA830)) + 0x4A;
    BYTE* characters[48] = {batman, robin, brucew, char04, char05, char06, char07, char08, char09, char10, char11, char12, char13, char14, char15, char16, char17, char18, char19, char20, char21, char22, char23, char24, char25, char26, char27, char28, char29, char30, char31, char32, char33, char34, char35, char36, char37, char38, char39, char40, char41, char42, char43, char44, char45, char46, char47, char48};
    // 7 byte add function
    // subtracting offset UP1 and adding 0x20 added by update 1. this means that the new code added in update 1 is after the damage code.
    // U0 -> BASE_ADDR + 1C356D
    // U1 -> BASE_ADDR + 1C358D
    BYTE* dmgFuncAddr = (BYTE*)(BASE_ADDR + (0x20) + (0x1C356D)); // not the damage function instead it's a pointer to an add function that adds -1 to health.
    



    /*////////////////////////////////
    -////  Pre Loop Setup Begin  ////-
    ////////////////////////////////*/


    // AP testing.

    //TODO: will need to be fool-proofed, remove duplicate code, and probably can modularize this, but testing initial connection & proof of concept
    std::ifstream connectionFile("APConnect.txt");
    std::cout << "Connector File open" << std::endl;
    std::string line {};
    std::getline(connectionFile, line); //to skip pass the header line
    std::getline(connectionFile, line);
    line.push_back(':');
    char serverURL[line.length() + 1];
    strcpy(serverURL, line.c_str());
    std::getline(connectionFile, line);
    char serverPort[line.length() + 1];
    strcpy(serverPort, line.c_str());
    std::getline(connectionFile, line);
    char playerName[line.length() + 1];
    strcpy(playerName, line.c_str());
    if(!connectionFile.eof()){ //TODO: to test how a password with an archi server works. Initial read through of the documentation appears to have the server tell the player?
        std::getline(connectionFile, line);   
        char password[line.length() + 1]; //TODO: when modularizing/researching this, need to have password survive the scope.
        strcpy(password, line.c_str());
    }
    connectionFile.close();
    char* serverIP = strcat(serverURL, serverPort);
    std::cout << "Finished reading file" << std::endl;
    std::cout << "Attempting initial connect" << std::endl;
    LB1AP_Init(serverIP, playerName, "");
    
    //Turn off damage player function
    //file << "Patching damage function..." << std::endl;
    //WriteCode(dmgFuncAddr, 0, (BYTE[]){0x80,0x87,0xC7,0x15,0x00,0x00,0xFF}, NOP, 7);
    //file << "Patched damage function." << std::endl;

    // for (DWORD i = 0; i < 48; i++) {
    //     if (*(characters[i]) == 0x03) *(characters[i]) = 0x00;
    //     //if (i%2 == 0) *(characters[i]) = 0x03;
    // }

    // int cou = -1; // count for cycling

    /*//////////////////////////////
    -////  Pre Loop Setup End  ////-
    //////////////////////////////*/


    file << "About to loop." << std::endl;

    while (true) {
        // file.open("a.txt", std::ios::app);
        // file << std::hex << (int) batman << " -> " << (int) (*batman) << std::endl; // write whether enabled.
        // file.close();
        

        // AP STUFF

        if(*levelBeatenH1_1 == 1 && LB1AP_location_checked(15868690003) == false){ //TODO: make this a better check rather than an if statement
            LB1AP_send_item(15868690003); //TODO: make this not hardcoded
            LB1AP_CheckLocation(15868690003);
            printf("Sent levelBeatenH1_1\n");
        }

        // cou = cou % 3;
        // for (DWORD i = 0; i < 48; i++) {
        //     if ((i+cou+(i/12))%3 == 0) {// even
        //         *(characters[i]) = 0x03;
        //         //if (*(characters[i]) == 0x00) *(characters[i]) = 0x03; else *(characters[i]) = 0x00;
        //     } else {
        //         *(characters[i]) = 0x00;
        //         //if (*(characters[i]) == 0x03) *(characters[i]) = 0x00; else *(characters[i]) = 0x03;
        //     }
        // }
        // cou++;

        Sleep(500);
    }

    file.close(); // close file before infinite loop. //temporarily moved to test sending/receiving items via archi

    // Never reached but good practice
    FreeLibraryAndExitThread(hSelf, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    // WARNING: minimize code in this function.
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule); // Prevent thread attach/detach notifications
        HANDLE hThread = CreateThread(NULL, 0, ThreadProc, (LPVOID)hModule, 0, NULL);
        if (hThread) CloseHandle(hThread);
    }
    return TRUE;
}