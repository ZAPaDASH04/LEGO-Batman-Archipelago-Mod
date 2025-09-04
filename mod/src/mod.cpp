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

    // Prevent the DLL from being unloaded? TODO: I don't know if this works
    HMODULE dummy;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)hSelf, &dummy);
    

    file.open("a.txt");
    b_file.open("b.txt");
    std::cout.rdbuf(file.rdbuf());
    std::cerr.rdbuf(file.rdbuf());
    freopen("b.txt", "a", stdout);
    freopen("b.txt", "a", stderr);
    setvbuf(stdout, NULL, _IONBF, 0);
    std::cout << "cout test" << std::endl;
    std::cerr << "cerr test" << std::endl;
    printf("stdout test\n");
    //fprintf(stderr, "stderr test\n"); // TODO: untested
    file << "ThreadProc started" << std::endl;

    
    
    HMODULE hModule = nullptr;
    while ((hModule = GetModuleHandleA("LEGOBatman.exe")) == nullptr) {
        file << "Waiting for Module." << std::endl;
        Sleep(50);
    }
    file << "Module found." << std::endl;
    std::cout << "here" << std::endl;
    DWORD BASE_ADDR = (DWORD)hModule; 
    std::cout << "here1" << std::endl;
    DWORD UP0 = 0x0; // initial version
    DWORD UP1 = 0x1000; // 7/9/2025 update | WARN: LEGO Batman suddenly updated and I assume this +0x1000 is the general fix for it at least when it comes to data addresses. code addresses vary.
    DWORD UP = UP0 + UP1;
    
    // build the game data and message box
    std::cout << "before game" << std::endl;
    Game game(BASE_ADDR + UP);
    std::cout << "before message box" << std::endl;
    HintMessageBox messageBox(BASE_ADDR + UP);

    // Nops for overwriting code
    BYTE NOP[16] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90};




    // Wait for data and code to be readable and writable

    ///////////////TODO: This sucks but I just can't find a good way to do this :(
    // WARN: does not work yet. just inject after in level.
    //Sleep(30000);
    volatile BYTE saveFile = game.saveSlot; // (BYTE*)(BASE_ADDR + UP + (0x56801C));
    volatile BYTE level = game.currentLevel; // This may not be working somehow

    std::cout << "saveSlot " << std::hex << (int) game.saveSlot << std::endl;
    // while (level == 0x00) {
    //     Sleep(500); 
    //     // this is flawed as it often crashes on batman robin loading
    // }
    // file << "Level is " << std::hex << (int)level << std::endl;
    // file << "Save file is " << (int)saveFile << std::endl;
    // // TODO: wait for player to gain control?
    // if (saveFile == 0xFF) {
    //     // NEW GAME started
    //     // TODO: Find some way to make the player save.
    // } else {
    //     // preexisting save file.
    // }
    std::cout << "angy" << std::endl;
    while ((game.playerControlOnP == nullptr) || (*((*game.playerControlOnP) + 0x258) == 0)) {
        Sleep(10);
    }

    std::cout << "yippee" << std::endl;






    /*////////////////////////////////
    -////  Pre Loop Setup Begin  ////-
    ////////////////////////////////*/


    
    // 7 byte add function
    // subtracting offset UP1 and adding 0x20 added by update 1. this means that the new code added in update 1 is after the damage code.
    // U0 -> BASE_ADDR + 1C356D
    // U1 -> BASE_ADDR + 1C358D
    //BYTE* dmgFuncAddr = (BYTE*)(BASE_ADDR + (0x20) + (0x1C356D)); // not the damage function instead it's a pointer to an add function that adds -1 to health.
    

    // Message box code overwrites

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


    // disable game unlocking levels
    WriteCode((BYTE*)(BASE_ADDR + 0x0024E254),(BYTE[]){0xC6,0x44,0x90,0x02,0x01},NOP,5);
    WriteCode((BYTE*)(BASE_ADDR + 0x0024E155),(BYTE[]){0xC6,0x44,0x90,0x02,0x01},NOP,5);
    // for beating episode
    WriteCode((BYTE*)(BASE_ADDR + 0x000788BF),(BYTE[]){0xC6, 0x84, 0x81, 0xDA, 0x77, 0x00, 0x00, 0x01},NOP,8);

    // disable default levels
    *game.levels.levelUnlocked[0] = 0; // H1-1
    *game.levels.levelUnlocked[5] = 0; // H2-1
    *game.levels.levelUnlocked[10] = 0; // H3-1



    // AP testing.
    messageBox.holdMessage("Holy Archipelago Batman!!! Attempting to connect...");
    //TODO: add remove player movement
    LB1AP_Connect();
    messageBox.setText("Holy Archipelago Batman!!! Successfully connected...");
    messageBox.releaseMessage();
    //TODO: restore player movement

    
    //Turn off damage player function
    //file << "Patching damage function..." << std::endl;
    //WriteCode(dmgFuncAddr, 0, (BYTE[]){0x80,0x87,0xC7,0x15,0x00,0x00,0xFF}, NOP, 7);
    //file << "Patched damage function." << std::endl;






    ///////// TODO: do a loop of all memory for missed checks.







    // WARN: temporary setup for testing.





    // fully unlock all levels
    // for (size_t i = 0; i < 35; i++)
    // {
    //     //std::cout << std::hex << (void*)game.levels.levelUnlocked[i] << std::endl;
    //     *game.levels.levelUnlocked[i] = 1;
    //     *game.levels.levelBeaten[i] = 1;
    // }
    

    // std::cout << "extra purch " << std::hex
    //       << reinterpret_cast<uintptr_t>(&game.extraPurchased)
    //       << std::endl;

    // purchase all extras.
    // for (size_t i = 0; i < 21; i++)
    // {
    //     game.extraPurchased |= (1 << i);
    // }

    // Easy true status
    *game.extraEnabled[ExtraName::Always_Score_Multiply] = 1;
    //*game.extraEnabled[ExtraName::Stud_Magnet] = 1;
    // Detectors on
    *game.extraEnabled[ExtraName::Minikit_Detector] = 1;
    *game.extraEnabled[ExtraName::Power_Brick_Detector] = 1;
    
    
    // unlock all characters
    for (size_t i = 0; i < game.characters.characterCount; i++)
    {
        //std::cout << "char " << std::hex << (int) game.characters._characterBytes[i] << std::endl;
        *game.characters._characterBytes[i] = 0x03;
        //*game.characters[i] = 0x03;
    }

    // std::cout << "suit unlock " << std::hex
    //       << reinterpret_cast<uintptr_t>(&game.suitUnlocked1)
    //       << std::endl;

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
    //SubLevelKits* saveKitData = game.levels.levelKitSaveData;
    //SubLevelKits levelKitData;
    while (true) {
        
        loopTest(game,loops);

        /*//////////////
        *//// Shop ////*
        //////////////*/

        // TODO: test
        if (game.inShopSubMenuPrev != game.inShopSubMenu) {
            std::cout << "entered or exited shop." << std::endl;
            BYTE a;
            if (game.isInShop()) {
                // entered shop
                std::cout << "entered shop" << std::endl;

                // per level
                for (size_t i = 0; i < 30; i++)
                {
                    a = game.powerBrickState[i];
                    std::cout << i << " : " << std::hex << (int)a << std::endl;

                    // Collected Item // TODO: test
                    if (a & 0b0010) *game.levels.levelRedBrick[i] = 1;
                    else *game.levels.levelRedBrick[i] = 0;

                    // Purchased Location // TODO: test
                    if (a & 0b0100) game.extraPurchased |= (1 << i);
                    else game.extraPurchased &= ~(1 << i);
                    game.extraPurchasedPrev = game.extraPurchased;
                }

                
            } else {
                // left shop
                std::cout << "exited shop" << std::endl;

                // per level
                for (size_t i = 0; i < 30; i++)
                {
                    std::cout << i << " : " << std::hex << (int)a << std::endl;
                    a = game.powerBrickState[i];
                    
                    // Collected Location // TODO: test
                    if (a & 0b0001) *game.levels.levelRedBrick[i] = 1;
                    else *game.levels.levelRedBrick[i] = 0;

                    // Purchased Item // TODO: test
                    if (a & 0b1000) game.extraPurchased |= (1 << i);
                    else game.extraPurchased &= ~(1 << i);
                    game.extraPurchasedPrev = game.extraPurchased;
                }
                
            }
            game.inShopSubMenuPrev = game.inShopSubMenu;
        }


        /*////////////////////////////
        *//// Location Detection ////*
        ////////////////////////////*/

        // TODO: extract each of these to functions. probably in this file.

        // Minikits
        if (game.inLevelKitCount < game.inLevelKitCountPrev) {
            // left level
            game.inLevelKitCountPrev = 0;

        } else for (BYTE i = game.inLevelKitCountPrev; i < game.inLevelKitCount; i++)
        {
            // new kit picked up.
            std::cout  
                << (int) *game.inLevelKitLocations[i] << " " << game.inLevelKits[i] << " : "
                << (int) game.minikits.findKitIndex(*game.inLevelKitLocations[i], game.inLevelKits[i])
                << std::endl;
            
            LB1AP_send_item(LB1AP_LOCATION_ID_OFFSET + 100 + game.minikits.findKitIndex(*game.inLevelKitLocations[i], game.inLevelKits[i]));
            /** 
             * TODO: auto save the kit to the file.
             * levelKitData = game.levels.levelKitSaveData[*game.inLevelKitLocations[i]];
             * strncpy(levelKitData.kits[levelKitData.count], game.inLevelKits[i], 8);
             */ 
            game.inLevelKitCountPrev++;
        }



        // Levels
        for (size_t i = 0; i < 30; i++) {
            if (*game.levels.levelBeaten[i] != game.levels.levelBeatenOld[i]) {
                std::cout
                    << "new level beaten " << (int) i 
                    << std::endl;
                LB1AP_send_item(LB1AP_LOCATION_ID_OFFSET + 425 + i);
                game.levels.levelBeatenOld[i] = *game.levels.levelBeaten[i];
            }
        }
        


        // Hostages
        int hostageCheck = game.levels.checkHostages();
        if (hostageCheck != -1) { // TODO: I don't know if this is how logic works
            std::cout
                << "new hostage " << (int) hostageCheck 
                << ". hostagedata " << std::hex << (int) *game.levels.hostages
                << std::endl;
            LB1AP_send_item(LB1AP_LOCATION_ID_OFFSET + 400 + hostageCheck);
            
            // TODO: setup receives when hush is unlockable?
        }

        // True Status
        if (game.inLevelTrueStatus != game.inLevelTrueStatusPrev) {
            if (game.inLevelTrueStatus < game.inLevelTrueStatusPrev) {
                // left level
                game.inLevelTrueStatusPrev = 0;

            } else {
                // TrueStatus Get
                std::cout
                    << "new True Status"
                    << std::endl;
                // WARN: high probability to fail. NEEDS SIGNIFICANT TESTING    
                LB1AP_send_item(LB1AP_LOCATION_ID_OFFSET + 455 + sublevelToLevel(game.currentLevel));
                game.inLevelTrueStatusPrev = game.inLevelTrueStatus;
            }

        }

        // Red Brick Collected
        if (game.inLevelPowerBrick != game.inLevelPowerBrickPrev) {
            if (game.inLevelPowerBrick < 2) {
                // left level
                game.inLevelPowerBrickPrev = 0;

            } else {
                // PowerBrick Get
                std::cout
                    << "new Power Brick found"
                    << std::endl;
                // WARN: high probability to fail. NEEDS SIGNIFICANT TESTING    
                LB1AP_send_item(LB1AP_LOCATION_ID_OFFSET + 485 + sublevelToLevel(game.currentLevel));
                game.inLevelPowerBrickPrev = game.inLevelPowerBrick;
                game.powerBrickState[sublevelToLevel(game.currentLevel)] |= 0b0001;
            }

        }

        // Red Brick Purchased TODO: test
        if (game.isInShop() && (game.extraPurchasedPrev != game.extraPurchased)) {
            int powerBrickCheck = game.checkPowerBricks();
            std::cout
                << "new PowerBrick " << (int) powerBrickCheck
                << ". PowerBrickdata " << std::hex << (int) game.extraPurchased
                << std::endl;
            LB1AP_send_item(LB1AP_LOCATION_ID_OFFSET + 515 + powerBrickCheck);
            game.powerBrickState[powerBrickCheck] != 0b0100;
            game.extraPurchasedPrev = game.extraPurchased;
        }


        /*////////////////////////
        *//// Item Receiving ////*
        ////////////////////////*/

        while (!receiveQueue.empty())
        {
            int i = receiveQueue.front();
            std::cout << "Received an Item!" << std::endl;
            if (i < 100) {
                std::cerr << "You should not be getting this: %d (report to devs)" << std::endl;
            } else if (i < 400) { // Minikits
                std::cerr << "You should not be getting this: %d (report to devs)" << std::endl;
            } else if (i < 425) { // Hostages
                std::cerr << "You should not be getting this: %d (report to devs)" << std::endl;
            } else if (i < 455) { // Level Unlock
                std::cout << "Level Unlocked!" << std::endl;
                *game.levels.levelUnlocked[i-425] = 1;
                //*game.levels.levelBeaten[i-425] = 1; // WARN: temporary

            } else if (i < 485) { // True status
                std::cerr << "You should not be getting this: %d (report to devs)" << std::endl;
            } else if (i < 515) { // Red Brick
                // TODO: test
                std::cout << "Red Brick Collected!" << std::endl;
                game.powerBrickState[i-485] |= 0b0010;


            } else if (i < 545) { // Red Brick purchased
                // TODO: test
                std::cout << "Red Brick Unlocked!" << std::endl;
                game.powerBrickState[i-515] |= 0b1000;

                

            } else { // out of bound
                printf("Received Unknown Item: %d\n", i);
            }
            receiveQueue.pop();

        }
        


        /*////////////////////////
        *//// Post Detection ////*
        ////////////////////////*/

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
