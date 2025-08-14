/**
 * @file mod.cpp
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @authors jr (jradcode23@gmail.com) @jr5768
 * @brief 
 * @version 0.3
 * @date 2025-07-07
 * 
 */

#include "LB1AP.h"

#include <windows.h>
#include <fstream>
#include <shlobj.h> // For SHGetFolderPath

#include "game.h"
#include "hintmessagebox.h"

std::ofstream file;
std::ofstream b_file;


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

bool WriteCode(LPVOID pAddress, void* bytesOld, void* bytes, int byteCount){
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


void hugeTest(Game game) {
    std::cout << "Huge test" << std::endl;
    std::cout << "saveSlot " << std::hex << (int) game.saveSlot << std::endl;
    
    
}

void loopTest(Game game, DWORD loops) {
    if (loops%500 == 0) {
        std::cout << "Loop test" << std::endl 
                  //<< std::hex << game.currentLevel << std::endl
                  << "levels:" << std::endl
                  << "Hero:" << std::endl;
        
        // for (size_t i = 0; i < 16; i++)
        // {
        //     std::cout << " lev" << std::dec << i << std::hex;
        //     std::cout << " " << (int) *game.levels.levelUnlocked[i] 
        //               << " " << (int) *game.levels.levelBeaten[i] 
        //               << " " << (int) *game.levels.levelKitCount[i]
        //               << " " << (int) *game.levels.levelRedBrick[i];
        //               //<< " " << (int) (((*game.levels.hostages) & ((DWORD32)0x1 << i)) > 0);
            
        // }
        // std::cout << std::endl << "Villain:" << std::endl;
        // for (size_t i = 16; i < 32; i++)
        // {
        //     std::cout << " lev" << std::dec << i << std::hex;
        //     std::cout << " " << (int) *game.levels.levelUnlocked[i] 
        //               << " " << (int) *game.levels.levelBeaten[i] 
        //               << " " << (int) *game.levels.levelKitCount[i]
        //               << " " << (int) *game.levels.levelRedBrick[i];
        //               //<< " " << (int) (((*game.levels.hostages) & ((DWORD32)0x1 << i)) > 0);
        // }

        std::cout << std::endl << "inlevel stuff" << std::endl 
                  << std::hex << (int) game.currentLevel << " " 
                  << std::hex << (int) game.inLevelTotalKitCount << " " 
                  << std::hex << (int) game.inLevelKitCount << std::endl;
        for (size_t i = 0; i < game.inLevelKitCount; i++)
        {
            std::cout << " " << std::hex << (int) game.inLevelKitLocations[i] << " " << game.inLevelKits[i];
        }
        std::cout << std::endl;
        
        

        // kit stuff
        std::cout << "Kit save data stuff" << std::endl;
        std::cout << std::hex << game.levels.levelKitSaveData << std::endl;
        std::cout << std::hex << &(game.levels.levelKitSaveData[0x12]) << std::endl;
        std::cout << (char*)game.levels.levelKitSaveData[0x12].kits[0] << std::endl << std::endl;


        
    }
    
    
}


DWORD WINAPI ThreadProc(LPVOID lpParam) {
    HMODULE hSelf = (HMODULE)lpParam;

    // Prevent the DLL from being unloaded
    HMODULE dummy;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)hSelf, &dummy);
    

    file.open("a.txt");
    b_file.open("b.txt");
    std::cout.rdbuf(file.rdbuf());
    std::cerr.rdbuf(file.rdbuf());
    freopen("b.txt", "a", stdout);
    freopen("b.txt", "a", stderr);
    setvbuf(stdout, NULL, _IONBF, 0);
    std::cout << "hello world" << std::endl;
    std::cerr << "Error world" << std::endl;
    printf("Does this work\n");
    file << "ThreadProc started" << std::endl;

    
    
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
    
    Game game(BASE_ADDR + UP);
    HintMessageBox messageBox(BASE_ADDR + UP);

    // example
    hugeTest(game);

    
    BYTE NOP[16] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90};


    // Wait for data and code to be readable and writable

    ///////////////TODO: This sucks but I just can't find a good way to do this :(
    // WARN: does not work yet. just inject after in level.
    //Sleep(30000);
    volatile BYTE saveFile = game.saveSlot; // (BYTE*)(BASE_ADDR + UP + (0x56801C));
    volatile BYTE level = game.currentLevel; // This may not be working somehow

    while (level == 0x00) {
        Sleep(500); 
        // this is flawed as it often crashes on batman robin loading
    }
    file << "Level is " << std::hex << (int)level << std::endl;
    file << "Save file is " << (int)saveFile << std::endl;
    // TODO: wait for player to gain control?
    if (saveFile == 0xFF) {
        // NEW GAME started
        // TODO: Find some way to make the player save.
    } else {
        // preexisting save file.
    }


    
    // 7 byte add function
    // subtracting offset UP1 and adding 0x20 added by update 1. this means that the new code added in update 1 is after the damage code.
    // U0 -> BASE_ADDR + 1C356D
    // U1 -> BASE_ADDR + 1C358D
    BYTE* dmgFuncAddr = (BYTE*)(BASE_ADDR + (0x20) + (0x1C356D)); // not the damage function instead it's a pointer to an add function that adds -1 to health.
    

    // Message box code nops

    // code that sets the hint id. LEGOBatman.exe+1D522D - 89 35 246FAC00        - mov [LEGOBatman.exe+6C6F24],esi
    WriteCode((BYTE*)(BASE_ADDR + 0x001D522D),(BYTE[]){0x89,0x35,0x24,0x6F,0xAC,0x00},NOP,6);
    // code that sets the hint id to 0 when timer > ~7.5. LEGOBatman.exe+1D5574 - C7 05 246FAC00 00000000 - mov [LEGOBatman.exe+6C6F24],00000000
    WriteCode((BYTE*)(BASE_ADDR + 0x001D5574),(BYTE[]){0xC7,0x05,0x24,0x6F,0xAC,0x00,0x00,0x00,0x00,0x00},NOP,10);
    // code that lowers timer. LEGOBatman.exe+1D5550 - D9 1D 346FAC00        - fstp dword ptr [LEGOBatman.exe+6C6F34]
    WriteCode((BYTE*)(BASE_ADDR + 0x001D5550),(BYTE[]){0xD9,0x1D,0x34,0x6F,0xAC,0x00},NOP,6);
    // code that resets timer to 0. LEGOBatman.exe+1D5221 - D9 15 346FAC00        - fst dword ptr [LEGOBatman.exe+6C6F34]
    WriteCode((BYTE*)(BASE_ADDR + 0x001D5221),(BYTE[]){0xD9,0x15,0x34,0x6F,0xAC,0x00},NOP,6);
    // code that resets time to 0 on level change? LEGOBatman.exe+1D4E3D - D9 15 346FAC00        - fst dword ptr [LEGOBatman.exe+6C6F34]
    WriteCode((BYTE*)(BASE_ADDR + 0x001D4E3D),(BYTE[]){0xD9,0x15,0x34,0x6F,0xAC,0x00},NOP,6);


    /*////////////////////////////////
    -////  Pre Loop Setup Begin  ////-
    ////////////////////////////////*/


    // AP testing.
    LB1AP_Connect();
    
    //Turn off damage player function
    //file << "Patching damage function..." << std::endl;
    //WriteCode(dmgFuncAddr, 0, (BYTE[]){0x80,0x87,0xC7,0x15,0x00,0x00,0xFF}, NOP, 7);
    //file << "Patched damage function." << std::endl;

    ///////// TODO: do a loop of all memory for missed checks.


    // WARN: temporary setup for testing.

    for (size_t i = 0; i < 32; i++)
    {
        *game.levels.levelUnlocked[i] = 1;
        *game.levels.levelBeaten[i] = 1;
    }
    
    for (size_t i = 0; i < game.characters.characterCount; i++)
    {
        //std::cout << "char " << std::hex << (int) game.characters._characterBytes[i] << std::endl;
        *game.characters._characterBytes[i] = 0x03;
        //*game.characters[i] = 0x03;
    }

    // std::cout << "extra purch " << std::hex
    //       << reinterpret_cast<uintptr_t>(&game.extraPurchased)
    //       << std::endl;

    // purchase all kits.
    for (size_t i = 0; i < 21; i++)
    {
        game.extraPurchased |= (1 << i);
    }
    // auto minkit detector on
    *game.extraEnabled[ExtraName::Minikit_Detector] = 1;
    
    std::cout << "suit unlock " << std::hex
          << reinterpret_cast<uintptr_t>(&game.suitUnlocked1)
          << std::endl;

    // unlock all suits
    for (size_t i = 0; i < 10; i++) 
    {
        game.suitUnlocked1 |= (WORD)(1 << i);
        game.suitUnlocked2 |= (WORD)(1 << i);
    }

    /*//////////////////////////////
    -////  Pre Loop Setup End  ////-
    //////////////////////////////*/


    file << "About to loop." << std::endl;

    DWORD loops = 0;
    SubLevelKits* saveKitData = game.levels.levelKitSaveData;
    SubLevelKits levelKitData;
    while (true) {
        
        loopTest(game,loops);

        // AP STUFF

        // if(*levelBeatenH1_1 == 1 && LB1AP_location_checked(15868690003) == false){ //TODO: make this a better check rather than an if statement
        //     LB1AP_send_item(15868690003); //TODO: make this not hardcoded
        //     LB1AP_CheckLocation(15868690003);
        //     printf("Sent levelBeatenH1_1\n");
        // }
        for (BYTE i = game.inLevelKitCountPrev; i < game.inLevelKitCount; i++)
        {
            // new kit picked up.
            std::cout  
                << (int) *game.inLevelKitLocations[i] << " " << game.inLevelKits[i] << " : "
                << (int) game.minikits.findKitIndex(*game.inLevelKitLocations[i], game.inLevelKits[i])
                << std::endl;
            
            LB1AP_send_item(400000 + 100 + game.minikits.findKitIndex(*game.inLevelKitLocations[i], game.inLevelKits[i]));
            // levelKitData = game.levels.levelKitSaveData[*game.inLevelKitLocations[i]];
            // strncpy(levelKitData.kits[levelKitData.count], game.inLevelKits[i], 8);
            game.inLevelKitCountPrev++;
        } 
        if (game.inLevelKitCount < game.inLevelKitCountPrev) {
            // left level
            // TODO: test
            game.inLevelKitCountPrev = 0;
        }

        //LB1AP_GetMessage();
        // if (AP_IsMessagePending()) {

        //     AP_Message* message = AP_GetLatestMessage();
        //     std::cout << message->text << std::endl;

        //     AP_ClearLatestMessage();
        // }
        messageBox.tick();

        Sleep(50);
        loops++;
    }

    file.close();
    b_file.close(); // close file before infinite loop. //temporarily moved to test sending/receiving items via archi

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
